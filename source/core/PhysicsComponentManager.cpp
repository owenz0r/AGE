#include "core/PhysicsComponentManager.h"
#include "core/Renderer.h"

namespace age
{
	class Entity;

	PhysicsComponent* PhysicsComponentManager::GetComponent(int cid)
	{
		return m_internal.GetComponent(cid);
	}

	int PhysicsComponentManager::CreatePhysicsComponent(const PhysicsComponent::Params& params)
	{
		auto idx = m_internal.CreateComponent(params.eID);
		if (idx.isValid())
		{
			if (params.type == PhysicsComponent::PhysType::Box)
				m_internal.m_array[idx].InitBox(params);
			if (params.type == PhysicsComponent::PhysType::Sphere)
				m_internal.m_array[idx].InitSphere(params);
		}
		return idx;
	}

	void PhysicsComponentManager::Update(const double dt)
	{
		m_collisions.clear();
		m_collisionPairs.clear();

		if (!m_frozen)
			m_internal.Process([&](PhysicsComponent& c) { c.Update(m_physics, dt); });
		BroadPhase();
		NarrowPhase();

		if (m_freezeOnCollision && m_collisions.size() > 0)
		{
			m_frozen = true;
		}
		else
		{
			ResolveCollisions();
		}
	}

	void PhysicsComponentManager::Freeze()
	{
		m_frozen = true;
	}

	void PhysicsComponentManager::Unfreeze()
	{
		if (m_frozen)
		{
			m_collisions.clear();
			m_frozen = false;
		}
	}

	void PhysicsComponentManager::StepBack()
	{
		m_internal.Process([](PhysicsComponent& c) { c.StepBack(); });
	}

	void PhysicsComponentManager::StepForward()
	{
		m_internal.Process([&](PhysicsComponent& c) { c.StepForward(m_physics); });
	}

	void PhysicsComponentManager::Forward()
	{
		m_internal.Process([](PhysicsComponent& c) { c.Forward(); });
		Unfreeze();
	}

	void PhysicsComponentManager::Rewind()
	{
		m_internal.Process([](PhysicsComponent& c) { c.Rewind(); });
		Unfreeze();
	}

	void PhysicsComponentManager::ResolveCollisions()
	{
		auto m_array = m_internal.GetComponentArray();
		for (auto& c : m_collisions)
		{
			auto& comp = m_array[c.comp];
			if (comp.resolved)
			{
				// if (!m_physics->CollisionCheck(c.comp, c.other))
				if (!m_physics->CollisionCheck(m_array, c.comp, c.other))
					continue;
			}
			m_physics->ResolveCollision(m_array, c);
		}
	}

	void PhysicsComponentManager::BroadPhase()
	{
		auto m_array = m_internal.GetComponentArray();
		auto& m_num = m_internal.m_num;

		for (int i = 0; i < m_num; ++i)
		{
			if (m_array[i].alive)
			{
				for (int j = i + 1; j < m_num; ++j)
				{
					if (i != j && m_array[j].alive)
					{
						auto pair = CollisionPair(i, j);
						if (std::find(m_collisionPairs.begin(), m_collisionPairs.end(), pair) == m_collisionPairs.end())
						{
							if (m_physics->CollisionCheck(m_array, pair.A, pair.B))
								m_collisionPairs.push_back(pair);
						}
					}
				}
			}
		}
	}

	void PhysicsComponentManager::NarrowPhase()
	{
		auto m_array = m_internal.GetComponentArray();
		for (auto& pair : m_collisionPairs)
		{
			auto result = m_physics->NarrowPhase(m_array, pair.A, pair.B);
			if (result.colliding)
			{
				m_collisions.push_back(result.info);
				m_debugCollisions.push_back(result.info);
			}
		}
	}

	std::vector<int> PhysicsComponentManager::Select(float x, float y)
	{
		auto m_array = m_internal.GetComponentArray();
		auto& m_num = m_internal.m_num;

		std::vector<int> result;
		for (int i = 0; i < m_num; ++i)
		{
			if (m_array[i].alive)
			{
				if (m_array[i].ContainsPoint(x, y))
				{
					result.push_back(i);
				}
			}
		}

		if (result.size() == 1)
			displayIdx = result[0];

		return result;
	}

	void PhysicsComponentManager::Draw(Renderer* r)
	{
		auto m_array = m_internal.GetComponentArray();
		auto& m_num = m_internal.m_num;

		m_internal.Process([&](PhysicsComponent& c) { c.Draw(r); });

		if (displayIdx >= 0 && displayIdx < m_num)
		{
			auto v2fToString = [](float x, float y) {
				std::string xstring(16, '\0');
				auto written = std::snprintf(&xstring[0], xstring.size(), "%.4f", x);
				xstring.resize(written);

				std::string ystring(16, '\0');
				written = std::snprintf(&ystring[0], ystring.size(), "%.4f", y);
				ystring.resize(written);

				return xstring + ", " + ystring;
			};

			TextParams params;
			params.fontId = 2;
			params.text = v2fToString(m_array[displayIdx].linearVelocity.x, m_array[displayIdx].linearVelocity.y);
			params.pos = {0.5, 29.0};
			params.radians = 0.0f;
			params.color = Color::White();
			params.temp = true;

			r->DrawText(params);

			params.text = v2fToString(m_array[displayIdx].pos().x, m_array[displayIdx].pos().y);
			params.pos.y = 30.0;

			r->DrawText(params);
		}
	}

	void PhysicsComponentManager::DebugCollisions(Renderer* r)
	{
		if (r)
		{
			// r->DrawText(1, std::to_string(m_collisionCount), {1.0f, 1.0f}, 0,
			// Color::Green());
			for (auto& c : m_debugCollisions)
			{
				for (int i = 0; i < c.num_points; ++i)
				{
					Vec2f& point = c.closestPtWorld[i];
					auto p2 = point + c.normal;
					r->DrawLine(point.x, point.y, p2.x, p2.y, Color::Green());
				}
			}
		}
	}

	void PhysicsComponentManager::Kill(int eID)
	{
		m_internal.Kill(eID);
	}

	void PhysicsComponentManager::Serialize(std::ofstream& ofs)
	{
		ofs << "PhysicsComponentManager" << '\n';
		m_internal.Serialize(ofs);
	}

	void PhysicsComponentManager::Deserialize(std::ifstream& ifs)
	{
		std::string line;
		ifs >> line; // "PhysicsComponentManager"
		m_internal.Deserialize(ifs);
	}

	void PhysicsComponentManager::ClearComponents()
	{
		m_internal.Clear();
	}
} // namespace age
