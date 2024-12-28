#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "StaticMesh.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "LuaLime.h"
#include <string>
#include <vector>

irr::video::E_LIGHT_TYPE intToLightType(int i) {
	switch (i) {
	case 0: return irr::video::E_LIGHT_TYPE::ELT_POINT;
	case 1: return irr::video::E_LIGHT_TYPE::ELT_SPOT;
	case 2: return irr::video::E_LIGHT_TYPE::ELT_DIRECTIONAL;
	default: return irr::video::E_LIGHT_TYPE::ELT_POINT;
	}
}

class Light
{
public:
	irr::scene::ILightSceneNode* light;

	Light() {
		light = smgr->addLightSceneNode();
		light->grab();
	}
	Light(const Light& other) {
		light->drop();
		light = other.light;
	}

	~Light() {
		destroy();
	}

	void destroy() {
		light->remove();
		light = nullptr;
	}

	int getType() {
		if (light)
			return light->getLightType();
		return 0;
	}

	void setType(int type) {
		if (light)
			light->setLightType(intToLightType(type));
	}

	bool getCastShadows() {
		if (light)
			return light->getCastShadow();
		return false;
	}

	void setCastShadows(bool enable) {
		if (light)
			light->enableCastShadow(enable);
	}

	bool getVisible() {
		if (light)
			return light->isVisible();
		return false;
	}

	void setVisible(bool enable) {
		if (light)
			light->setVisible(enable);
	}

	float getRadius() {
		if (light)
			return light->getRadius();
		return 0.0f;
	}

	void setRadius(float radius) {
		if (light)
			light->setRadius(radius);
	}

	Vector3D getPosition() {
		if (light) {
			irr::core::vector3df pos = light->getPosition();
			return Vector3D(pos.X, pos.Y, pos.Z);
		}
		return Vector3D();
	}

	void setPosition(const Vector3D& pos) {
		if (light)
			light->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
	}

	Vector3D getRotation() {
		if (light) {
			irr::core::vector3df rot = light->getLightData().Direction;
			return Vector3D(rot.X, rot.Y, rot.Z);
		}
		return Vector3D();
	}

	void setRotation(const Vector3D& rot) {
		if (light)
			light->setRotation(irr::core::vector3df(rot.x, rot.y, rot.z));
	}

	Vector3D getDiffuseColor() {
		if (light)
			return Vector3D(light->getLightData().DiffuseColor.getRed(), light->getLightData().DiffuseColor.getGreen(), light->getLightData().DiffuseColor.getBlue());
		return Vector3D();
	}

	void setDiffuseColor(Vector3D& color) {
		if (light)
			light->getLightData().DiffuseColor = irr::video::SColor(255, color.x, color.y, color.z);
	}

	Vector3D getSpecularColor() {
		if (light)
			return Vector3D(light->getLightData().SpecularColor.getRed(), light->getLightData().SpecularColor.getGreen(), light->getLightData().SpecularColor.getBlue());
		return Vector3D();
	}

	void setSpecularColor(Vector3D& color) {
		if (light)
			light->getLightData().SpecularColor = irr::video::SColor(255, color.x, color.y, color.z);
	}

	float getInnerCone() {
		if (light)
			return light->getLightData().InnerCone;
		return 0.0f;
	}

	void setInnerCone(float deg) {
		if (light)
			light->getLightData().InnerCone = deg;
	}

	float getOuterCone() {
		if (light)
			return light->getLightData().OuterCone;
		return 0.0f;
	}

	void setOuterCone(float deg) {
		if (light)
			light->getLightData().OuterCone = deg;
	}

	float getFallOff() {
		if (light)
			return light->getLightData().Falloff;
		return 0.0f;
	}

	void setFallOff(float fallOff) {
		if (light)
			light->getLightData().Falloff = fallOff;
	}

	Vector3D getAttenuation() {
		if (light) {
			irr::core::vector3df atten = light->getLightData().Attenuation;
			return Vector3D(atten.X, atten.Y, atten.Z);
		}
		return Vector3D();
	}

	void setAttenuation(Vector3D atten) {
		if (light)
			light->getLightData().Attenuation = irr::core::vector3df(static_cast<u32>(atten.x), static_cast<u32>(atten.y), static_cast<u32>(atten.z));
	}

	StaticMesh* getParent() {
		if (light && light->getParent() && light->getParent()->getType() == irr::scene::ESNT_ANIMATED_MESH) {
			auto parentNode = static_cast<irr::scene::IAnimatedMeshSceneNode*>(light->getParent());
			return new StaticMesh{ parentNode }; // Might need to change to return ID?
		}
		return nullptr;
	}

	void setParent(StaticMesh* parent) {
		if (light && parent && parent->meshNode) {
			light->setParent(parent->meshNode);
		}
	}
};

void bindLight() {
	sol::usertype<Light> bind_type = lua->new_usertype<Light>("Light",
		sol::constructors<Light(), Light(const Light & other)>(),

		"type", sol::property(&Light::getType, &Light::setType),
		"position", sol::property(&Light::getPosition, &Light::setPosition),
		"rotation", sol::property(&Light::getRotation, &Light::setRotation),
		"shadows", sol::property(&Light::getCastShadows, &Light::setCastShadows),
		"visible", sol::property(&Light::getVisible, &Light::setVisible),
		"radius", sol::property(&Light::getRadius, &Light::setRadius),
		"diffuseColor", sol::property(&Light::getDiffuseColor, &Light::setDiffuseColor),
		"specularColor", sol::property(&Light::getSpecularColor, &Light::setSpecularColor),
		"innerConeAngle", sol::property(&Light::getInnerCone, &Light::setInnerCone),
		"outerConeAngle", sol::property(&Light::getOuterCone, &Light::setOuterCone),
		"fallOff", sol::property(&Light::getFallOff, &Light::setFallOff),
		"attenuation", sol::property(&Light::getAttenuation, &Light::setAttenuation),
		"parent", sol::property(&Light::getParent, &Light::setParent)
	);

	bind_type["destroy"] = &Light::destroy;
}