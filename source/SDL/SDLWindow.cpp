#include <iostream>

#include "SDL.h"
#include "SDL/SDLWindow.h"
#include "core/globals.h"

namespace age
{
	bool SDLWindow::Init(int width, int height)
	{
		m_width = width;
		m_height = height;
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cout << "SDL couldn't init video" << std::endl;
			return false;
		}
		else
		{
			m_window.reset(SDL_CreateWindow("Holdit", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
											SDL_WINDOW_SHOWN));
			if (m_window == nullptr)
			{
				std::cout << "Couldn't create window" << std::endl;
				return false;
			}
		}
		return true;
	}

	SDL_Window* SDLWindow::GetInternal()
	{
		return m_window.get();
	}
} // namespace age
