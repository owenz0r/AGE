#pragma once

#include "core/Font.h"
#include "core/Texture.h"
#include "core/Utils.h"
#include "core/globals.h"
#include <filesystem>
#include <string>
#include <unordered_map>

namespace age
{
	class Renderer;

	class ResourceManager
	{
	  protected:
		static constexpr int MAX_TEXTURES = 64;
		int m_textureCount = 1;
		std::unordered_map<std::string, int> m_textureHashMap;

		static constexpr int MAX_FONTS = 16;
		int m_fontCount = 0;
		std::unordered_map<std::string, int> m_fontMap;

		std::filesystem::path root;
		std::filesystem::path fontsDir = "fonts";
		std::filesystem::path texturesDir = "textures";
		std::filesystem::path audioDir = "audio";

	  public:
		Texture textures[MAX_TEXTURES];
		Font m_fonts[MAX_FONTS];

		virtual ~ResourceManager() = default;

		virtual bool Init(Renderer* r, std::filesystem::path rootPath) = 0;
		virtual int LoadTexture(std::filesystem::path path) = 0;
		virtual int LoadFont(std::filesystem::path path, int size) = 0;
		virtual int GetTextTexture(const int fontId, const std::string_view text, const Color color) = 0;
		virtual Texture TempTextTexture(const int fontId, const std::string_view text, const Color color) = 0;

		virtual void* GetTextureData(const int id) const = 0;
	};
} // namespace age
