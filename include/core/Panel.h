#pragma once

#include "DrawComponentManager.h"
#include "Utils.h"

namespace age
{
	class Panel
	{
	  public:
		Panel() = default;
		Panel(float x, float y, float w, float h) : rect{x, y, w, h} {};
		Rect rect;
		Color color;
	};

	class PanelScope
	{
		Panel* m_prevPanel = nullptr;
		DrawComponentManager* m_draw = nullptr;

	  public:
		PanelScope(DrawComponentManager* d, Panel* p) : m_draw(d)
		{
			m_prevPanel = m_draw->activePanel;
			m_draw->activePanel = p;
		}

		~PanelScope()
		{
			m_draw->activePanel = m_prevPanel;
		}
	};
} // namespace age
