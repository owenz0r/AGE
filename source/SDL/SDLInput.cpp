#include "SDL/SDLInput.h"

namespace age
{
	void SDLInput::processKeyDown(const SDL_Event& e)
	{
		if (m_keydownmap.count(e.key.keysym.sym) > 0)
		{
			m_keydownmap[e.key.keysym.sym]();
		}

		switch (e.key.keysym.sym)
		{
			case SDLK_ESCAPE:
				if (m_userQuitCallback)
					m_userQuitCallback();
				// g_quit = true;
				break;
		}
	}

	void SDLInput::processKeyUp(const SDL_Event& e)
	{
		if (m_keyupmap.count(e.key.keysym.sym) > 0)
		{
			m_keyupmap[e.key.keysym.sym]();
		}
	}

	void SDLInput::processMouseDown(const SDL_Event& e)
	{
		if (m_mouseDownCallback != nullptr)
			m_mouseDownCallback(e.button.x, e.button.y);
	}

	void SDLInput::processMouseUp(const SDL_Event& e)
	{
		if (m_mouseUpCallback != nullptr)
			m_mouseUpCallback(e.button.x, e.button.y);
	}

	void SDLInput::processMouseMoved(const SDL_Event& e)
	{
		if (m_mouseMoveCallback != nullptr)
			m_mouseMoveCallback(e.motion.x, e.motion.y);
	}

	void SDLInput::ProcessInput()
	{
		SDL_Event e;
		// poll events
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				if (m_userQuitCallback)
					m_userQuitCallback();
				// g_quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				processKeyDown(e);
			}
			else if (e.type == SDL_KEYUP)
			{
				processKeyUp(e);
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				processMouseDown(e);
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				processMouseUp(e);
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				processMouseMoved(e);
			}
		}
	}

	void SDLInput::SetQuitCallback(std::function<void()> callback)
	{
		m_userQuitCallback = callback;
	}
} // namespace age
