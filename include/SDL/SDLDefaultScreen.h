#pragma once

#include "SDL/SDLInput.h"
#include "core/Renderer.h"
#include "core/ResourceManager.h"
#include "core/Screen.h"

#include <iostream>

namespace age
{
	class SDLDefaultScreen : public Screen
	{
		int m_fontId = -1;

	  public:
		SDLDefaultScreen(Renderer* r, Engine* e) : Screen(nullptr, nullptr, r, e)
		{
		}

		void Init() override
		{
			m_input = std::make_unique<SDLInput>();
			m_input->m_keydownmap.insert({' ', [&] { m_engine->Transition(0, ""); }});
			m_input->SetQuitCallback([this]() { m_engine->Quit(); });

			// m_fontId = Global::ResourceManager->LoadFont("Kenney Bold.ttf", 40);
			m_fontId = 0;
			if (m_fontId < 0)
			{
				std::cout << "Failed to load font" << std::endl;
				m_initialized = false;
			}
			else
			{
				m_initialized = true;
			}
		}

		void Draw() override
		{
			auto id = Global::ResourceManager->GetTextTexture(m_fontId, "Hello SDL World!", Color::Red());
			if (id >= 0)
			{
				Texture& tex = Global::Engine->GetResourceManager()->textures[id];
				m_renderer->DrawText(tex, {(m_engine->width / 2.0f) - ((tex.w / m_renderer->xscale) / 2.0f),
										   (m_engine->height / 2.0f) - ((tex.h / m_renderer->yscale) / 2.0f)});
			}
		}
	};
} // namespace age
