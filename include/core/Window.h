#pragma once

namespace age
{
	class Window
	{
	  public:
		virtual ~Window() = default;
		virtual bool Init(int width, int height) = 0;

	  public:
		int m_width;  // pixels
		int m_height; // pixels
	};
} // namespace age
