//
//  Entity.cpp
//  HoldIt
//
//  Created by Owen McNally on 2024-09-30.
//

#include "core/Entity.h"
#include "core/ScreenManager.h"

namespace age
{
	void Entity::Update(const double dt)
	{
		if (m_type == EntityType::Emitter)
		{
			m_accum += dt;
			if (m_accum > m_rate)
			{
				m_accum = 0.0f;
				auto e = ScreenManager::Get()->CreateEntity();
			}
		}
	}

	void Entity::Serialize(std::ofstream& ofs)
	{
		ofs << "EntityComponent" << '\n';

		int type = 0;
		switch (m_type)
		{
			case EntityType::None:
				type = 0;
				break;
			case EntityType::Ball:
				type = 1;
				break;
			case EntityType::Paddle:
				type = 2;
				break;
			case EntityType::Block:
				type = 3;
				break;
			case EntityType::Wall:
				type = 4;
				break;
			case EntityType::Killer:
				type = 5;
				break;
			case EntityType::Emitter:
				type = 6;
				break;
		}

		ofs << type << '\n';
		ofs << alive << '\n';
		ofs << m_physicsID << '\n';
		ofs << m_drawID << '\n';
		ofs << m_audioID << '\n';
		ofs << m_rate << '\n';
		ofs << m_accum << '\n';
		;
	}

	void Entity::Deserialize(std::ifstream& ifs)
	{
		std::string line;
		ifs >> line; //"EntityComponent";

		int type = 0;
		ifs >> type;
		switch (type)
		{
			case 0:
				m_type = EntityType::None;
				break;
			case 1:
				m_type = EntityType::Ball;
				break;
			case 2:
				m_type = EntityType::Paddle;
				break;
			case 3:
				m_type = EntityType::Block;
				break;
			case 4:
				m_type = EntityType::Wall;
				break;
			case 5:
				m_type = EntityType::Killer;
				break;
			case 6:
				m_type = EntityType::Emitter;
				break;
		}

		ifs >> alive;
		ifs >> m_physicsID;
		ifs >> m_drawID;
		ifs >> m_audioID;
		ifs >> m_rate;
		ifs >> m_accum;
	}
} // namespace age
