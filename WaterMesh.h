#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "StaticMesh.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Material.h"
#include <string>
#include <vector>

class Water
{
public:
	irr::scene::ISceneNode* water;
	float height;
	float speed;
	float length;
	irr::core::vector2df tileSize;
	irr::core::vector2df tileCount;
	irr::core::vector2df texRepeat;
	irr::video::SMaterial material;
	irr::scene::IMesh* rawMesh;

	Water(float h, float s, float l, const Vector2D& ts, const Vector2D& tc, const Vector2D& tr, const Material& m) {
		height = h; // 2
		speed = s; //300
		length = l; //30

		tileSize = irr::core::vector2df(ts.x, ts.y); // 20,20
		tileCount = irr::core::vector2df(tc.x, tc.y); // 40,40
		texRepeat = irr::core::vector2df(tr.x, tr.y); // 10,10

		material = m.mat;

		createRaw();

		refreshMesh();
	}

	Water() : Water(2, 300, 30, Vector2D(20,20), Vector2D(40,40), Vector2D(30,30), Material()) {}

	Water(const Material& m) : Water(2, 300, 30, Vector2D(20, 20), Vector2D(40, 40), Vector2D(10, 10), m) {}

	Water(const Vector2D& ts, const Vector2D& tc, const Vector2D& tr) : Water(2, 300, 30, ts, tc, tr, Material()) {}

	Water(sol::table tbl) {
		height = 2.0f;
		speed = 300.0f;
		length = 30.0f;

		tileSize = irr::core::vector2df(20, 20);
		tileCount = irr::core::vector2df(40, 40);
		texRepeat = irr::core::vector2df(10, 10);

		material = Material().mat;

		if (tbl["height"])
			height = tbl["height"];

		if (tbl["speed"])
			speed = tbl["speed"];

		if (tbl["length"])
			length = tbl["length"];

		if (tbl["tileSize"]) {
			tileSize.X = static_cast<Vector2D>(tbl["tileSize"]).x;
			tileSize.Y = static_cast<Vector2D>(tbl["tileSize"]).y;
		}

		if (tbl["tileCount"]) {
			tileCount.X = static_cast<Vector2D>(tbl["tileCount"]).x;
			tileCount.Y = static_cast<Vector2D>(tbl["tileCount"]).y;
		}

		if (tbl["textureRepeat"]) {
			texRepeat.X = static_cast<Vector2D>(tbl["textureRepeat"]).x;
			texRepeat.Y = static_cast<Vector2D>(tbl["textureRepeat"]).y;
		}

		if (tbl["material"]) {
			material = tbl["material"];
		}

		createRaw();

		refreshMesh();
	}

	~Water() {

	}

	void refreshMesh() {
		// Wave height, speed, and length
		irr::core::vector3df pos = water ? water->getPosition() : irr::core::vector3df();
		irr::core::vector3df rot = water ? water->getRotation() : irr::core::vector3df();
		irr::core::vector3df scale = water ? water->getScale() : irr::core::vector3df(1,1,1);

		if (water)
			water->remove();
		water = smgr->addWaterSurfaceSceneNode(rawMesh, height, speed, length, 0, 0, pos, rot, scale);
	}

	void createRaw() {
		rawMesh = smgr->addHillPlaneMesh("myHill",
			core::dimension2d<f32>(tileSize.X, tileSize.Y),
			core::dimension2d<u32>(tileCount.X, tileCount.X),
			&material,
			0, core::dimension2d<f32>(0, 0),
			core::dimension2d<f32>(texRepeat.X, texRepeat.Y))->getMesh(0);

		refreshMesh();
	}

	void destroy() {
		if (water)
			water->remove();
	}

	float getHeight() {
		if (water)
			return height;
		return 0.0f;
	}

	void setHeight(float i) {
		if (water) {
			height = i;
			refreshMesh();
		}
	}

	float getSpeed() {
		if (water)
			return speed;
		return 0.0f;
	}

	void setSpeed(float i) {
		if (water) {
			speed = i;
			refreshMesh();
		}
	}

	float getLength() {
		if (water)
			return length;
		return 0.0f;
	}

	void setLength(float i) {
		if (water) {
			length = i;
			refreshMesh();
		}
	}

	bool getVisibility() const {
		if (!water)
			return false;
		return water->isVisible();
	}

	void setVisibility(bool visible) {
		water->setVisible(visible);
	}

	Vector3D getPosition() {
		if (!water)
			return Vector3D();
		return Vector3D(water->getPosition().X, water->getPosition().Y, water->getPosition().Z);
	}

	void setPosition(const Vector3D& pos) {
		if (!water)
			return;
		water->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
	}

	Vector3D getRotation() {
		if (!water)
			return Vector3D();
		return Vector3D(water->getRotation().X, water->getRotation().Y, water->getRotation().Z);
	}

	void setRotation(const Vector3D& rot) {
		if (!water)
			return;
		water->setRotation(irr::core::vector3df(rot.x, rot.y, rot.z));
	}

	Vector3D getScale() {
		if (!water)
			return Vector3D();
		return Vector3D(water->getScale().X, water->getScale().Y, water->getScale().Z);
	}

	void setScale(const Vector3D& scale) {
		if (!water)
			return;
		water->setScale(irr::core::vector3df(scale.x, scale.y, scale.z));
	}

	void loadMaterial(const Material& m) {
		if (water) {
			material = m.mat;
			createRaw();
		}
	}

	Vector2D getTileSize() {
		if (water)
			return Vector2D(tileSize.X, tileSize.Y);
		return Vector2D();
	}

	void setTileSize(const Vector2D& other) {
		if (water)
			tileSize = irr::core::vector2df(other.x, other.y);
	}

	Vector2D getTileCount() {
		if (water)
			return Vector2D(tileCount.X, tileCount.Y);
		return Vector2D();
	}

	void setTileCount(const Vector2D& other) {
		if (water)
			tileCount = irr::core::vector2df(other.x, other.y);
	}

	Vector2D getTexRepeat() {
		if (water)
			return Vector2D(texRepeat.X, texRepeat.Y);
		return Vector2D();
	}

	void setTexRepeat(const Vector2D& other) {
		if (water)
			texRepeat = irr::core::vector2df(other.x, other.y);
	}

	void setParent(StaticMesh* parent) {
		if (water && parent && parent->meshNode) {
			water->setParent(parent->meshNode);
			return;
		}
		water->setParent(nullptr);
	}
};

void bindWater() {
	sol::usertype<Water> bind_type = lua->new_usertype<Water>("Water",
		sol::constructors <Water(), Water(const Vector2D & ts, const Vector2D & tc, const Vector2D & tr), Water(const Material& m), Water(sol::table tbl)>(),

		"position", sol::property(&Water::getPosition, &Water::setPosition),
		"rotation", sol::property(&Water::getRotation, &Water::setRotation),
		"scale", sol::property(&Water::getScale, &Water::setScale),
		"visible", sol::property(&Water::getVisibility, &Water::setVisibility),
		"height", sol::property(&Water::getHeight, &Water::setHeight),
		"speed", sol::property(&Water::getSpeed, &Water::setSpeed),
		"length", sol::property(&Water::getLength, &Water::setLength)
	);

	bind_type["destroy"] = &Water::destroy;
	bind_type["loadMaterial"] = &Water::loadMaterial;
	bind_type["setParent"] = &Water::setParent;
}