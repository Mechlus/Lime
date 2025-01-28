#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Material.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "LuaLime.h"
#include <string>
#include <vector>
#include "DebugVisual.h"

using namespace irr;
using namespace video;

class Empty
{
public:
	irr::scene::ISceneNode* emp = nullptr;
	DebugSceneNode* d = 0;

	Empty() {
		emp = smgr->addEmptySceneNode();
	}

	Empty(const Vector3D& pos) : Empty() {
		emp->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
	}

	Empty(const Vector3D& pos, const Vector3D& rot) : Empty(pos) {
		emp->setRotation(irr::core::vector3df(rot.x, rot.y, rot.z));
	}

	Empty(const Vector3D& pos, const Vector3D& rot, const Vector3D& scale) : Empty(pos, rot) {
		emp->setScale(irr::core::vector3df(scale.x, scale.y, scale.z));
	}

	bool getVisibility() const {
		if (!emp)
			return false;
		return emp->isVisible();
	}

	void setVisibility(bool visible) {
		emp->setVisible(visible);
	}

	Vector3D getPosition() {
		if (!emp)
			return Vector3D();
		return Vector3D(emp->getPosition().X, emp->getPosition().Y, emp->getPosition().Z);
	}

	void setPosition(const Vector3D& pos) {
		if (!emp)
			return;
		emp->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
	}

	Vector3D getRotation() {
		if (!emp)
			return Vector3D();
		return Vector3D(emp->getRotation().X, emp->getRotation().Y, emp->getRotation().Z);
	}

	void setRotation(const Vector3D& rot) {
		if (!emp)
			return;
		emp->setRotation(irr::core::vector3df(rot.x, rot.y, rot.z));
	}

	Vector3D getScale() {
		if (!emp)
			return Vector3D();
		return Vector3D(emp->getScale().X, emp->getScale().Y, emp->getScale().Z);
	}

	void setScale(const Vector3D& scale) {
		if (!emp)
			return;
		emp->setScale(irr::core::vector3df(scale.x, scale.y, scale.z));
	}

	bool getDebug() {
		return emp ? d != nullptr : false;
	}

	void setDebug(bool visible) {
		if (emp) {
			if (visible && !d)
				d = new DebugSceneNode(emp, smgr, 0, DebugType::EMPTY);
			else
				d->remove();
		}
	}

	sol::table getBoundingBox() {
		sol::table result = lua->create_table();
		result["min"] = Vector3D();
		result["max"] = Vector3D();

		if (emp) {
			core::aabbox3d<f32> bb = emp->getTransformedBoundingBox();
			result["min"] = Vector3D(bb.MinEdge.X, bb.MinEdge.Y, bb.MinEdge.Z);
			result["max"] = Vector3D(bb.MaxEdge.X, bb.MaxEdge.Y, bb.MaxEdge.Z);
		}

		return result;
	}

	void destroy() {
		if (emp)
			emp->remove();
	}

	// Parent functions
	void setParentNil() {
		if (emp)
			emp->setParent(nullptr);
	}
	void setParentMesh(const StaticMesh& m) {
		if (emp && m.meshNode)
			emp->setParent(m.meshNode);
	}

	void setParentEmpty(const Empty& m) {
		if (emp && m.emp)
			emp->setParent(m.emp);
	}

	void setParentTrail(const Trail& m) {
		if (emp && m.t)
			emp->setParent(m.t);
	}

	void setParentBoard(const Billboard& m) {
		if (emp && m.bb)
			emp->setParent(m.bb);
	}

	void setParentWater(const Water& m) {
		if (emp && m.water)
			emp->setParent(m.water);
	}

	void setParentLight(const Light& m) {
		if (emp && m.holder) {
			emp->setParent(m.holder);
		}
	}

	void setParentCamera(const Camera3D& m) {
		if (emp && m.camera) {
			emp->setParent(m.camera);
		}
	}

	void setParentPPX(const ParticleSystem& m) {
		if (emp && m.ps) {
			emp->setParent(m.ps);
		}
	}
};

void bindEmpty() {
	sol::usertype<Empty> bind_type = lua->new_usertype<Empty>("Empty",
		sol::constructors<Empty(), Empty(const Vector3D & pos), Empty(const Vector3D & pos, const Vector3D & rot), Empty(const Vector3D & pos, const Vector3D & rot, const Vector3D & scale)>(),

		"visible", sol::property(&Empty::getVisibility, &Empty::setVisibility),
		"position", sol::property(&Empty::getPosition, &Empty::setPosition),
		"rotation", sol::property(&Empty::getRotation, &Empty::setRotation),
		"scale", sol::property(&Empty::getScale, &Empty::setScale),
		"debug", sol::property(&Empty::getDebug, &Empty::setDebug),

		"parent", sol::overload(&Empty::setParentPPX, &Empty::setParentCamera, &Empty::setParentMesh, &Empty::setParentEmpty, &Empty::setParentTrail, &Empty::setParentBoard, &Empty::setParentLight, &Empty::setParentNil, &Empty::setParentWater)
	);

	bind_type["destroy"] = &Empty::destroy;
	bind_type["getBoundingBox"] = &Empty::getBoundingBox;
}