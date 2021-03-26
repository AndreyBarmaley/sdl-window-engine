-- require 'SWE'
assert(SWE, "SDL Window Engine only")

SWE.SetDebug(true)
local win = SWE.DisplayInit("Lua SWE: TestFPS(5000 Rects)", 800, 600, false)

local colors = {}
for col in pairs(SWE.Color) do
    table.insert(colors, col)
end

win.frs = SWE.FontRender.System()
win.fps = 0
win.rects = {}

function RandomColorRect()
    local t = {}
    local col = colors[math.random(#colors)]
    local posx = math.random(win.width-32)
    local posy = math.random(win.height-32)
    t.color = SWE.Color[col]
    t.rect = SWE.Rect(posx, posy, 32, 32)
    return t
end

for num = 1, 5000 do
    table.insert(win.rects, RandomColorRect())
end

function win.RenderWindow()
    win:RenderClear(SWE.Color.Navy)
    for num = 1, #win.rects do
        local t = win.rects[num]
        win:RenderRect(t.color, t.rect, true)
    end
    win:RenderText(win.frs, tostring(win.fps) .. " fps", SWE.Color.Black, win.width-20, win.height-20, SWE.Align.Right, SWE.Align.Bottom)
    -- print(win.fps, "fps")
    return true
end

function win.KeyPressEvent(key)
    if SWE.Key.ESCAPE == key then
        -- close scene, see SWE.MainLoop
        win:SetVisible(false)
        return true
    end

    if SWE.SystemMobileOs() ~= nil then
        -- android back
        -- wince exit
        if key == SWE.Key.ANDROID_BACK or key == SWE.Key.WINCE_EXIT then
            win:SetVisible(false)
            return true
        end
    end

    return false
end

function win.SystemTickEvent(time)
    local index = math.random(#win.rects)
    win.rects[index] = RandomColorRect()
    SWE.DisplayDirty()
end

function win.SystemRenderEvent(time)
    local integral, fractal = math.modf(1000 / time)
    win.fps = integral
end

-- toy color win
function CreateSimpleColorWindow(color, str, posx, posy, parent)
    local res = SWE.Window(posx, posy, 240, 196, parent)

    res.colbg = color
    res.colbr = SWE.Color.Silver
    res.rtbar = SWE.Rect(1, 1, res.width - 2, 16)
    res.area  = SWE.Rect(0, 0, res.width, res.height)
    res.ptmov = nil
    res.stbar = false
    res.focus = false
    res.text  = SWE.Texture.Text(parent.frs, str, SWE.Color.Black)
    res:SetMouseTracking(true)

    res.RenderWindow = function()
        res:RenderClear(res.colbg)
        res:RenderRect(res.colbr, res.rtbar, true)
        res:RenderTexture(res.text, (res.width - res.text.width) / 2, 2)
        if res.focus then
            res:RenderRect(SWE.Color.Black, res.area, false)
        end
        return true
    end

    res.MouseFocusEvent = function(focus)
        if focus then
            res.colbr = SWE.Color.DimGray
            SWE.Scene.MoveTop(res)
        else
            res.colbr = SWE.Color.Silver
        end
        res.focus = focus
        SWE.DisplayDirty()
    end

    res.MousePressEvent = function(x, y, btn)
        local pt = SWE.Point(x, y)
        if res.rtbar:PointInRect(pt) and btn == 1 then
            res.ptmov = pt
            res.stbar = true
            return true
        end
        return false
    end

    res.MouseReleaseEvent = function(x, y, btn)
        if res.stbar then
            res.stbar = false
            return true
        end
        return true
    end

    res.MouseMotionEvent = function(x, y, btn)
        if btn == SWE.MouseButton.Left and res.stbar then
            return true
        end
        return false
    end

    res.MouseTrackingEvent = function(x, y, btn)
        if btn == SWE.MouseButton.Left and res.stbar then
            local posx = x - res.ptmov.posx
            local posy = y - res.ptmov.posy
            res:SetPosition(posx, posy)
            return true
        end
        return false
    end

    return res
end

win.colorwin1 = CreateSimpleColorWindow(SWE.Color.DarkSeaGreen, "Color Window1", 100, 100, win)
win.colorwin2 = CreateSimpleColorWindow(SWE.Color.Plum, "Color Window2", 150, 150, win)

SWE.MainLoop(win)
