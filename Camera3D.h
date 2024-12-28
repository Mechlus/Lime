#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Material.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "LuaLime.h"
#include <string>
#include <vector>

class Camera3D {
public:
	irr::scene::ICameraSceneNode* camera;
	Vector3D target;

	Camera3D() {
		camera = smgr->addCameraSceneNode();
		camera->grab();
	}

	Camera3D(const Camera3D& other) {
		if (smgr->getActiveCamera() == camera) {
			smgr->setActiveCamera(other.camera);
		}
		destroy();
		camera = other.camera;
		camera->grab();
	}

	void destroy() {
		if (smgr->getActiveCamera() == camera) {
			smgr->setActiveCamera(nullptr);
		}
		camera->remove();
		camera = nullptr;
	}

	Vector3D getPosition() {
		return Vector3D(camera->getPosition().X, camera->getPosition().Y, camera->getPosition().Z);
	}

	void setPosition(const Vector3D& pos) {
		camera->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
	}

	Vector3D getRotation() {
		return Vector3D(camera->getRotation().X, camera->getRotation().Y, camera->getRotation().Z);
	}

	void setRotation(const Vector3D& rot) {
		camera->setRotation(irr::core::vector3df(rot.x, rot.y, rot.z));
	}

	void setTargetBind(bool val) {
		camera->bindTargetAndRotation(val);
	}

	float getNearPlane() {
		return camera->getNearValue();
	}

	void setNearPlane(float val) {
		camera->setNearValue(val);
	}

	float getFarPlane() {
		return camera->getFarValue();
	}

	void setFarPlane(float val) {
		camera->setFarValue(val);
	}

	Vector2D getPlanes() {
		return Vector2D(camera->getNearValue(), camera->getFarValue());
	}

	void setPlanes(const Vector2D& planes) {
		camera->setNearValue(planes.x);
		camera->setFarValue(planes.y);
	}

	float getFOV() {
		return camera->getFOV();
	}

	void setFOV(float val) {
		camera->setFOV(val);
	}

	bool getVisible() {
		return camera->isVisible();
	}

	void setVisible(bool val) {
		camera->setVisible(val);
	}

	Vector3D getForward() {
		irr::core::vector3df rotation = camera->getRotation();
		float pitch = rotation.X * irr::core::DEGTORAD;
		float yaw = rotation.Y * irr::core::DEGTORAD;

		float x = cos(pitch) * sin(yaw);
		float y = sin(pitch);
		float z = cos(pitch) * cos(yaw);

		return Vector3D(x, y, z);
	}

	Vector3D getUp() {
		irr::core::vector3df up = camera->getUpVector();
		return Vector3D(up.X, up.Y, up.Z);
	}

	void setUp(const Vector3D& up) {
		camera->setUpVector(irr::core::vector3df(up.x, up.y, up.z));
	}

	void setActive() {
		if (!camera->isVisible())
			return;
		smgr->setActiveCamera(camera);
	}

	void setTarget(const Vector3D& tar) {
		target = tar;
		camera->setTarget(irr::core::vector3df(target.x, target.y, target.z));
	}

	Vector3D getTarget() {
		return target;
	}

	bool getOrtho() {
		return camera->isOrthogonal();
	}

	void setOrtho(bool val) {
		// TO-DO
	}
};

void bindCamera3D() {
	sol::usertype<Camera3D> bind_type = lua->new_usertype<Camera3D>("Camera",
		sol::constructors<Camera3D(), Camera3D(const Camera3D & other)>(),
		"position", sol::property(&Camera3D::getPosition, &Camera3D::setPosition),
		"rotation", sol::property(&Camera3D::getRotation, &Camera3D::setRotation),
		"viewPlanes", sol::property(&Camera3D::getPlanes, &Camera3D::setPlanes),
		"fieldOfView", sol::property(&Camera3D::getFOV, &Camera3D::setFOV),
		"visible", sol::property(&Camera3D::getVisible, &Camera3D::setVisible),
		"target", sol::property(&Camera3D::getTarget, &Camera3D::setTarget)
	);

	bind_type["getForward"] = &Camera3D::getForward;
	bind_type["getUp"] = &Camera3D::getUp;
	bind_type["setUp"] = &Camera3D::setUp;
	bind_type["setActive"] = &Camera3D::setActive;
	bind_type["destroy"] = &Camera3D::destroy;
	bind_type["useTarget"] = &Camera3D::setTargetBind;

	/*
	* sol::meta_function::garbage_collect, sol::destructor([](Camera3D* self) {
			self->drop(); // Doesn't get called?
			}),
	*/
}
