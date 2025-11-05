#pragma once

#include "core/PhysicsComponent.h"

namespace age
{
	class EmitterComponent
	{
	  public:
		EmitterComponent();
		bool alive = true; // required
		float accum = 0.0f;
		float rate = 1.0f;
		PhysicsComponent::PhysType type = PhysicsComponent::PhysType::Box;
		SphereParams sphereParams;
		BoxParams boxParams;

		void Update(const double dt);
	};
} // namespace age
