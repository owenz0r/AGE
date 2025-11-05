#pragma once

#include "core/Audio.h"
#include <string>

class Mix_Chunk;

namespace age
{
	class SDLAudio : public Audio
	{
	  public:
		bool Init() override;
		int LoadClip(std::string path) override;
		void PlayClip(int id) override;

	  private:
		Mix_Chunk* _LoadClip(std::string path);

		static constexpr int MAX_CHUNKS = 16;
		Mix_Chunk* m_chunks[MAX_CHUNKS];
		int m_chunkCount = 1;
	};
} // namespace age
