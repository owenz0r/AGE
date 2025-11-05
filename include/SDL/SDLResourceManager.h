#include "SDL_render.h"
#include "SDL_ttf.h"
#include "core/ResourceManager.h"
#include "core/Utils.h"
#include <filesystem>

namespace age
{
	class SDL_Renderer;

	class SDLResourceManager : public ResourceManager
	{
		static constexpr int MAX_FONTS = 16;
		TTF_Font* m_fonts[MAX_FONTS];
		int m_fontCount = 0;
		std::unordered_map<std::string, int> m_fontMap;

		Texture _LoadSDLTexture(std::filesystem::path path);
		TTF_Font* _LoadSDLFont(std::filesystem::path path, int size);
		Texture _CreateText(const int fontId, const std::string_view text, const SDL_Color color) const;

	  public:
		SDLResourceManager();
		~SDLResourceManager();
		::SDL_Renderer* m_renderer;

		bool Init(Renderer* r, std::filesystem::path rootPath) override;
		int LoadTexture(std::filesystem::path path) override;
		int LoadFont(std::filesystem::path path, int size) override;
		int GetTextTexture(const int fontId, const std::string_view text, const Color color) override;
		Texture TempTextTexture(const int fontId, const std::string_view text, const Color color) override;
		void* GetTextureData(const int id) const override;
	};
} // namespace age
