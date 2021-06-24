-- require 'SWE'
assert(SWE, "SDL Window Engine only")

local fullscreen = false

SWE.SetDebug(true)

local win = SWE.DisplayInit("LuaSWE example1", 640, 480, fullscreen)
local area = SWE.Window(100, 100, 100, 50, win)
area.colors = { SWE.Color.Blue, SWE.Color.Red, SWE.Color.Green }
area.bgcol = 1

local frs = SWE.FontRender("terminus.ttf", 14, SWE.Font.RenderSolid)
local poly = SWE.Polygon(320,240, 330,280, 370,290, 330,300, 320,340, 310,300, 270,290, 310,280, 320,240)
poly.focused = false

function win.RenderWindow()
    win:RenderClear(SWE.Color.Silver)
    win:RenderRect(SWE.Color.Red, 0, 0, win.width, win.height)
    return true
end

function win.KeyPressEvent(key)
    if SWE.Key.ESCAPE == key then
        -- close scene, see SWE.MainLoop
        win:SetVisible(false)
        return true
    end
    if SWE.SystemMobileOs() ~= nil then
        if key == SWE.Key.ANDROID_BACK or key == SWE.Key.WINCE_EXIT then
            win:SetVisible(false)
            return true
        end
    end
    return false
end

function area.MouseClickEvent(px, py, pbtn, rx, ry, rbtn)
    print("mouse click: ", px, py, pbtn, rx, ry, rbtn)
    area.bgcol = area.bgcol + 1
    if area.bgcol > #area.colors then
	area.bgcol = 1
    end
    SWE.DisplayDirty()
    return true
end

function area.MouseFocusEvent(f)
    print("focus: ", f)
end

function area.RenderWindow()
    area:RenderClear(area.colors[area.bgcol])
    area:RenderRect(SWE.Color.Yellow, 0, 0, win.width, win.height)
    area:RenderText(frs, "Hello World!", SWE.Color.White, area.width / 2, area.height / 2, SWE.Align.Center, SWE.Align.Center)
    return true
end

function poly.MouseFocusEvent(f)
    poly.focused = f
    SWE.DisplayDirty()
end

function poly.RenderWindow()
    local points1 = poly:GetAreaPoints()
    local fill = SWE.Color.Yellow
    local bound = SWE.Color.Navy
    if poly.focused then
	fill = SWE.Color.Navy
	bound = SWE.Color.Yellow
    end
    for i=1,#points1 do
	poly:RenderPoint(fill, points1[i].posx, points1[i].posy)
    end
    local points2 = poly:GetBoundaryPoints()
    for i=1,#points2 do
	poly:RenderPoint(bound, points2[i].posx, points2[i].posy)
    end
    return true
end

SWE.MainLoop(win)
