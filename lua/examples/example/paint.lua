-- require 'SWE'
assert(SWE, "SDL Window Engine only")

SWE.SetDebug(true)
local win = nil

if SWE.SystemMobileOs() ~= nil then
    win = SWE.DisplayInit("Lua SWE: Paint", false)
else
    win = SWE.DisplayInit("Lua SWE: Paint", 320, 480, false)
end

if not win then
    print("SWE init error")
    os.exit(-1)
end

win.color = SWE.Color.Black
win.area = SWE.Rect(1,21,win.width-2, win.height-22)
win.points = {}
win.palette = {
    {
        color = SWE.Color.Red,
        rect = SWE.Rect( 1, 1, 18, 18)
    },
    {
        color = SWE.Color.Orange,
        rect = SWE.Rect( 20, 1, 18, 18)
    },
    {
        color = SWE.Color.Yellow,
        rect = SWE.Rect( 40, 1, 18, 18)
    },
    {
        color = SWE.Color.Green,
        rect = SWE.Rect( 60, 1, 18, 18)
    },
    {
        color = SWE.Color.Aqua,
        rect = SWE.Rect( 80, 1, 18, 18)
    },
    {
        color = SWE.Color.Blue,
        rect = SWE.Rect( 100, 1, 18, 18)
    },
    {
        color = SWE.Color.Purple,
        rect = SWE.Rect( 120, 1, 18, 18)
    },
    {
        color = SWE.Color.White,
        rect = SWE.Rect( 140, 1, 18, 18)
    }
}

function win.KeyPressEvent(key)
    if SWE.Key.ESCAPE == key then
        -- close scene, see SWE.Loop
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

function win.MouseClickEvent(px, py, pb, rx, ry, rb)
    local pt = SWE.Point(px, py)
    for i=1,#win.palette do
        if win.palette[i].rect:PointInRect(pt) then
            win.color = win.palette[i].color
            SWE.DisplayDirty()
            return true
        end
    end

    if win.area:PointInRect(pt) then
        if win.color ~= SWE.Color.Black then
            win.points[pt] = win.color
        elseif win.points[pt] ~= nil then
            win.points[pt] = nil
        end
        SWE.DisplayDirty()
        return true
    end

    return false
end

function win.MouseMotionEvent(px,py,btn)
    local pt = SWE.Point(px, py)
    if 0 < btn and win.area:PointInRect(pt) then
        if win.color ~= SWE.Color.Black then
            win.points[pt] = win.color
        elseif win.points[pt] ~= nil then
            win.points[pt] = nil
        end
        SWE.DisplayDirty()
    end
end

function win.RenderWindow()
    win:RenderClear(SWE.Color.Black)

    for i=1,#win.palette do
        win:RenderRect(win.palette[i].color, win.palette[i].rect, true)
    end

    for k,v in pairs(win.points) do
        win:RenderPoint(v, k)
    end

    win:RenderRect(win.color, win.width - 19, 1, 18, 18, true)
    win:RenderRect(SWE.Color.Red, 0, 21, win.width, win.height)
    return true
end

SWE.MainLoop(win)
