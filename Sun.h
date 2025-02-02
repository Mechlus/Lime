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

class Sun : public Compatible3D {
public:
    irr::scene::ILightSceneNode* light = 0;
    DebugSceneNode* d;

    irr::scene::ISceneNode* getNode() const override { return light; }
};

void bindSun();