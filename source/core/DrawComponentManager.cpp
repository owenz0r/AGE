#include "core/DrawComponentManager.h"
#include "core/Renderer.h"
#include "core/ResourceManager.h"
#include "core/globals.h"

namespace age
{
	int DrawComponentManager::CreateComponent(int eID)
	{
		return m_internal.CreateComponent(eID);
	}

	int DrawComponentManager::CreateBoxComponent(int eID, const int textureId)
	{
		auto idx = m_internal.CreateComponent(eID);
		if (idx.isValid())
		{
			m_internal.m_array[idx].SetTexture(textureId);
			m_internal.m_array[idx].InitBox(activePanel);
		}
		return idx;
	}

	int DrawComponentManager::CreateBoxComponent(int eID, std::string_view texture)
	{
		auto idx = m_internal.CreateComponent(eID);
		if (idx.isValid())
		{
			auto textureId = Global::ResourceManager->LoadTexture(texture);
			// auto textureId = m_renderer->LoadTexture(texture);
			if (textureId < 0)
				return -1;
			m_internal.m_array[idx].SetTexture(textureId);
			m_internal.m_array[idx].InitBox(activePanel);
		}
		return idx;
	}

	DrawComponent* DrawComponentManager::GetComponent(int cid)
	{
		return m_internal.GetComponent(cid);
	}

	int DrawComponentManager::CreateSphereComponent(int eID, const int textureId)
	{
		auto idx = m_internal.CreateComponent(eID);
		if (idx.isValid())
		{
			m_internal.m_array[idx].SetTexture(textureId);
			m_internal.m_array[idx].InitSphere(activePanel);
		}
		return idx;
	}

	int DrawComponentManager::CreateSphereComponent(int eID, std::string texture)
	{
		auto idx = m_internal.CreateComponent(eID);
		if (idx.isValid())
		{
			// auto textureId = m_renderer->LoadTexture(texture);
			auto textureId = Global::ResourceManager->LoadTexture(texture);
			if (textureId < 0)
				return -1;
			m_internal.m_array[idx].SetTexture(textureId);
			m_internal.m_array[idx].InitSphere(activePanel);
		}
		return idx;
	}

	int DrawComponentManager::CreateTextComponent(int eID, const int textureId, const Point2Df pos)
	{
		auto idx = m_internal.CreateComponent(eID);
		if (idx.isValid())
		{
			m_internal.m_array[idx].SetTexture(textureId);
			m_internal.m_array[idx].InitText(pos, activePanel);
		}
		return idx;
	}

	void DrawComponentManager::UpdateTextComponent(const int cid, const int textureId)
	{
		auto cp = GetComponent(cid);
		if (cp)
			cp->textureId = textureId;
	}

	void DrawComponentManager::Update()
	{
		m_internal.Process([](DrawComponent& c) { c.Update(); });
	}

	void DrawComponentManager::Draw()
	{
		m_internal.Process([&](DrawComponent& c) { c.Draw(m_renderer); });
	}

	void DrawComponentManager::Kill(int eID)
	{
		m_internal.Kill(eID);
	}

	void DrawComponentManager::Serialize(std::ofstream& ofs)
	{
		ofs << "DrawComponentManager" << '\n';
		m_internal.Serialize(ofs);
	}

	void DrawComponentManager::Deserialize(std::ifstream& ifs)
	{
		std::string line;
		ifs >> line; // "DrawComponentManager"
		m_internal.Deserialize(ifs);
	}

	void DrawComponentManager::ClearComponents()
	{
		m_internal.Clear();
	}
} // namespace age
