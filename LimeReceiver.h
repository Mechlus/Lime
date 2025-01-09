#pragma once
#include <irrlicht.h>
#include <sol/sol.hpp>
#include "LuaLime.h"
#include "Vector2D.h"

using namespace irr;

class LimeReceiver : public IEventReceiver
{
public:
    struct SMouseState
    {
        core::position2di Position;
        bool LeftButtonDown;
        bool RightButtonDown;
        bool MiddleButtonDown;
        float WheelDelta;

        SMouseState()
            : LeftButtonDown(false), RightButtonDown(false), MiddleButtonDown(false), WheelDelta(0.0f)
        { }
    } MouseState;

    struct SControllerState
    {
        f32 Axis[SEvent::SJoystickEvent::NUMBER_OF_AXES];
        u32 Buttons;

        SControllerState()
        {
            std::fill(std::begin(Axis), std::end(Axis), 0.0f);
            Buttons = 0;
        }

        bool isButtonPressed(u32 buttonIndex) const
        {
            return Buttons & (1 << buttonIndex);
        }
    } ControllerState;

    LimeReceiver()
    {
        keys.fill(false);
        keysRepeat.fill(false);
    }

    virtual bool OnEvent(const SEvent& event) override
    {
        // Handle keyboard input events
        if (event.EventType == EET_KEY_INPUT_EVENT)
        {
            keys[event.KeyInput.Key] = event.KeyInput.PressedDown;

            if (event.KeyInput.PressedDown) {
                if (!keysRepeat[event.KeyInput.Key]) {
                    callLuaFunction("Input", "OnKeyPressed", static_cast<irr::EKEY_CODE>(event.KeyInput.Key));
                    keysRepeat[event.KeyInput.Key] = true;
                }
            }
            else {
                callLuaFunction("Input", "OnKeyReleased", static_cast<irr::EKEY_CODE>(event.KeyInput.Key));
                keysRepeat[event.KeyInput.Key] = false;
            }
        }

        // Handle mouse input events
        if (event.EventType == EET_MOUSE_INPUT_EVENT)
        {
            switch (event.MouseInput.Event)
            {
            case EMIE_LMOUSE_PRESSED_DOWN:
                MouseState.LeftButtonDown = true;
                callLuaFunction("Input", "OnLeftMouseClick");
                break;

            case EMIE_LMOUSE_LEFT_UP:
                MouseState.LeftButtonDown = false;
                break;

            case EMIE_RMOUSE_PRESSED_DOWN:
                MouseState.RightButtonDown = true;
                callLuaFunction("Input", "OnRightMouseClick");
                break;

            case EMIE_RMOUSE_LEFT_UP:
                MouseState.RightButtonDown = false;
                break;

            case EMIE_MMOUSE_PRESSED_DOWN:
                MouseState.MiddleButtonDown = true;
                callLuaFunction("Input", "OnMiddleMouseClick");
                break;

            case EMIE_MMOUSE_LEFT_UP:
                MouseState.MiddleButtonDown = false;
                break;

            case EMIE_MOUSE_MOVED:
                MouseState.Position.X = event.MouseInput.X;
                MouseState.Position.Y = event.MouseInput.Y;
                callLuaFunction("Input", "OnMouseMove", Vector2D(MouseState.Position.X, MouseState.Position.Y));
                break;

            case EMIE_MOUSE_WHEEL:
                MouseState.WheelDelta = event.MouseInput.Wheel;
                callLuaFunction("Input", "OnMouseScroll", MouseState.WheelDelta);
                break;

            default:
                break;
            }
        }

        if (event.EventType == EET_JOYSTICK_INPUT_EVENT)
        {
            JoystickState = event.JoystickEvent;

            for (u32 i = 0; i < SEvent::SJoystickEvent::NUMBER_OF_AXES; ++i)
            {
                ControllerState.Axis[i] = JoystickState.Axis[i];
            }

            ControllerState.Buttons = JoystickState.ButtonStates;
        }

        return false;
    }

    sol::table getMouseState() const
    {
        sol::table table = lua->create_table();
        table["position"] = Vector2D(MouseState.Position.X, MouseState.Position.Y);
        table["leftDown"] = MouseState.LeftButtonDown;
        table["rightDown"] = MouseState.RightButtonDown;
        table["middleDown"] = MouseState.MiddleButtonDown;
        table["wheelDelta"] = MouseState.WheelDelta;
        return table;
    }

    sol::table getControllerState() const
    {
        sol::table table = lua->create_table();

        sol::table axisTable = lua->create_table();
        for (int i = 0; i < SEvent::SJoystickEvent::NUMBER_OF_AXES; i++) {
            axisTable[i + 1] = ControllerState.Axis[i] / 32767.f;
        }

        sol::table buttonTable = lua->create_table();
        for (int i = 0; i < 32; ++i) {
            buttonTable[i + 1] = ControllerState.isButtonPressed(i);
        }

        table["axis"] = axisTable;
        table["buttons"] = buttonTable;
        return table;
    }

    // Check if a key is currently pressed
    bool isKeyDown(irr::EKEY_CODE keyCode) const
    {
        return keys[keyCode];
    }

private:
    std::array<bool, KEY_KEY_CODES_COUNT> keys;
    std::array<bool, KEY_KEY_CODES_COUNT> keysRepeat;
    SEvent::SJoystickEvent JoystickState;

    template<typename... Args>
    void callLuaFunction(const std::string& tableName, const std::string& functionName, Args&&... args)
    {
        sol::optional<sol::function> func = (*lua)[tableName][functionName];
        if (func && (*lua)[tableName][functionName].get_type() == sol::type::function)
        {
            try {
                func.value()(std::forward<Args>(args)...);
            }
            catch (const sol::error& e) {
                std::cerr << "Error calling " << tableName << "." << functionName << ": " << e.what() << std::endl;
            }
        }
    }
};