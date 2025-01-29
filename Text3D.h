#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "StaticMesh.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include <string>

#include "Compatible3D.h"

class Text3D : public Compatible3D {
public:
    irr::scene::ITextSceneNode* text;
    irr::video::SColor color;
    int opacity;
    std::string myText;

    Text3D();
    Text3D(const std::string& tx);
    Text3D(const std::string& tx, const Vector3D& pos);
    Text3D(const std::string& tx, const Vector3D& pos, const Vector3D& col, int op);
    Text3D(const std::string& tx, const std::string& fontName);
    Text3D(const std::string& tx, const Vector3D& pos, const Vector3D& col, int op, const std::string& fontName);

    std::string getText();
    void setText(const std::string& tx);

    bool getVisibility() const;
    void setVisibility(bool visible);

    Vector3D getPosition();
    void setPosition(const Vector3D& pos);

    Vector3D getColor();
    void setColor(const Vector3D& col);

    int getOpacity();
    void setOpacity(int op);

    void updateColor();

    bool setFont(const std::string& fontName);

    void destroy();

    irr::scene::ISceneNode* getNode() const override { return text; }
};

void bindText3D();