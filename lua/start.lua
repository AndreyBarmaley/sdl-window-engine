-- require 'SWE'
assert(SWE, "SDL Window Engine only")

SWE.RegisterLuaDirectory("gui")
require 'gui_tools'
require 'gui_dialog'
require 'gui_editor'
require 'gui_android'
require 'gui_filebrowser'

SWE.RegisterResourceDirectory("gui")
SWE.RegisterResourceDirectory("examples")

SWE.SetDebug(true)
-- SWE.Dump(_G)

local function ReadCommanderConfig()
    local sharedir = SWE.SystemShareDirectories()
    if sharedir ~= nil then
        local file = SWE.SystemConcatePath(sharedir, "commander.json")
        SWE.Debug("check config:", file)
	local buf = SWE.BinaryBuf.ReadFromFile(file)
        if buf ~= nil then
            return SWE.JsonParse(buf:ToString())
        end
    end

    local cwd = SWE.SystemCurrentDirectory()
    -- calculate font size
    local dw,dh,df = SWE.DisplaySize()
    local fsz = 14
    if SWE.SystemMobileOs() == "android" then
	fsz = 32
    end
    if dw > 0 then
        fsz = ToInt(dw / 320 * 12)
    end
    return { cwd=cwd, fsz=fsz, width=640, height=480 }
end

local function SaveCommanderConfig(win, frs)
    if win.cmd then
        local sharedir = SWE.SystemShareDirectories()
        if sharedir ~= nil then
    	    SWE.SystemMakeDirectory(sharedir)
    	    local config = SWE.SystemConcatePath(sharedir, "commander.json")
    	    -- json format
	    local t = { fsz=frs.size, cwd=win.cmd.cwd, width=win.width, height=win.height }
	    SWE.BinaryBuf(SWE.TableToJson(t)):SaveToFile(config)
    	    SWE.Debug("save config:", config)
    	end
    end
end

local function ShowError(win, frs, err, start)
    local dirn, basen = SWE.SystemDirnameBasename(start)
    local pos1, pos2 = string.find(err, basen)

    print(res)

    if pos1 ~= nil then
	local pos3 = string.find(err, "%s", pos2)
        hdr = "error - " .. string.sub(err, pos1, pos3 - 2)
        local msg = string.sub(err, pos3 + 1)

        DialogInform(win, frs, hdr, msg)
    else
        DialogInform(win, frs, "error", err)
    end
end

local function DumpFingerEvents(event, obj)
    if event == SWE.Signal.GestureFingerUp then
	print("gesture finger up")
    elseif event == SWE.Signal.GestureFingerDown then
	print("gesture finger down")
    elseif event == SWE.Signal.GestureFingerLeft then
	print("gesture finger left")
    elseif event == SWE.Signal.GestureFingerRight then
	print("gesture finger right")
    elseif event == SWE.Signal.FingerMoveUp then
	print("finger move up")
    elseif event == SWE.Signal.FingerMoveDown then
	print("finger move down")
    elseif event == SWE.Signal.FingerMoveLeft then
	print("finger move left")
    elseif event == SWE.Signal.FingerMoveRight then
	print("finger move right")
    end
end

local function CommanderInit(win, frs, cwd)
    local cols = ToInt(win.width / frs.fixedWidth)
    local rows = ToInt(win.height / frs.lineHeight)

    -- create TerminalWindow
    local term = FileBrowserInit(win, frs, cwd,
		{ cols = cols, rows = rows, include = "%.lua$", exclude = { "start%.lua", "template%.lua" } })

    -- remove system buttons
    term.button1:SetVisible(false)
    term.button2:SetVisible(false)
    SWE.Scene.Remove(term.button1)
    SWE.Scene.Remove(term.button2)
    term.button1 = nil
    term.button2 = nil

    -- add new buttons
    term.bzo = TermLabelActionCreate("F-", frs, 1, term.rows - 1, term, SWE.Color.White)
    term.bzi = TermLabelActionCreate("F+", frs, term.cols - 5, term.rows - 1, term, SWE.Color.White)
    term.bed = TermLabelActionCreate("ED", frs, 6, term.rows - 1, term, SWE.Color.White)
    term.bed.disable = true

    -- add close button
    if SWE.SystemMobileOs() then
        term.cls = TermLabelActionCreate("X", frs, term.cols - 5, 0, term, SWE.Color.White)

        term.cls.MouseClickEvent = function(px,py,pb,rx,ry,rb)
            term:SetVisible(false)
	    return true
        end
    end

    term.FileEdit = function(self)
	local stat = SWE.SystemFileStat(term.result)
	if stat and not stat.isdir then
            term:SetVisible(false)
    	    local editor = EditorInit(term, term.frs, term.result)
    	    SWE.MainLoop(editor)
            term:SetVisible(true)
	end
    end

    term.bed.MouseClickEvent = function(px,py,pb,rx,ry,rb)
        term:FileEdit()
	return true
    end

    term.bzo.MouseClickEvent = function(px,py,pb,rx,ry,rb)
        local fsz = frs.size - 2
        if fsz > 10 then
	    SWE.PushEvent(SWE.Action.FontChanged, fsz, win)
	end
	return true
    end

    term.bzi.MouseClickEvent = function(px,py,pb,rx,ry,rb)
        local fsz = frs.size + 2
        SWE.PushEvent(SWE.Action.FontChanged, fsz, win)
	return true
    end

    term.TerminalResizeEvent = function()
        if term.parent ~= nil then
            local cols = ToInt(term.parent.width / term.frs.fixedWidth)
            local rows = ToInt(term.parent.height / term.frs.lineHeight)

	    if cols * term.frs.fixedWidth > term.parent.width then
		cols = cols - 1
	    end
    	    if rows * term.frs.lineHeight > term.parent.height then
		rows = rows - 1
	    end

            term:SetTermSize(cols,rows)
	    
            term.bzo:SetPosition(1 * term.frs.fixedWidth, (rows - 1) * term.frs.lineHeight)
            term.bzi:SetPosition((cols - 5) * term.frs.fixedWidth, (rows - 1) * term.frs.lineHeight)
            term.bed:SetPosition(6 * term.frs.fixedWidth, (rows - 1) * term.frs.lineHeight)

	    SWE.DisplayDirty()
        end
    end

    term.LocalUserEvent = function(self, event, obj)
        if event == SWE.Action.ItemSelected then
            term.bed.disable = obj.isdir
        end
	if event == SWE.Signal.GestureFingerLeft then
	    if term.log then
		return false
	    end

	    term.log = CatLogInit(win, term.frs, {})
	    SWE.MainLoop(term.log)

	    SWE.Scene.Remove(term.log)
	    term.log = nil

	    return true
	end
        return false
    end

    term.LocalKeyPressEvent = function(self, key, mod, scancode)
	if key == SWE.Key.F4 then
            term:FileEdit()
            return true
        end
        return false
    end

    return term
end

while true do
    local frs = {}
    local win = {}
    local cfg = ReadCommanderConfig()

    if cfg.cwd ~= nil then
	local fs = SWE.SystemFileStat(cfg.cwd)
	if fs == nil or not fs.isdir then
    	    cfg.cwd = SWE.SystemCurrentDirectory()
	end
    else
        cfg.cwd = SWE.SystemCurrentDirectory()
    end
    if cfg.fsz == nil or tonumber(cfg.fsz) <= 0 then
        cfg.fsz = 14
    end
    if cfg.width == nil then
	cfg.width = 640
    end
    if cfg.height == nil then
	cfg.height = 480
    end

    if SWE.SystemMobileOs() ~= nil then
        win = SWE.DisplayInit("Lua SWE: Commander", false)
    else
        win = SWE.DisplayInit({ title="Lua SWE: Commander", width=cfg.width, height=cfg.height, resized=true })
    end

    if not win then
        print("SWE init error")
        os.exit(-1)
    end

    if frs == nil or frs.font == nil or frs.size ~= tonumber(cfg.fsz) then
	frs = SWE.FontRender("terminus.ttf", tonumber(cfg.fsz), SWE.Font.RenderSolid)
    end

    win.RenderWindow = function()
	win:RenderClear(SWE.Color.MidnightBlue)
	return true
    end

    win.SystemUserEvent = function(event, obj)
        if event == SWE.Action.FontChanged then
    	    frs = SWE.FontRender("terminus.ttf", tonumber(obj), SWE.Font.RenderSolid)
	    if win.cmd then
		win.cmd:SetVisible(false)
		-- font changed result
		win.cmd.result = 0x123456
		cfg.cwd = win.cmd.cwd
	    end
	    return true
	end
	return false
    end

    win.WindowCloseEvent = function()
	SaveCommanderConfig(win, frs)
    end

    win.TextureInvalidEvent = function()
	-- print("Invalid texture event!")
    end

    win.DisplayResizeEvent = function(w,h)
        win:SetSize(w, h)
        SWE.DisplayDirty()
    end

    SWE.Debug("-- run commander")
    win.cmd = CommanderInit(win, frs, cfg.cwd)

    -- fixed position
    if win.width ~= win.cmd.width or win.height ~= win.cmd.height then
	win.cmd:SetPosition((win.width-win.cmd.width)/2, (win.height-win.cmd.height)/2)
    end
    SWE.MainLoop(win.cmd)

    if not win.cmd.result then
	win:SetVisible(false)
        break
    elseif type(win.cmd.result) == "number" then
	-- font changed
	if win.cmd.result == 0x123456 then
	    SaveCommanderConfig(win, frs)
	end
    elseif type(win.cmd.result) == "string" then
	local start = win.cmd.result
	local backrun = SWE.runfile

	win.cmd:SetVisible(false)
	SWE.Scene.Remove(win.cmd)
	SaveCommanderConfig(win, frs)
	win.cmd = nil

        SWE.Debug("-- run script:", start)
	-- fixed running directory: scan resource
	SWE.runfile = start
        -- run script
        local res, err = pcall(dofile, start)

        -- show error dialog
        if not res then
	    ShowError(win, frs, err, start)
        end

	SWE.runfile = backrun
	frs = nil
    end
end
