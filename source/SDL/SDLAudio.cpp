#include "SDL/SDLAudio.h"
#include "SDL.h"
#include "SDL_mixer.h"

#include <iostream>

namespace age
{
	bool SDLAudio::Init()
	{
		if (SDL_Init(SDL_INIT_AUDIO) < 0)
		{
			std::cout << "SDL couldn't init video" << std::endl;
			return false;
		}
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
			return false;
		}
		return true;
	}

	Mix_Chunk* SDLAudio::_LoadClip(std::string path)
	{
		return Mix_LoadWAV(path.c_str());
		;
	}

	int SDLAudio::LoadClip(const std::string path)
	{
		if (m_chunkCount < MAX_CHUNKS - 1)
		{
			::Mix_Chunk* a = _LoadClip(path);
			if (a == nullptr)
				return -1;
			m_chunks[m_chunkCount++] = a;
			return m_chunkCount - 1;
		}
		return -1;
	}

	void SDLAudio::PlayClip(int id)
	{
		::Mix_PlayChannel(-1, m_chunks[id], 0);
	}
} // namespace age
