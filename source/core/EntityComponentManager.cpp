#include "core/EntityComponentManager.h"

namespace age
{
	int EntityComponentManager::CreateEntity()
	{
		return m_internal.CreateComponent();
	}

	Entity* EntityComponentManager::Get(int eID)
	{
		return m_internal.GetComponent(eID);
	}

	void EntityComponentManager::Kill(int eID)
	{
		m_internal.Kill(eID);
	}

	void EntityComponentManager::Update(const double dt)
	{
		m_internal.Process([dt](Entity& c) { c.Update(dt); });
	}

	void EntityComponentManager::Serialize(std::ofstream& ofs)
	{
		ofs << "EntityComponentManager" << '\n';
		m_internal.Serialize(ofs);
	}

	void EntityComponentManager::Deserialize(std::ifstream& ifs)
	{
		std::string line;
		ifs >> line; // "EntityComponentManager"
		m_internal.Deserialize(ifs);
	}

	void EntityComponentManager::ClearComponents()
	{
		m_internal.Clear();
	}
} // namespace age
