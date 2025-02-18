#pragma once
#include <irrlicht.h>
#include <sol/sol.hpp>
#include "LuaLime.h"
#include "Vector2D.h"

class ButtonCallbackPairClick {
public:
    ButtonCallbackPairClick(irr::gui::IGUIButton* b, sol::function f) : button(b), callback(f) {
    }

    irr::gui::IGUIButton* button;
    sol::function callback;
};

class ButtonCallbackPairHover {
public:
    ButtonCallbackPairHover(irr::gui::IGUIButton* b, sol::function f) : button(b), hover(f) {
    }

    irr::gui::IGUIButton* button;
    sol::function hover;
};

using namespace irr;

class LimeReceiver : public IEventReceiver
{
public:
    std::vector<ButtonCallbackPairClick> buttonCallbackClick;
    std::vector<ButtonCallbackPairHover> buttonCallbackHover;

    void removeImg(irr::gui::IGUIButton* b);

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

    LimeReceiver();

    virtual bool OnEvent(const SEvent& event) override;

    sol::table getMouseState() const;

    sol::table getControllerState() const;

    // Check if a key is currently pressed
    bool isKeyDown(irr::EKEY_CODE keyCode) const;
    irr::gui::IGUIButton* lastFocused = nullptr;
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