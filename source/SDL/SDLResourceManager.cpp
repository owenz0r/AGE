#include "SDL.h"
#include "SDL_image.h"
// #include "SDL/SDL_render.h"
#include "SDL/SDLRenderer.h"
#include "SDL/SDLResourceManager.h"
#include "extras/kenney_bold_font.h"

#include <iostream>
#include <sstream>
#include <string_view>

extern unsigned char Kenney_Bold_ttf[];
extern unsigned int Kenney_Bold_ttf_len;

namespace age
{
	SDLResourceManager::SDLResourceManager()
	{
		Global::ResourceManager = this;
	}

	SDLResourceManager::~SDLResourceManager()
	{
		for (int i = 0; i < m_textureCount; ++i)
			SDL_DestroyTexture((SDL_Texture*)GetTextureData(i));
		IMG_Quit();
		TTF_Quit();
	}

	Texture SDLResourceManager::TempTextTexture(const int fontId, const std::string_view text, const Color color)
	{
		SDL_Color c;
		c.r = color.r;
		c.g = color.g;
		c.b = color.b;
		c.a = color.a;
		return _CreateText(fontId, text, c);
	}

	Texture SDLResourceManager::_CreateText(const int fontId, const std::string_view text, const SDL_Color color) const
	{
		Texture result;
		SDL_Surface* textSurface = TTF_RenderUTF8_Solid(m_fonts[fontId], text.data(), color);
		if (textSurface)
		{
			result.data = SDL_CreateTextureFromSurface(m_renderer, textSurface);
			if (result.data)
			{
				result.w = textSurface->w;
				result.h = textSurface->h;
			}
			SDL_FreeSurface(textSurface);
		}
		return result;
	}

	void* SDLResourceManager::GetTextureData(const int id) const
	{
		if (id > 0 && id < m_textureCount)
			return textures[id].data;
		else
			return nullptr;
	}

	int SDLResourceManager::GetTextTexture(const int fontId, const std::string_view text, const Color color)
	{
		std::ostringstream oss;
		oss << fontId << text << color.r << color.g << color.b << color.a;
		std::string hash = oss.str();

		if (m_textureHashMap.count(hash) > 0)
			return m_textureHashMap[hash];

		if (m_textureCount < MAX_TEXTURES - 1)
		{
			SDL_Color c;
			c.r = color.r;
			c.g = color.g;
			c.b = color.b;
			c.a = color.a;
			Texture t = _CreateText(fontId, text, c);
			if (t.data == nullptr)
				return -1;
			textures[m_textureCount] = t;
			m_textureHashMap[hash] = m_textureCount;
			return m_textureCount++;
		}
		return -1;
	}

	bool SDLResourceManager::Init(Renderer* r, std::filesystem::path rootPath)
	{
		m_renderer = (::SDL_Renderer*)r->GetInternal();

		// init texture loading
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
			return false;

		// init fonts
		if (TTF_Init() == -1)
		{
			printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
			return false;
		}

		root = rootPath;

		// Load default font
		SDL_RWops* rwops = SDL_RWFromConstMem(Kenney_Bold_ttf, Kenney_Bold_ttf_len);
		if (!rwops)
		{
			std::cout << "Failed to load default Font from Memory" << std::endl;
			return false;
		}

		TTF_Font* dFont = TTF_OpenFontRW(rwops, 1, 12);
		if (!dFont)
		{
			std::cout << "Failed to load default Font from RW" << std::endl;
			return false;
		}

		m_fonts[m_fontCount++] = dFont;

		return true;
	}

	TTF_Font* SDLResourceManager::_LoadSDLFont(std::filesystem::path path, int size)
	{
		auto fullpath = root / fontsDir / path;
		if (std::filesystem::exists(fullpath))
		{
			return TTF_OpenFont(path.string().c_str(), size);
		}
		else
		{
			std::cout << "Failed to find font at " << fullpath << std::endl;
			return nullptr;
		}
	}

	int SDLResourceManager::LoadFont(std::filesystem::path path, int size)
	{
		std::ostringstream oss;
		oss << path << size;
		std::string hash = oss.str();

		if (m_fontMap.count(hash) > 0)
			return m_fontMap[hash];

		if (m_fontCount < MAX_FONTS - 1)
		{
			TTF_Font* f = _LoadSDLFont(path, size);
			if (f == nullptr)
				return -1;
			m_fonts[m_fontCount] = f;
			m_fontMap[hash] = m_fontCount;
			return m_fontCount++;
		}
		return -1;
	}

	Texture SDLResourceManager::_LoadSDLTexture(std::filesystem::path path)
	{
		Texture result;

		if (!std::filesystem::exists(path))
		{
			std::cout << "Failed to find Texture at " << path << std::endl;
			return result;
		}

		SDL_Surface* loadedSurface = IMG_Load(path.string().c_str());

		if (loadedSurface)
		{
			result.data = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
			if (result.data)
			{
				result.w = loadedSurface->w;
				result.h = loadedSurface->h;
			}
			SDL_FreeSurface(loadedSurface);
		}
		return result;
	}

	int SDLResourceManager::LoadTexture(std::filesystem::path path)
	{
		path = root / texturesDir / path;

		if (m_textureHashMap.count(path.string()) > 0)
			return m_textureHashMap[path.string()];

		if (m_textureCount < MAX_TEXTURES - 1)
		{
			Texture t = _LoadSDLTexture(path);
			if (t.data == nullptr)
				return -1;
			textures[m_textureCount] = t;
			m_textureHashMap[path.string()] = m_textureCount;
			return m_textureCount++;
		}
		return -1;
	}
} // namespace age
