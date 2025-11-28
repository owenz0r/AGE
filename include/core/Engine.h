#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <unordered_map>

namespace age
{

	class Renderer;
	class Window;
	class Screen;
	class Audio;
	class Physics;
	class ResourceManager;

	typedef std::chrono::steady_clock Clock;

	class Engine
	{
	  public:
		Engine();
		~Engine();
		virtual bool Init(int screen_width, int screen_height, double tr);
		virtual void Run();
		virtual void Quit();

		float width = 64;  // meters
		float height = 32; // meters

		virtual void Transition(std::string screen_name, std::string transition_data);
		void SetActiveScreen(std::string name);
		bool InitializeScreen(Screen* screen);

		bool m_quit = false;

		ResourceManager* GetResourceManager();
		void AddScreen(std::string name, Screen* screen);

	  protected:
		virtual void _TimeStep();
		virtual void _Update(const double dt);
		virtual void _Render();
		virtual void _ProcessInput();

		std::unique_ptr<ResourceManager> m_resourceManager;
		std::unique_ptr<Physics> m_physics;
		std::unique_ptr<Audio> m_audio;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<Window> m_window;

		Screen* m_activeScreen = nullptr;
		std::unordered_map<std::string, Screen*> m_screenMap;

		std::chrono::steady_clock::time_point m_currentTime = std::chrono::steady_clock::now();

		double tickRate = 1.0 / 60.0;
	};

} // namespace age
