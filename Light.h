#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "StaticMesh.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "LuaLime.h"
#include <string>
#include <vector>

class Light
{
public:
	Light() {
	}

	~Light() {
		destroy();
	}

	void destroy() {
		
	}

};

void bindLight() {
	sol::usertype<Light> bind_type = lua->new_usertype<Light>("Light",
		sol::constructors<Light()>()
	);
}