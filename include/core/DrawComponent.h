#pragma once

#include "Utils.h"
#include <string>

namespace age
{
	enum class DrawType
	{
		Null,
		Box,
		Sphere,
		Text,
		Num_Types
	};

	class Renderer;
	class PhysicsComponentManager;
	class Panel;

	class DrawComponent
	{
	  public:
		DrawComponent() {};
		DrawComponent(int eID) : eid(eID) {};

		void InitBox(Panel* p = nullptr);
		void InitSphere(Panel* p = nullptr);
		void InitText(const Point2Df pos, Panel* p = nullptr);
		void SetTexture(const int tId)
		{
			textureId = tId;
		}
		void Update();
		void Draw(Renderer* renderer) const;
		void Serialize(std::ofstream& ofs);
		void Deserialize(std::ifstream& ifs);

		DrawType m_type = DrawType::Null;
		int eid = -1;
		bool alive = true;
		int textureId = -1;
		int fontId = -1;

		Point2Df pos = {0.0f, 0.0f};

		float m_width = 0.0f;
		float m_height = 0.0f;
		float m_radius = 0.0f;
		float m_radians = 0.0f;

		Color m_color = {255, 255, 255, 255};

		Panel* m_panel = nullptr;
	};
} // namespace age
