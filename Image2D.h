#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Vector2D.h"
#include "Texture.h"
#include <string>
#include <vector>

class Image2D {
public:
    irr::gui::IGUIImage* img = nullptr;
    irr::gui::IGUIButton* button = nullptr;
    bool clickable = false;
    sol::function onClick;

    Image2D();
    Image2D(const Texture& tex);
    Image2D(const Texture& tex, const Vector2D& pos);
    Image2D(const Texture& tex, const Vector2D& pos, const Vector2D& dimensions);
    Image2D(const Image2D& other);

    bool getClickable();
    void setHovered();
    bool getHovered();
    void setClickable(sol::function f);

    Vector3D getColor();
    void setColor(Vector3D& color);

    int getOpacity();
    void setOpacity(int o);

    bool getVisible();
    void setVisible(bool vis);

    Vector2D getPosition();
    void setPosition(const Vector2D& pos);

    Vector2D getSize();
    void setSize(const Vector2D& size);

    void setImage(const Texture& tex);
    void destroy();

    void bringToFront();
    void sendToBack();

    bool scalesToFit();
    void setScalesToFit(bool scale);

    void setBorderAlignment(int a, int b, int c, int d);
    void setMaxSize(const Vector2D& max);

    bool getEnabled();
    void setEnabled(bool enable);

    bool getUseAlpha();
    void setUseAlpha(bool enable);

    void setParent(const Image2D& other);

private:
    void updateButton();
};

void bindImage2D();