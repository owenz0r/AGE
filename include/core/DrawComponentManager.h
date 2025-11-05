#pragma once

#include "ComponentManager.h"
#include "DrawComponent.h"
#include "globals.h"

namespace age
{
	class Renderer;

	class DrawComponentManager
	{
		ComponentManager<DrawComponent> m_internal;
		Renderer* m_renderer;

	  public:
		Panel* activePanel = nullptr;

		DrawComponentManager(Renderer* r) : m_renderer(r) {};

		int CreateComponent(int eID);
		int CreateBoxComponent(int eID, const int textureId);
		int CreateBoxComponent(int eID, std::string_view texture);
		DrawComponent* GetComponent(int cid);
		int CreateSphereComponent(int eID, const int textureId);
		int CreateSphereComponent(int eID, std::string texture);
		int CreateTextComponent(int eID, const int textureId, const Point2Df pos);
		void UpdateTextComponent(const int cid, const int textureId);
		void Update();
		void Draw();
		void Kill(int eID);
		void Serialize(std::ofstream& ofs);
		void Deserialize(std::ifstream& ifs);
		void ClearComponents();
	};
} // namespace age
