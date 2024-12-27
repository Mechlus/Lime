---@diagnostic disable: undefined-global
--[[
 _     _                
| |   (_)               
| |    _ _ __ ___   ___ 
| |   | | '_ ` _ \ / _ \
| |___| | | | | | |  __/
\_____/_|_| |_| |_|\___|

Scripting version 0.0

]]--

package.path = "./content/?.lua"
local irr = require("modules.irrlicht")

cam = null
box = null
dt = 0
mat = null
bron = null

Lime.SetShowConsole(true)
Lime.SetWriteConsole(true)
Lime.SetDriverType(irr.DRIVER_TYPE.EDT_DIRECT3D9)
--Lime.SetWindowSize(Vector2D.new(1024, 768))

-- Use . for Lime, etc. Use : on objects.
function Lime.OnStart()
    cam = Camera:new()
    cam:setActive()
    cam:useTarget(false)

    local tex = Texture:new("./content/Day.bmp")
    World.SetSkydome(tex)
    World.SetSkydomeParameters(16, 8, 0.9, 2.0, 100.0)

    World.SetFrameRate(120)
    cam.position = Vector3D.new(0, 4, -8)

    Input.CheckControllers()

    box = Mesh:new("./content/classd.x")

    tex = Texture:new("./content/base.jpg")
    mat = Material:new(tex)
    mat.diffuseColor = Vector3D.new(255,255,255)
    mat.lighting = false
    mat.mipmaps = false
    mat.textureWrap = 0
    mat.type = irr.MATERIAL_TYPE.EMT_SOLID
    mat.ID = 3
    box.ID = 5
    mat.fog = false
    box.collision = false
    Lime.Log(box:toStr())
    box:loadMaterial(mat, 0)

    Lime.Log(box:getFrameCount() .. " frames")

    local bronny = Material:new(mat)
    bronny.texture = Texture:new("./content/bron.png")
    bron = Billboard.new(bronny)
end

function Lime.OnEnd()
end

frame = 0
running = false
ogPos = Vector2D.new()
spinning = false
spinSpeed = 0
off = 0

function Lime.OnUpdate(v)
    dt = v

    local time = Lime.GetElapsedTime()
    local mem = World.GetMemoryUsage()
    local objs = World.GetObjectCount()
    local fps = World.GetFrameRate()
    --Lime.SetCaption("Lime - " .. math.floor(time/1000) .. "s elapsed, " .. objs .. " objects" .. ", " .. mem .. " MB, " .. fps .. " FPS")

    local joy = Input.GetControllerState()
    local out = Vector2D.new(joy.axis[1], joy.axis[2])
    local out1 = Vector2D.new(joy.axis[4], joy.axis[5])
    --Lime.SetCaption("Left Stick: " .. out:toStr() .. ", Right Stick: " .. out1:toStr())

    --box.rotation = Vector3D.new(0, box.rotation.y + dt * 1.0, 0)

    if running then
        frame = (frame + dt * 0.7) % 23
        if frame < 1 then
            frame = 1
        end
    else
        frame = 0
    end

    box.frame = frame

    Lime.SetCaption(box.frame .. tostring(running) .. ", " .. off)

    local mouse = Input.GetMouseState()
    if mouse["leftDown"] then
        if not spinning then
            ogPos = mouse["position"]
            spinning = true
        end

        if math.abs(mouse["position"].x - ogPos.x) > 1 then
            spinSpeed = spinSpeed - (mouse["position"].x - ogPos.x) * 0.2 * dt
        end
        ogPos = mouse["position"]
    else
        if spinning then
            spinning = false
        end
    end

    spinSpeed = clamp(spinSpeed, -10.0, 10.0)
    box.rotation = Vector3D.new(0, box.rotation.y + spinSpeed * 0.8 * dt, 0)

    spinSpeed = spinSpeed / ((1.0 + 0.1 * dt))

    off = off + 0.02 * dt
    --mat:setUVOffset(Vector2D.new(0, off * 0.1), 0)
    --mat:setTextureScale(Vector2D.new(off * 0.2, off * 0.2), 0)
    box:loadMaterial(mat, 0)
end

function clamp(x, y, z)
    if x >= y and x <= z then
        return x
    end
    if x < y then
        return y
    end
    if x > z then
        return z
    end
end

function Input.OnKeyPressed(c)
    running = not running
end