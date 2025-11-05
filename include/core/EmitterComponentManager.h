#pragma once

#include "ComponentManager.h"
#include "EmitterComponent.h"

namespace age
{
	class EmitterComponentManager
	{
		ComponentManager<EmitterComponent> m_internal;

	  public:
		int CreateEmitterComponent(const PhysicsComponent::Params& params);
		void Update(const double dt);
	};
} // namespace age
