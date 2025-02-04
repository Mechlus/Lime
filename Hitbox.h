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

    irr::scene::ISceneNode* getNode() const override { return node; }

    void construct(float radius, float height, int rings, int sectors);

protected:
    int teamID = 0;
};

void bindLegacyLight();