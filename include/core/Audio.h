#pragma once

#include <string>

namespace age
{
	class Audio
	{
	  protected:
		float m_xscale = 1.0f;
		float m_yscale = 1.0f;

	  public:
		virtual ~Audio() = default;
		virtual bool Init() = 0;
		virtual int LoadClip(std::string path) = 0;
		virtual void PlayClip(int id) = 0;
	};
} // namespace age
