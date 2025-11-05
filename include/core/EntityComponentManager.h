#pragma once

#include "ComponentManager.h"
#include "Entity.h"

namespace age
{
	class EntityComponentManager
	{
		ComponentManager<Entity> m_internal;

	  public:
		int CreateEntity();
		Entity* Get(int eID);
		void Kill(int eID);
		void Update(const double dt);
		void Serialize(std::ofstream& ofs);
		void Deserialize(std::ifstream& ifs);
		void ClearComponents();
	};
} // namespace age
