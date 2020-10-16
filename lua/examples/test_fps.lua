-- require 'SWE'

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
    local posx = math.random(800-32)
    local posy = math.random(600-32)
    t.color = SWE.Color[col]
    t.rect = SWE.Rect(posx, posy, 32, 32)
    return t
end

for num = 1, 5000 do
    table.insert(win.rects, RandomColorRect())
end

function ToInt(x)
    local integral, fractal = math.modf(x)
    return integral
end


function win.RenderWindow()
    win:RenderClear(SWE.Color.Navy)
    for num = 1, #win.rects do
        local t = win.rects[num]
        win:RenderRect(t.color, t.rect, true)
    end
    win:RenderText(win.frs, tostring(win.fps) .. " fps", SWE.Color.Black, 780, 580, SWE.Align.Right, SWE.Align.Bottom)
    print(win.fps, "fps")
    return true
end

function win.KeyPressEvent(key)
    if SWE.Key.ESCAPE == key then
        -- close scene, see SWE.MainLoop
        win:SetVisible(false)
        return true
    end
    return false
end

function win.SystemTickEvent(time)
    local index = math.random(#win.rects)
    win.rects[index] = RandomColorRect()
    SWE.DisplayDirty()
end

function win.SystemRenderEvent(time)
    win.fps = ToInt(1000 / time)
end

SWE.MainLoop(win)
