#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Image2D.h"
#include <string>

class Text2D {
private:
    irr::gui::IGUIStaticText* text;
    irr::gui::IGUIButton* button = nullptr;
    bool clickable = false;
    sol::function onClick;

public:
    Text2D();
    Text2D(std::string tx);
    Text2D(std::string tx, const Vector2D& pos);
    Text2D(std::string tx, const Vector2D& pos, const Vector2D& dimensions);
    Text2D(const Text2D& other);

    wchar_t* charToWchar(const char* str);

    std::string getText();
    void setText(std::string tx);

    bool getVisible();
    void setVisible(bool vis);

    bool getWrap();
    void setWrap(bool wrap);

    bool getDrawBorder();
    void setDrawBorder(bool enable);

    bool getDrawBackground();
    void setDrawBackground(bool draw);

    Vector3D getBackgroundColor();
    void setBackgroundColor(const Vector3D& col);

    int getBackgroundOpacity();
    void setBackgroundOpacity(int op);

    Vector3D getTextColor();
    void setTextColor(const Vector3D& col);

    int getTextOpacity();
    void setTextOpacity(int op);

    Vector2D getSize();
    void setSize(const Vector2D& size);

    Vector2D getPosition();
    void setPosition(const Vector2D& pos);

    void bringToFront();
    void sendToBack();

    void setBorderAlignment(int a, int b, int c, int d);
    void setTextAlignment(const Vector2D& align);
    void setMaxSize(const Vector2D& max);

    bool setFont(const std::string& fontName);
    void setParent(const Image2D& other);

    void destroy();
};

void bindText2D();