#pragma once

#include "SDL.h"
#include "core/Input.h"

namespace age
{
	class SDLInput : public Input
	{
	  public:
		void processKeyDown(const SDL_Event& e);
		void processKeyUp(const SDL_Event& e);
		void processMouseDown(const SDL_Event& e);
		void processMouseUp(const SDL_Event& e);
		void processMouseMoved(const SDL_Event& e);
		void ProcessInput() override;
		void SetQuitCallback(std::function<void()> callback) override;
	};
} // namespace age
