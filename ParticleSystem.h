#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Vector2D.h"
#include "StaticMesh.h"
#include "Texture.h"
#include <string>
#include <vector>

class ParticleSystem
{
	irr::scene::IParticleSystemSceneNode* ps = nullptr;
public:
	ParticleSystem() {
	}

	void setEmitter(int i, sol::table params) {
		if (!ps)
			return;

		scene::IParticleEmitter* em = nullptr;

		irr::core::vector3df pos = irr::core::vector3df();

		if (params["position"]) {
			Vector3D p = static_cast<Vector3D>(params["position"]);
			pos = irr::core::vector3df(p.x, p.y, p.z);
		}

		switch (i) {
		default:
			// No special params needed
			em = ps->createPointEmitter();
			break;
		case 1:
			// Define aabb
			irr::core::vector3df minEdge = irr::core::vector3df(-3,-3,-3);
			irr::core::vector3df maxEdge = irr::core::vector3df(3,3,3);

			if (params["minEdge"]) {
				Vector3D p = static_cast<Vector3D>(params["minEdge"]);
				minEdge = irr::core::vector3df(p.x, p.y, p.z);
			}
			if (params["maxEdge"]) {
				Vector3D p = static_cast<Vector3D>(params["maxEdge"]);
				maxEdge = irr::core::vector3df(p.x, p.y, p.z);
			}

			em = ps->createBoxEmitter(irr::core::aabbox3df(minEdge + pos, maxEdge + pos));
			break;
		case 2:
			// Define radius
			float radius = 3.0f;
			if (params["radius"]) {
				radius = params["radius"];
			}

			em = ps->createSphereEmitter(pos, radius);
			break;
		case 3:
			// Define radius and ring thickness
			float radius = 3.0f;
			if (params["radius"]) {
				radius = params["radius"];
			}
			float thickness = 1.0f;
			if (params["thickness"]) {
				radius = params["thickness"];
			}

			em = ps->createRingEmitter(pos, radius, thickness);
			break;
		}

		if (params["direction"]) {
			Vector3D p = static_cast<Vector3D>(params["direction"]);
			em->setDirection(irr::core::vector3df(p.x, p.y, p.z));
		}



		em->setMaxAngleDegrees();

		em->setMaxLifeTime();

		em->setMaxParticlesPerSecond();

		em->setMaxStartColor();

		em->setMaxStartSize();

		em->setMinLifeTime();

		em->setMinParticlesPerSecond();

		em->setMinStartColor();

		em->setMinStartSize();

		// Handle generic parameters

		if (em) {
			ps->setEmitter(em);
			em->drop();
		}
	}

	void addAffector(int i, sol::table params) {

	}

	void setParent(const StaticMesh& parent) {

	}

	void destroy() {
		if (ps)
			ps->remove();
	}
};

inline void bindParticleSystem() {
	
}