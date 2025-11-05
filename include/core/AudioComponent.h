#pragma once

namespace age
{
	class Audio;

	class AudioComponent
	{
	  public:
		int eid = -1;
		int m_soundID = -1;
		bool alive = true;

		AudioComponent() = default;
		AudioComponent(int eID);

		void SetSound(const int sId)
		{
			m_soundID = sId;
		}
		void Play(Audio* a) const;
	};
} // namespace age
