#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "StaticMesh.h"
#include "Vector2D.h"
#include "Texture.h"
#include <string>
#include <vector>

class Image2D
{
public:
	// Make custom Irrlicht scene node that uses the smgr to draw itself on render
	Image2D() {
		//driver->draw2DImage();
	}
};

void bindImage2D() {
	sol::usertype<Image2D> bind_type = lua->new_usertype<Image2D>("Image2D",
		sol::constructors <>()
	);
}