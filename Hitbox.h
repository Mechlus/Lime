#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "LuaLime.h"
#include "DebugVisual.h"
#include <string>
#include <vector>

#include "Compatible3D.h"

class Hitbox : public Compatible3D {
public:
    irr::scene::IMeshSceneNode* node = 0;
    irr::scene::ITriangleSelector* col = 0; // Does not use collision by default, but can be if the user requests

    float radius;
    float height;
    int lod = 1; // 0 - Low poly, 1 - Medium, 2 - Highest (affects rings and sectors only, does not affect collision performance with other hitboxes)

    irr::scene::ISceneNode* getNode() const override { return node; }

    Vector3D getPosition();
    void setPosition(const Vector3D& pos);
    Vector3D getRotation();
    void setRotation(const Vector3D& rot);
    bool getActive();
    void setActive(bool active); // Active hitbox turns the hitbox yellow, otherwise blue (but if it is not visible then the capsule will not be rendered at all)
    bool getVisible();
    void setVisible(bool visible); // Sets vertex opacity to 255; hitboxes will always be truly visible
    int getLOD();
    void setLOD(int i); // Calls construct if LOD is different than lod member variable
    bool getCollision();
    void setCollision(bool enable); // If enabled, will be interactable with raypicks etc.
    int getID();
    void setID(int id);

    void construct(float radius, float height, int rings, int sectors);
    bool overlaps(const Hitbox& other); // Starts with aabb check, then two spheric checks, then a cylindrical check
    void destroy();
};

void bindHitbox();