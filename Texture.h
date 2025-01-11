#pragma once

#include <string>
#include "irrlicht.h"
#include "IrrManagers.h"

class Texture
{
public:
	irr::video::ITexture* texture;
	std::string path = "";

	Texture() : path("") {}
	Texture(const std::string imgpath) : path(imgpath) {
		if (!path.empty()) {
			load(imgpath);
		}
	}

	bool load(const std::string& imgpath) {
		if (!imgpath.empty()) {
			texture = driver->getTexture(imgpath.c_str());
			path = imgpath;
			return true;
		}
		return false;
	}

	std::string getPath() const {
		return path;
	}

	void keyColor(const Vector2D& pos) {
		if (texture)
			driver->makeColorKeyTexture(texture, core::position2d<s32>(pos.x, pos.y));
	}

	bool saveTexture(std::string path) {
		if (texture)
			return irrHandler->writeTextureToFile(texture, path.c_str());
		return false;
	}
};

void bindTexture() {
	sol::usertype<Texture> bind_type = lua->new_usertype<Texture>("Texture",
		sol::constructors<Texture(), Texture(std::string imgpath)>()
	);

	bind_type["load"] = &Texture::load;
	bind_type["toStr"] = &Texture::getPath;
	bind_type["keyColor"] = &Texture::keyColor;
	bind_type["save"] = &Texture::saveTexture;
}