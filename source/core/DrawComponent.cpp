#include "core/DrawComponent.h"

#include "core/Entity.h"
#include "core/Panel.h"
#include "core/Renderer.h"
#include "core/ScreenManager.h"

namespace age
{
	void DrawComponent::InitBox(Panel* p)
	{
		m_type = DrawType::Box;
		m_panel = p;
	};

	void DrawComponent::InitSphere(Panel* p)
	{
		m_type = DrawType::Sphere;
		m_panel = p;
	}

	void DrawComponent::InitText(const Point2Df pp, Panel* p)
	{
		m_type = DrawType::Text;
		m_panel = p;

		pos = pp;
	}

	void DrawComponent::Update()
	{
		if (eid >= 0)
		{
			Entity* e = ScreenManager::Get()->GetEntity(eid);
			if (e->m_physicsID >= 0)
			{
				auto p = ScreenManager::Get()->PhysicsManager()->GetComponent(e->m_physicsID);
				if (p)
				{
					// pos = p->pos;
					pos = {p->mat.m[6], p->mat.m[7]};

					// m_radians = p->radians;
					m_radians = p->mat.radianRotation();

					if (m_type == DrawType::Box)
					{
						m_width = p->width;
						m_height = p->height;
					}
					else if (m_type == DrawType::Sphere)
					{
						m_radius = p->radius;
					}
				}
			}
		}
	}

	void DrawComponent::Draw(Renderer* renderer) const
	{
		float xpos = pos.x;
		float ypos = pos.y;

		//	float xpos = pos.x;
		//	float ypos = pos.y;

		if (m_panel)
		{
			xpos += m_panel->rect.x;
			ypos += m_panel->rect.y;
		}

		switch (m_type)
		{
			case DrawType::Box:
				renderer->DrawRectTexture(textureId, {xpos, ypos, m_width, m_height}, m_radians, m_color);
				break;

			case DrawType::Sphere:
				renderer->DrawRectTexture(textureId, {xpos, ypos, m_radius * 2, m_radius * 2}, m_radians, m_color);
				break;

			case DrawType::Text:
				renderer->DrawText(textureId, {xpos, ypos});
			default:
				break;
		}
	}

	void DrawComponent::Serialize(std::ofstream& ofs)
	{
		ofs << "DrawComponent" << '\n';
		int type = 0;
		switch (m_type)
		{
			case DrawType::Null:
				type = 0;
				break;
			case DrawType::Box:
				type = 1;
				break;
			case DrawType::Sphere:
				type = 2;
				break;
			case DrawType::Text:
				type = 3;
				break;
		}
		ofs << type << '\n';
		ofs << eid << '\n';
		ofs << alive << '\n';
		ofs << textureId << '\n';
		ofs << fontId << '\n';

		ofs << pos << '\n';

		ofs << m_width << '\n';
		ofs << m_height << '\n';
		ofs << m_radius << '\n';
		ofs << m_radians << '\n';

		ofs << m_color << '\n';
	}

	void DrawComponent::Deserialize(std::ifstream& ifs)
	{
		std::string line;
		ifs >> line; // DrawComponent

		int type = 0;
		ifs >> type;
		switch (type)
		{
			case 0:
				m_type = DrawType::Null;
				break;
			case 1:
				m_type = DrawType::Box;
				break;
			case 2:
				m_type = DrawType::Sphere;
				break;
			case 3:
				m_type = DrawType::Text;
				break;
		}

		ifs >> eid;
		ifs >> alive;
		ifs >> textureId;
		ifs >> fontId;
		ifs >> pos;
		ifs >> m_width;
		ifs >> m_height;
		ifs >> m_radius;
		ifs >> m_radians;
		ifs >> m_color;
	}
} // namespace age
