#pragma once

#include "ComponentManager.h"
#include "Physics.h"
#include "PhysicsComponent.h"

namespace age
{
	class Entity;

	class PhysicsComponentManager
	{
		ComponentManager<PhysicsComponent> m_internal;

		Physics* m_physics;
		bool m_frozen = false;
		bool m_freezeOnCollision = false;

		std::vector<CollisionPair> m_collisionPairs;
		std::vector<CollisionInfo> m_collisions;
		std::vector<CollisionInfo> m_debugCollisions;

	  public:
		int displayIdx = 56;

		PhysicsComponentManager(Physics* p) : m_physics(p) {};
		PhysicsComponent* GetComponent(int cid);
		int CreatePhysicsComponent(const PhysicsComponent::Params& params);
		void Update(const double dt);
		void Freeze();
		void Unfreeze();
		void StepBack();
		void StepForward();
		void Forward();
		void Rewind();
		void ResolveCollisions();
		void BroadPhase();
		void NarrowPhase();
		std::vector<int> Select(float x, float y);
		void Draw(Renderer* r);
		void DebugCollisions(Renderer* r);
		void Kill(int eID);
		void Serialize(std::ofstream& ofs);
		void Deserialize(std::ifstream& ifs);
		void ClearComponents();
	};
} // namespace age
