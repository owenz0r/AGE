#pragma once

#include <memory>

#include "SDL.h"
#include "core/Window.h"

namespace age
{
	class SDLWindow : public Window
	{
	  public:
		bool Init(int width, int height);
		SDL_Window* GetInternal();

	  private:
		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window = {nullptr, SDL_DestroyWindow};
	};
} // namespace age
