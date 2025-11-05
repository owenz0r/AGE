#pragma once

#include <functional>
#include <unordered_map>

namespace age
{
	class Input
	{
	  public:
		virtual ~Input() = default;
		virtual void ProcessInput() = 0;
		virtual void SetQuitCallback(std::function<void()> callback) = 0;
		std::unordered_map<char, std::function<void()>> m_keydownmap;
		std::unordered_map<char, std::function<void()>> m_keyupmap;

		std::function<void()> m_userQuitCallback = nullptr;

		std::function<void(int x, int y)> m_mouseDownCallback = nullptr;
		std::function<void(int x, int y)> m_mouseUpCallback = nullptr;
		std::function<void(int x, int y)> m_mouseMoveCallback = nullptr;
	};
} // namespace age
