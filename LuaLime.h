#pragma once
#include <filesystem>
#include <sol/sol.hpp>
#include "IrrHandling.h"

class LuaLime
{
private:
	int ensureStructure();
public:
	int initLua(irr::scene::ISceneManager* smgr, irr::video::IVideoDriver* driver);
};

inline sol::state* lua;