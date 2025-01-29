#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Material.h"
#include "StaticMesh.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "LuaLime.h"

class Billboard {
public:
    irr::scene::IBillboardSceneNode* bb;
    irr::video::SMaterial myMaterial;

    Billboard();
    Billboard(const Billboard& other);
    Billboard(const Material& material);

    Vector3D getPosition();
    void setPosition(const Vector3D& pos);

    Vector2D getSize();
    void setSize(const Vector2D& size);

    bool loadMaterial(const Material& material);

    Vector3D getRotLock();
    void setRotLock(const Vector3D& rot);

    irr::f32 getYPivot();
    void setYPivot(float y);

    bool getVisible();
    void setVisible(bool visible);

    void setParent(StaticMesh* parent);

    void destroy();
};

void bindBillboard();