#include "core/Engine.h"
#include "SDL/SDLAudio.h"
#include "SDL/SDLDefaultScreen.h"
#include "SDL/SDLInput.h"
#include "SDL/SDLRenderer.h"
#include "SDL/SDLResourceManager.h"
#include "SDL/SDLWindow.h"
#include "core/Physics.h"
#include "core/Screen.h"
#include "core/globals.h"

namespace age
{
	Engine::Engine()
	{
		Global::Engine = this;
	};

	Engine::~Engine()
	{
		SDL_Quit();
		Global::Engine = nullptr;
	}

	void Engine::_Update(const double dt)
	{
		m_activeScreen->Update(dt);
	}

	void Engine::_TimeStep()
	{
		auto newTime = Clock::now();
		std::chrono::duration<double> ft = newTime - m_currentTime;
		double frameTime = ft.count();
		m_currentTime = newTime;

		while (frameTime > 0.0)
		{
			double deltaTime = fmin(frameTime, tickRate);
			_Update(deltaTime);
			frameTime -= deltaTime;
		}
	}

	void Engine::_Render()
	{
		m_renderer->SetDrawColor({0x00, 0x00, 0x00, 0xFF});
		m_renderer->RenderClear();

		m_activeScreen->Draw();

		m_renderer->Display();
	}

	void Engine::_ProcessInput()
	{
		m_activeScreen->ProcessInput();
	}

	bool Engine::Init(int screen_width, int screen_height, double tr)
	{
		tickRate = tr;

		m_window = std::make_unique<SDLWindow>();
		if (!m_window->Init(screen_width, screen_height))
			return false;

		m_renderer = std::make_unique<SDLRenderer>();

		if (!m_renderer->Init(m_window->m_width / width, m_window->m_height / height, m_window.get()))
			return false;

		m_resourceManager = std::make_unique<SDLResourceManager>();
		if (!m_resourceManager->Init(m_renderer.get(), "./media"))
		{
			std::cout << "Failed to initialize Resource Manager" << std::endl;
			return false;
		}

		m_audio = std::make_unique<SDLAudio>();
		m_audio->Init();

		m_physics = std::make_unique<Physics>();

		return true;
	}

	void Engine::Run()
	{
		if (!m_activeScreen)
		{
			m_activeScreen = new SDLDefaultScreen(m_renderer.get(), this);
			m_activeScreen->Init();
		}

		m_currentTime = Clock::now();
		srand(static_cast<unsigned>(time(nullptr)));

		while (!m_quit)
		{
			_TimeStep();
			_Render();
			_ProcessInput();
			SDL_Delay(10);
		}
	}

	bool Engine::InitializeScreen(Screen* screen)
	{
		if (!screen->m_initialized)
		{
			screen->Setup(m_physics.get(), m_audio.get(), m_renderer.get(), this);
			screen->Init();
		}
		else
		{
			screen->Reset();
		}
		return screen->m_initialized;
	}
	void Engine::Transition(std::string screen_name, std::string transition_data)
	{
		if (m_screenMap.count(screen_name) > 0)
		{
			auto screen = m_screenMap[screen_name];
			screen->SetTransitionData(transition_data);
			if (InitializeScreen(screen))
				m_activeScreen = screen;
		}
	}

	void Engine::Quit()
	{
		m_quit = true;
	}

	ResourceManager* Engine::GetResourceManager()
	{
		return m_resourceManager.get();
	}

	void Engine::AddScreen(std::string name, Screen* screen)
	{
		m_screenMap[name] = screen;
	}

	void Engine::SetActiveScreen(std::string name)
	{
		if (InitializeScreen(m_screenMap[name]))
			m_activeScreen = m_screenMap[name];
	}
} // namespace age
