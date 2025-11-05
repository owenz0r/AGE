//
//  Screen.h
//  HoldIt
//
//  Created by Owen McNally on 2023-09-22.
//

#pragma once

#include <memory>

#include "Input.h"
#include "Panel.h"

namespace age
{
	class Renderer;
	class Engine;
	class Audio;
	class Physics;

	constexpr int MAX_PANELS = 8;

	class Screen
	{
	  protected:
		Panel m_panels[MAX_PANELS];
		int m_panelCount = 1;
		Renderer* m_renderer = nullptr;
		Engine* m_engine = nullptr;
		Audio* m_audio = nullptr;
		Physics* m_physics = nullptr;
		std::unique_ptr<Input> m_input;
		bool m_transition = false;
		std::string m_transitionData;

	  public:
		Screen() {};
		Screen(Physics* p, Audio* a, Renderer* r, Engine* e) : m_physics(p), m_audio(a), m_renderer(r), m_engine(e) {};
		virtual ~Screen() {};
		virtual void Setup(Physics* p, Audio* a, Renderer* r, Engine* e)
		{
			m_physics = p;
			m_audio = a;
			m_renderer = r;
			m_engine = e;
			m_initialized = true;
		};
		virtual void Init()
		{
		}
		virtual void Reset()
		{
		}
		virtual void Update(const double dt) {};
		virtual void Draw()
		{
		}

		virtual void SetTransitionData(std::string transition_data) {};
		virtual void ProcessInput()
		{
			if (m_input)
				m_input->ProcessInput();
		};
		virtual Panel* CreatePanel(float x, float y, float w, float h)
		{
			if (m_panelCount < MAX_PANELS - 1)
			{
				m_panels[m_panelCount].rect = {x, y, w, h};
				return &m_panels[m_panelCount++];
			}
			return nullptr;
		}
		bool m_initialized = false;
	};
} // namespace age
