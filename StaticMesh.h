#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Material.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "LuaLime.h"
#include <string>
#include <vector>

class StaticMesh
{
public:
	irr::scene::IAnimatedMeshSceneNode* meshNode;
	std::string meshPath = "";
	std::vector<irr::video::SMaterial> materials;
	irr::scene::ITriangleSelector* selector = 0;
	bool collisionEnabled = false;

	StaticMesh() : meshNode(nullptr) {}

	StaticMesh(const std::string& filePath) : meshPath(filePath), meshNode(nullptr) {
		loadMesh(meshPath);
	}

	StaticMesh(const StaticMesh& other) {
		meshNode = other.meshNode;
		materials = other.materials;
		meshPath = other.meshPath;

		if (collisionEnabled != other.collisionEnabled)
			setCollision(other.collisionEnabled);

		collisionEnabled = other.collisionEnabled;
	}

	StaticMesh(irr::scene::IAnimatedMeshSceneNode* node) : meshNode(node) {
		if (meshNode) {
			materials.clear();
			for (int i = 0; i < meshNode->getMaterialCount(); ++i) {
				materials.push_back(meshNode->getMaterial(i));
			}
		}
	}

	~StaticMesh() {
		deload();
	}

	std::string getMesh() const {
		return meshPath;
	}

	bool loadMesh(const std::string& filePath) {
		irr::scene::IAnimatedMesh* mesh = smgr->getMesh(filePath.c_str());
		if (!mesh) {
			std::string currentPath = std::filesystem::current_path().string();
			dConsole.sendMsg(currentPath.c_str(), 1);
			return false;
		}

		meshPath = filePath;
		meshNode = smgr->addAnimatedMeshSceneNode(mesh);
		if (!meshNode) return false;

		materials.clear();
		for (int i = 0; i < meshNode->getMaterialCount(); ++i) {
			materials.push_back(meshNode->getMaterial(i));
		}

		return true;
	}

	unsigned int getVertexCount() const {
		if (!meshNode) return 0;
		unsigned int vertexCount = 0;
		irr::scene::IAnimatedMesh* mesh = meshNode->getMesh();
		if (mesh) {
			for (unsigned int i = 0; i < mesh->getMeshBufferCount(); ++i) {
				vertexCount += mesh->getMeshBuffer(i)->getVertexCount();
			}
		}
		return vertexCount;
	}

	unsigned int getMaterialCount() const {
		return meshNode ? meshNode->getMaterialCount() : 0;
	}

	void deload() {
		if (meshNode) {
			meshNode->remove();
			meshNode = nullptr;
			meshPath.clear();
			materials.clear();
		}
	}

	bool loadMaterial(const Material& material, int slot) {
		if (!meshNode || slot < 0 || slot >= materials.size()) return false;
		materials[slot] = material.mat;
		meshNode->getMaterial(slot) = materials[slot];
		return true;
	}

	StaticMesh* getParent() {
		if (meshNode && meshNode->getParent() && meshNode->getParent()->getType() == irr::scene::ESNT_ANIMATED_MESH) {
			auto parentNode = static_cast<irr::scene::IAnimatedMeshSceneNode*>(meshNode->getParent());
			return new StaticMesh{ parentNode };
		}
		return nullptr;
	}

	void setParent(StaticMesh* parent) {
		if (meshNode && parent && parent->meshNode) {
			meshNode->setParent(parent->meshNode);
		}
	}

	bool getCollision() const {
		return meshNode ? collisionEnabled : false;
	}

	void setCollision(bool enable) {
		if (enable && meshNode) {
			selector = smgr->createTriangleSelector(meshNode);
			meshNode->setTriangleSelector(selector);
			selector->drop();
			collisionEnabled = true;
		} else if (meshNode) {
			meshNode->setTriangleSelector(nullptr);
			collisionEnabled = false;
		}
	}

	bool getVisibility() const {
		if (!meshNode)
			return false;
		return meshNode->isVisible();
	}

	void setVisibility(bool visible) {
		meshNode->setVisible(visible);
	}

	Vector3D getPosition() {
		if (!meshNode)
			return Vector3D();
		return Vector3D(meshNode->getPosition().X, meshNode->getPosition().Y, meshNode->getPosition().Z);
	}

	void setPosition(const Vector3D& pos) {
		if (!meshNode)
			return;
		meshNode->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
	}

	Vector3D getRotation() {
		if (!meshNode)
			return Vector3D();
		return Vector3D(meshNode->getRotation().X, meshNode->getRotation().Y, meshNode->getRotation().Z);
	}

	void setRotation(const Vector3D& rot) {
		if (!meshNode)
			return;
		meshNode->setRotation(irr::core::vector3df(rot.x, rot.y, rot.z));
	}

	Vector3D getScale() {
		if (!meshNode)
			return Vector3D();
		return Vector3D(meshNode->getScale().X, meshNode->getScale().Y, meshNode->getScale().Z);
	}

	void setScale(const Vector3D& scale) {
		if (!meshNode)
			return;
		meshNode->setScale(irr::core::vector3df(scale.x, scale.y, scale.z));
	}

	int getID() {
		if (meshNode)
			return meshNode->getID();
	}

	void setID(int i) {
		if (meshNode)
			meshNode->setID(i);
	}

	int getFrame() {
		if (meshNode)
			return meshNode->getFrameNr();
		return 0;
	}

	void setFrame(int i) {
		if (meshNode) {
			meshNode->setCurrentFrame(i);
		}
	}

	int getFrameCount() {
		if (meshNode)
			return meshNode->getEndFrame() - meshNode->getStartFrame() + 1;
		return 0;
	}

	sol::table getBoneData(irr::scene::IBoneSceneNode* bone) {
		sol::table boneInfo = lua->create_table();
		boneInfo["name"] = "";
		boneInfo["head"] = Vector3D(0, 0, 0);
		boneInfo["tail"] = Vector3D(0, 0, 0);
		boneInfo["rotation"] = Vector3D(0, 0, 0);

		if (bone) {
			Vector3D head = Vector3D(bone->getPosition().X, bone->getPosition().Y, bone->getPosition().Z);
			Vector3D tail = Vector3D(
				bone->getAbsolutePosition().X,
				bone->getAbsolutePosition().Y,
				bone->getAbsolutePosition().Z
			);
			boneInfo["name"] = bone->getName();
			boneInfo["rotation"] = Vector3D(bone->getRotation().X, bone->getRotation().Y, bone->getRotation().Z);
			boneInfo["headPosition"] = head;
			boneInfo["tailPosition"] = tail;
			boneInfo["length"] = sqrt(
				pow(tail.x - head.x, 2) +
				pow(tail.y - head.y, 2) +
				pow(tail.z - head.z, 2)
			); // Not performant! Potentially remove...
		}

		return boneInfo;
	}

	sol::table getBoneInfoByIndex(int i) {
		irr::scene::IBoneSceneNode* bone = nullptr;
		if (meshNode) {
			bone = meshNode->getJointNode(i);
		}
		return getBoneData(bone);
	}

	sol::table getBoneInfoByName(const std::string& name) {
		irr::scene::IBoneSceneNode* bone = nullptr;
		if (meshNode) {
			bone = meshNode->getJointNode(name.c_str());
		}
		return getBoneData(bone);
	}
};

void bindStaticMesh() {
	sol::usertype<StaticMesh> bind_type = lua->new_usertype<StaticMesh>("Mesh",
		sol::constructors<StaticMesh(), StaticMesh(const std::string & filePath), StaticMesh(const StaticMesh & other)>(),

		"collision", sol::property(&StaticMesh::getCollision, &StaticMesh::setCollision),
		"visible", sol::property(&StaticMesh::getVisibility, &StaticMesh::setVisibility),
		"position", sol::property(&StaticMesh::getPosition, &StaticMesh::setPosition),
		"rotation", sol::property(&StaticMesh::getRotation, &StaticMesh::setRotation),
		"scale", sol::property(&StaticMesh::getScale, &StaticMesh::setScale),
		"parent", sol::property(&StaticMesh::getParent, &StaticMesh::setParent),
		"ID", sol::property(&StaticMesh::getID, &StaticMesh::setID),
		"frame", sol::property(&StaticMesh::getFrame, &StaticMesh::setFrame)
	);

	bind_type["load"] = &StaticMesh::loadMesh;
	bind_type["loadMaterial"] = &StaticMesh::loadMaterial;
	bind_type["destroy"] = &StaticMesh::deload;
	bind_type["getVertexCount"] = &StaticMesh::getVertexCount;
	bind_type["getMaterialCount"] = &StaticMesh::getMaterialCount;
	bind_type["toStr"] = &StaticMesh::getMesh;
	bind_type["getBoneDataByIndex"] = &StaticMesh::getBoneInfoByIndex;
	bind_type["getBoneDataByName"] = &StaticMesh::getBoneInfoByName;
	bind_type["getFrameCount"] = &StaticMesh::getFrameCount;
}