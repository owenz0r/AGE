#pragma once

#include "Utils.h"
#include "globals.h"
#include <fstream>

namespace age
{
	class Physics;
	class Renderer;
	class Panel;

	class PhysicsComponent
	{
	  public:
		enum class PhysType
		{
			Box,
			Sphere
		};
		PhysType m_type;

		struct Params
		{
			int eID;
			PhysType type;
			float xpos;
			float ypos;
			float rot;
			float width;
			float height;
			Vec2f linVel = {0.0f, 0.0f};
			float angVel = 0.0f;
			float mass = 0.0f;
		};

		PhysicsComponent() {};

		void InitBox(const Params& params);
		void InitSphere(const Params& params);

		void Update(Physics* p, const double dt);
		void StepBack();
		void StepForward(Physics* p);
		void Rewind();
		void Forward();
		void Draw(Renderer* r);
		bool ContainsPoint(float x, float y);
		void CalculateAABB();
		Vec2f pos();
		void Serialize(std::ofstream& ofs);
		void Deserialize(std::ifstream& ifs);

		// engine params
		int eid = -1;
		bool alive = true;
		bool resolved = false;
		bool colliding = false;
		bool isStatic = false;
		bool isKinematic = false;
		bool rewind = false;

		// physics params
		Mat3f mat;
		float inertia;

		Vec2f linearVelocity = {0.0f, 0.0f};
		float angularVelocity = 1.0f;
		float invmass = 0;

		Rect aabb = {0.0f, 0.0f, 0.0f, 0.0f};
		float width = 0.0f;
		float height = 0.0f;
		float radians = 0.0f;
		float radius = 0.0f;

		// replay params
		constexpr static int PHYS_HISTORY_SIZE = 1024;
		constexpr static int INVALID_IDX = -9999;
		PhysInfo physHistory[PHYS_HISTORY_SIZE];

		int histIdx = 0;
		int maxRewind = INVALID_IDX;
		int maxForward = INVALID_IDX;
		bool steppedBack = false;
	};
} // namespace age
