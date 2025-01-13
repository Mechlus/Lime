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
	int index = 0;
	ISceneNode* target = 0;
	ISceneNode* holder = 0;

	void createBase() {
		target = smgr->addEmptySceneNode();
		holder = smgr->addEmptySceneNode();

		holder->addChild(target); // Rotate holder to change target position, but make sure to update target position with absolute position
		target->setPosition(irr::core::vector3df(0, -1, 0));
	}

	Light() : Light(Vector3D(0,0,0), Vector3D(0,0,0), Vector3D(255,255,255), Vector2D(0.1, 50.0), 90, false) {
	}
	Light(const Vector3D& pos, const Vector3D& rot, const Vector3D& col) 
		: Light(pos, rot, col, Vector2D(0.1, 50.0), 90, false) {}
	Light(const Vector3D& pos, const Vector3D& rot, const Vector3D& col, const Vector2D& viewPlanes, float fov, bool directional ) {
		createBase();

		SColorf c = SColorf(col.x / 255.0, col.y / 255.0, col.z/255.0, 1.0);
		SShadowLight s = SShadowLight(irrHandler->defaultShadowResolution, vector3df(pos.x, pos.y, pos.z), target->getAbsolutePosition(), 
						 c, viewPlanes.x, viewPlanes.y, fov * DEGTORAD, directional);
		effects->addShadowLight(s);
		index = effects->getShadowLightCount() - 1;
	}

	Vector3D getPosition() {
		
	}

	void setPosition(const Vector3D& pos) {

	}

	Vector3D getRotation() {

	}

	void setRotation(const Vector3D& rot) {

	}

	Vector3D getColor() {

	}

	void setColor(const Vector3D& col) {

	}

	bool getDirectional() {

	}

	void setDirectional(bool enable) {

	}

	Vector2D getViewPlanes() {

	}

	void setViewPlanes(const Vector2D& planes) {

	}

	float getFOV() {

	}

	void setFOV(float f) {

	}

	~Light() {
		destroy();
	}

	void destroy() {
		// Seemingly can't destroy, modify xeffects
	}

};

void bindLight() {
	sol::usertype<Light> bind_type = lua->new_usertype<Light>("Light",
		sol::constructors<Light()>()
	);
}