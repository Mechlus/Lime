#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "StaticMesh.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "LuaLime.h"
#include "DebugVisual.h"
#include <string>
#include <vector>

class Light
{
public:
	int index = 0;
	ISceneNode* target = 0;
	ISceneNode* holder = 0;
	DebugSceneNode* d = 0;

	void createBase() {
		target = smgr->addEmptySceneNode();
		holder = smgr->addEmptySceneNode();

		holder->addChild(target);
		target->setPosition(irr::core::vector3df(0, 0, 50));
	}

	Light() : Light(Vector3D(0,0,0), Vector3D(0,0,0), Vector3D(255,255,255), Vector2D(0.1, 50.0), 90, false) {
	}
	Light(const Vector3D& pos, const Vector3D& rot, const Vector3D& col) 
		: Light(pos, rot, col, Vector2D(0.1, 50.0), 90, false) {}
	Light(const Vector3D& pos, const Vector3D& rot, const Vector3D& col, const Vector2D& viewPlanes, float fov, bool directional ) {
		createBase();

		SColorf c = SColorf(col.x / 255.0, col.y / 255.0, col.z/255.0, 1.0);
		holder->setPosition(vector3df(pos.x, pos.y, pos.z));
		SShadowLight s = SShadowLight(irrHandler->defaultShadowResolution, holder->getPosition(), target->getAbsolutePosition(),
						 c, viewPlanes.x, viewPlanes.y, fov * DEGTORAD, directional);
		effects->addShadowLight(s);
		index = irrHandler->curLight;
		irrHandler->curLight += 1;

		updateTarget();
	}

	void updateTarget() {
		holder->updateAbsolutePosition();
		target->updateAbsolutePosition();
		effects->getShadowLight(index).setTarget(target->getAbsolutePosition());

		if (d) {
			d->remove();
			d = new DebugSceneNode(holder, smgr, 0, DebugType::LIGHT);
		}
	}

	Vector3D getPosition() {
		return true ? Vector3D(target->getPosition().X, target->getPosition().Y, target->getPosition().Z) : Vector3D();
	}

	void setPosition(const Vector3D& pos) {
		if (true) {
			holder->setPosition(vector3df(pos.x, pos.y, pos.z));
			effects->getShadowLight(index).setPosition(vector3df(pos.x, pos.y, pos.z));
			
			updateTarget();
		}
	}

	Vector3D getRotation() {
		return true ? Vector3D(holder->getRotation().X, holder->getRotation().Y, holder->getRotation().Z) : Vector3D();
	}

	void setRotation(const Vector3D& rot) {
		if (true) {
			holder->setRotation(vector3df(rot.x, rot.y, rot.z));
			updateTarget();
		}
	}

	Vector3D getColor() {
		if (true) {
			SColorf c = effects->getShadowLight(index).getLightColor();
			Vector3D(c.getRed(), c.getGreen(), c.getBlue());
		}
		return Vector3D();
	}

	void setColor(const Vector3D& col) {
		if (true)
			effects->getShadowLight(index).setLightColor(SColorf(col.x / 255.0, col.y / 255.0, col.z / 255.0, 1.0));
	}

	bool getDirectional() {
		return false;
	}

	void setDirectional(bool enable) {
		// remake light
	}

	Vector2D getViewPlanes() {
		return Vector2D();
	}

	void setViewPlanes(const Vector2D& planes) {

	}

	float getFOV() {
		return 0.0;
	}

	void setFOV(float f) {

	}

	~Light() {
		destroy();
	}

	void destroy() {
		// Seemingly can't destroy, modify xeffects
	}

	bool getDebug() {
		return (true && d);
	}

	void setDebug(bool visible) {
		if (true) {
			if (visible && !d)
				d = new DebugSceneNode(holder, smgr, 0, DebugType::LIGHT);
			else if (!visible && d)
				d->remove();
		}
	}

};

void bindLight() {
	sol::usertype<Light> bind_type = lua->new_usertype<Light>("Light",
		sol::constructors<Light(), Light(const Vector3D & pos, const Vector3D & rot, const Vector3D & col), Light(const Vector3D & pos, const Vector3D & rot, const Vector3D & col, const Vector2D & viewPlanes, float fov, bool directional)>(),

		"position", sol::property(&Light::getPosition, &Light::setPosition),
		"rotation", sol::property(&Light::getRotation, &Light::setRotation),
		"color", sol::property(&Light::getColor, &Light::setColor),
		"debug", sol::property(&Light::getDebug, &Light::setDebug)
	);
}