
#include "core/AudioComponentManager.h"

namespace age
{
	int AudioComponentManager::CreateComponent(int eID)
	{
		return m_internal.CreateComponent(eID);
	}

	int AudioComponentManager::CreateComponentWithSound(int eID, const int sID)
	{
		auto idx = m_internal.CreateComponent(eID);
		if (idx.isValid())
			m_internal.m_array[idx].SetSound(sID);
		return idx;
	}

	void AudioComponentManager::Play(int idx)
	{
		m_internal.m_array[idx].Play(m_audio);
	}

	void AudioComponentManager::Kill(int eID)
	{
		m_internal.Kill(eID);
	}
} // namespace age
