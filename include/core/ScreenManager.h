#pragma once

#include <memory>

#include "AudioComponentManager.h"
#include "DrawComponentManager.h"
#include "EmitterComponentManager.h"
#include "EntityComponentManager.h"
#include "PhysicsComponentManager.h"

namespace age
{
	struct CreateEntityResult
	{
		int eid;
		Entity* e;
	};

	class ScreenManager
	{
		std::unique_ptr<AudioComponentManager> m_audioManager;
		std::unique_ptr<PhysicsComponentManager> m_physicsManager;
		std::unique_ptr<DrawComponentManager> m_drawManager;
		std::unique_ptr<EntityComponentManager> m_entityManager;
		std::unique_ptr<EmitterComponentManager> m_emitterManager;

	  public:
		ScreenManager()
		{
			m_entityManager = std::make_unique<EntityComponentManager>();
			m_emitterManager = std::make_unique<EmitterComponentManager>();
		}

		static ScreenManager* Get()
		{
			static ScreenManager m_instance;
			return &m_instance;
		}

		CreateEntityResult CreateEntity()
		{
			CreateEntityResult result;
			result.eid = m_entityManager->CreateEntity();
			result.e = m_entityManager->Get(result.eid);
			return result;
		}

		void KillEntity(int eID)
		{
			Entity* e = m_entityManager->Get(eID);
			if (e && e->alive)
			{
				if (e->m_physicsID >= 0)
					m_physicsManager->Kill(e->m_physicsID);
				if (e->m_drawID >= 0)
					m_drawManager->Kill(e->m_drawID);
				if (e->m_physicsID >= 0)
					m_audioManager->Kill(e->m_audioID);
				m_entityManager->Kill(eID);
			}
		}

		Entity* GetEntity(int eID)
		{
			return m_entityManager->Get(eID);
		}

		void InitPhysicsComponents(Physics* p)
		{
			m_physicsManager = std::make_unique<PhysicsComponentManager>(p);
		}

		void InitDrawComponents(Renderer* r)
		{
			m_drawManager = std::make_unique<DrawComponentManager>(r);
		}

		void InitAudioComponents(Audio* a)
		{
			m_audioManager = std::make_unique<AudioComponentManager>(a);
		}

		EntityComponentManager* EntityManager()
		{
			return m_entityManager.get();
		}

		EmitterComponentManager* EmitterManager()
		{
			return m_emitterManager.get();
		}

		PhysicsComponentManager* PhysicsManager()
		{
			return m_physicsManager.get();
		}

		DrawComponentManager* DrawManager()
		{
			return m_drawManager.get();
		}

		AudioComponentManager* AudioManager()
		{
			return m_audioManager.get();
		}
	};

	inline void GenerateSphere(const SphereParams& params)
	{
		auto sm = ScreenManager::Get();

		auto result = sm->CreateEntity();

		PhysicsComponent::Params physicsParams;
		physicsParams.type = PhysicsComponent::PhysType::Sphere;
		physicsParams.eID = result.eid;
		physicsParams.xpos = params.pos.x;
		physicsParams.ypos = params.pos.y;
		physicsParams.rot = params.rot;
		physicsParams.width = params.radius;
		physicsParams.height = params.radius;
		physicsParams.angVel = params.angVel;
		physicsParams.mass = params.mass;
		physicsParams.linVel = params.linVel;

		result.e->m_physicsID = sm->PhysicsManager()->CreatePhysicsComponent(physicsParams);
		result.e->m_drawID = sm->DrawManager()->CreateSphereComponent(result.eid, params.texture);
	}

	inline void GenerateBox(const BoxParams& params)
	{
		auto sm = ScreenManager::Get();

		auto result = sm->CreateEntity();

		PhysicsComponent::Params physicsParams;
		physicsParams.type = PhysicsComponent::PhysType::Box;
		physicsParams.eID = result.eid;
		physicsParams.xpos = params.pos.x;
		physicsParams.ypos = params.pos.y;
		physicsParams.rot = params.rot;
		physicsParams.width = params.width;
		physicsParams.height = params.height;
		physicsParams.angVel = params.angVel;
		physicsParams.mass = params.mass;
		physicsParams.linVel = params.linVel;

		result.e->m_physicsID = sm->PhysicsManager()->CreatePhysicsComponent(physicsParams);
		result.e->m_drawID = sm->DrawManager()->CreateBoxComponent(result.eid, params.texture);
	}
} // namespace age
