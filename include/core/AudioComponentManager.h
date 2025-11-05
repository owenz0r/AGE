#pragma once

#include "AudioComponent.h"
#include "ComponentManager.h"

namespace age
{
	class Audio;

	class AudioComponentManager
	{
		Audio* m_audio;

		ComponentManager<AudioComponent> m_internal;

	  public:
		AudioComponentManager(Audio* a) : m_audio(a) {};
		int CreateComponent(int eID);
		int CreateComponentWithSound(int eID, const int sId);
		void Play(int idx);
		void Kill(int eID);
	};
} // namespace age
