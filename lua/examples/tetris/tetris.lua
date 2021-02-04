-- require 'SWE'

-- main window
local win = SWE.DisplayInit("Tetris", 240, 320, false)
if not win then
    print("SWE init error")
    os.exit(-1)
end

math.randomseed(os.time())

local config = { frs = nil, shapes = nil, tiles = nil, shape1 = nil, shape2 = nil,
		lines = 0, score = 0, background = SWE.Color.MidnightBlue,
		ox = 2, oy = 0, delay = 500, tick = 0, endgame = false, fastdown = false }

config.frs = SWE.FontRender.System()
config.shapes = SWE.JsonParse(SWE.BinaryBuf.ReadFromFile("tetris.json"):ToString())
config.shape1 = config.shapes[math.random(1, #config.shapes)]
config.shape2 = config.shapes[math.random(1, #config.shapes)]

-- init: tiles array 10x20
config.tiles = {}
for id = 1, 200 do
    local t = {}
    t.id = id
    t.color = config.background
    table.insert(config.tiles, t)
end

local function ShapeAllowShiftDown(shape, num)
    if num == 1 then
        return shape[13] == 0 and shape[14] == 0 and shape[15] == 0 and shape[16] == 0
    elseif num == 2 then
        return shape[9] == 0 and shape[10] == 0 and shape[11] == 0 and shape[12] == 0 and ShapeAllowShiftDown(shape, 1)
    elseif num == 3 then
        return shape[5] == 0 and shape[6] == 0 and shape[7] == 0 and shape[8] == 0 and ShapeAllowShiftDown(shape, 2)
    end
    return false
end

local function ShapeAllowShiftLeft(shape, num)
    if num == 1 then
        return shape[1] == 0 and shape[5] == 0 and shape[9] == 0 and shape[13] == 0
    elseif num == 2 then
        return shape[2] == 0 and shape[6] == 0 and shape[10] == 0 and shape[14] == 0 and ShapeAllowShiftLeft(shape, 1)
    elseif num == 3 then
        return shape[3] == 0 and shape[7] == 0 and shape[11] == 0 and shape[15] == 0 and ShapeAllowShiftLeft(shape, 2)
    end
    return false
end

local function ShapeAllowShiftRight(shape, num)
    if num == 1 then
        return shape[4] == 0 and shape[8] == 0 and shape[12] == 0 and shape[16] == 0
    elseif num == 2 then
        return shape[3] == 0 and shape[7] == 0 and shape[11] == 0 and shape[15] == 0 and ShapeAllowShiftRight(shape, 1)
    elseif num == 3 then
        return shape[2] == 0 and shape[6] == 0 and shape[10] == 0 and shape[14] == 0 and ShapeAllowShiftRight(shape, 2)
    end
    return false
end

local function ShapeAllowMoveDown(shape)
    for py = 0, 3 do
	for px = 0, 3 do
	    local tid1 = py * 4 + px + 1
            if shape[tid1] == 1 then
		-- bottom tid
		local tid2 = (config.oy + py + 1) * 10 + config.ox + px + 1
		-- check bottom tile
		if not config.tiles[tid2] then
		    return false
		end
		if (config.tiles[tid2].color ~= config.background) then
		    return false
		end
	    end
	end
    end

    if 16 > config.oy or ShapeAllowShiftDown(config.shape1.shape, config.oy - 15) then
    	return true
    end

    return false
end

local function ShapeAllowMoveRight(shape)
    for py = 0, 3 do
	for px = 0, 3 do
	    local tid1 = py * 4 + px + 1
            if shape[tid1] then
		-- right tid
		local tid2 = (config.oy + py) * 10 + config.ox + px + 2
		-- check right tile
		if not config.tiles[tid2] then
		    return false
		end
        	if config.tiles[tid2].color ~= config.background then
		    return false
		end
	    end
	end
    end

    if 6 > config.ox or ShapeAllowShiftRight(config.shape1.shape, config.ox - 5) then
	return true
    end

    return false
end

local function ShapeAllowMoveLeft(shape)
    for py = 0, 3 do
	for px = 0, 3 do
	    local tid1 = py * 4 + px + 1
            if shape[tid1] == 1 then
		-- left tid
	        local tid2 = (config.oy + py) * 10 + config.ox + px
		-- check left tile
		if not config.tiles[tid2] then
		    return false
		end
        	if config.tiles[tid2].color ~= config.background then
		    return false
		end
	    end
	end
    end

    if 0 < config.ox or ShapeAllowShiftLeft(config.shape1.shape, math.abs(config.ox) + 1) then
	return true
    end

    return false
end

local function ShapeRotateRight(shape)
    local tmp = shape[1]
    shape[1] = shape[13]
    shape[13] = shape[16]
    shape[16] = shape[4]
    shape[4] = tmp

    tmp = shape[2]
    shape[2] = shape[9]
    shape[9] = shape[15]
    shape[15] = shape[8]
    shape[8] = tmp

    tmp = shape[3]
    shape[3] = shape[5]
    shape[5] = shape[14]
    shape[14] = shape[12]
    shape[12] = tmp

    tmp = shape[6]
    shape[6] = shape[10]
    shape[10] = shape[11]
    shape[11] = shape[7]
    shape[7] = tmp
end

-- window virtual
function win.RenderWindow()
    win:RenderClear(SWE.Color.Silver)
    local px = 0
    local py = 0
    local si = 0

    -- render tiles
    for py = 0, 19 do
	for px = 0, 9 do
	    local tid = py * 10 + px + 1
	    win:RenderRect(config.tiles[tid].color, px * 16, py * 16, 16, 16, true)
	end
    end

    -- render current shape
    for py = 0, 3 do
        for px = 0, 3 do
    	    local tid = py * 4 + px + 1
    	    if config.shape1.shape[tid] == 1 then
            	local col = SWE.Color[config.shape1.color]
            	win:RenderRect(col, (config.ox + px) * 16, (config.oy + py) * 16, 16, 16, true)
    	    end
        end
    end

    -- render next shape
    for py = 0, 3 do
	for px = 0, 3 do
	    local tid = py * 4 + px + 1
            local col = SWE.Color.Black
            if config.shape2.shape[tid] == 1 then
                col = SWE.Color[config.shape2.color]
            end
            win:RenderRect(col, 168 + px * 16, 8 + py * 16, 16, 16, true)
        end
    end

    -- render labels
    win:RenderText(config.frs, "Score: " .. config.score, SWE.Color.Blue, 162, 88, SWE.Align.Left, SWE.Align.Top)
    win:RenderText(config.frs, "Lines: " .. config.lines, SWE.Color.Blue, 162, 108, SWE.Align.Left, SWE.Align.Top)

    if config.endgame then
	win:RenderText(config.frs, "<<End Game>>", SWE.Color.Red, win.width / 2, win.height / 2, SWE.Align.Center, SWE.Align.Center)
	win:RenderText(config.frs, "press any key for restart", SWE.Color.Yellow, win.width / 2, win.height / 2 + 20, SWE.Align.Center, SWE.Align.Center)
    end

    return true
end

function win.KeyPressEvent(key)
    if SWE.Key.ESCAPE == key then
        -- close scene, see SWE.Loop
        win:SetVisible(false)
        return true
    end

    if key == SWE.Key.LEFT and ShapeAllowMoveLeft(config.shape1.shape) then
        config.ox = config.ox - 1
        SWE.DisplayDirty()
        return true
    elseif key == SWE.Key.RIGHT and ShapeAllowMoveRight(config.shape1.shape) then
        config.ox = config.ox + 1
        SWE.DisplayDirty()
        return true
    elseif key == SWE.Key.UP then
	ShapeRotateRight(config.shape1.shape)
        SWE.DisplayDirty()
        return true
    elseif key == SWE.Key.DOWN then
	config.fastdown = true
        return true
    end

    -- restart game
    if config.endgame then
	config.fastdown = false
	config.shape1 = config.shape2
	config.shape2 = config.shapes[math.random(1, #config.shapes)]
	config.ox = 0
	config.oy = 0
	config.endgame = false
    end

    return false
end 
        
function win.SystemTickEvent(ms)
    if not config.endgame and (config.fastdown or 0 == config.tick or (ms - config.tick) > config.delay) then
	config.tick = ms
	-- allow move down
	if ShapeAllowMoveDown(config.shape1.shape) then
	    config.oy = config.oy + 1
	else
	    -- merge shape1
	    for py = 0, 3 do
    		for px = 0, 3 do
    		    local tid1 = py * 4 + px + 1
    		    if config.shape1.shape[tid1] == 1 then
    			local tid2 = (config.oy + py) * 10 + config.ox + px + 1
			config.tiles[tid2].color = SWE.Color[config.shape1.color]
		    end
    		end
	    end
	    config.score = config.score + 10
	    -- new part: reset values
	    config.fastdown = false
	    config.shape1 = config.shape2
	    config.shape2 = config.shapes[math.random(1, #config.shapes)]
	    config.ox = 0
	    config.oy = 0
	    config.endgame = false
	    -- check end game
	    if not ShapeAllowMoveDown(config.shape1.shape) then
		config.endgame = true
	    end
	end
        SWE.DisplayDirty()
    end
end
    
SWE.MainLoop(win)
