#pragma once

#include <fstream>

namespace age
{
	enum class EntityType
	{
		None,
		Ball,
		Paddle,
		Block,
		Wall,
		Killer,
		Emitter
	};

	class Entity
	{
	  public:
		bool alive = true;
		EntityType m_type = EntityType::None;
		int m_physicsID = -1;
		int m_drawID = -1;
		int m_audioID = -1;

		float m_rate = 1.0f;
		float m_accum = 0.0f;

		void Update(const double dt);
		void Serialize(std::ofstream& ofs);
		void Deserialize(std::ifstream& ifs);
	};
} // namespace age
