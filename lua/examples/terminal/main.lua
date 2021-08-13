-- require 'SWE'
assert(SWE, "only for use with SWE")

SWE.RegisterResourceDirectory("data")
SWE.SetDebug(false)

local fname = "DejaVuSansMono.ttf"
local frs = SWE.FontRender(fname, 14, SWE.Font.RenderSolid)
local term = SWE.TerminalInit({ title="SWE Terminal", cols=80, rows=25, fontrender=frs, accel=true, resized=true })

-- save font render
term.frs = frs
frs = nil

term.RenderWindow = function()
    term:CursorPosition(0, 0):FillColors(SWE.Color.Silver, SWE.Color.Black, term.cols, term.rows)
    term:CursorPosition(0, 0):FillProperty(term.frs.blended, term.frs.style, term.frs.hinting, term.cols, term.rows)
    term:CursorPosition(0, 0):FillCharset(0x33, term.cols, term.rows)
    term:SetFGColor(SWE.Color.Yellow)
    term:CursorPosition(0, 0):DrawRect(term.cols, term.rows, SWE.Line.Thin)
    term:ResetFGColor()
    term:SetFlush()
    print("terminal rendered")
    return true
end

term.KeyPressEvent = function(key, mod, scancode)
    if key == SWE.Key.SPACE then
	SWE.DisplayDirty()
    end
    if key == SWE.Key.ESCAPE then
        term:SetVisible(false)
        return true
    end
    return false
end

term.ScrollUpEvent = function(posx,posy)
    local fsz = term.frs.size + 1
    local frs = SWE.FontRender(term.frs.font, fsz, term.frs.blended, term.frs.style, term.frs.hinting)
    term:SetFont(frs)
    term.frs = frs
    return true
end

term.ScrollDownEvent = function(posx,posy)
    local fsz = term.frs.size - 1
    local frs = SWE.FontRender(term.frs.font, fsz, term.frs.blended, term.frs.style, term.frs.hinting)
    term:SetFont(frs)
    term.frs = frs
    return true
end

term.WindowCloseEvent = function()
    print("close event")
end

-- term.TextureInvalidEvent = function()
-- -- if used members type(SWE.Texture), then it is necessary recreate it here
-- end

term.TerminalResizeEvent = function()
    print("terminal resized",term.cols,term.rows)
    -- change child object position
end

SWE.MainLoop(term)
