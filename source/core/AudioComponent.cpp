#include "core/AudioComponent.h"
#include "core/Audio.h"

namespace age
{
	void AudioComponent::Play(Audio* a) const
	{
		a->PlayClip(m_soundID);
	}
} // namespace age
