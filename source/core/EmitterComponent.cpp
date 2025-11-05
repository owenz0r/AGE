#include "core/EmitterComponent.h"
#include "core/ScreenManager.h"

namespace age
{
	EmitterComponent::EmitterComponent()
	{
		sphereParams.pos = {10.0f, 10.0f};
		sphereParams.radius = 1.0f;
		sphereParams.mass = 1.0f;
		sphereParams.angVel = 0.0f;
		sphereParams.linVel = {5.0f, 5.0f};
		sphereParams.rot = 0.0f;
		sphereParams.texture = "ballBlue.png";

		boxParams.pos = {10.0f, 10.0f};
		boxParams.width = 1.0f;
		boxParams.height = 1.0f;
		boxParams.mass = 1.0f;
		boxParams.angVel = 0.0f;
		boxParams.linVel = {5.0f, 5.0f};
		boxParams.rot = 0.0f;
		boxParams.texture = "element_yellow_square.png";
	}

	void EmitterComponent::Update(const double dt)
	{
		accum += dt;
		if (accum >= rate)
		{
			switch (type)
			{
				case (PhysicsComponent::PhysType::Sphere):
					GenerateSphere(sphereParams);
					break;
				case (PhysicsComponent::PhysType::Box):
					GenerateBox(boxParams);
					break;
			}
			accum = 0.0f;
		}
	}
} // namespace age
