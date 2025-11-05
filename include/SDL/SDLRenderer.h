#pragma once

#include <memory>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"
#include "core/Renderer.h"
#include "core/Utils.h"

namespace age
{
	class Game;
	class SDLWindow;

	class SDLRenderer : public Renderer
	{
	  public:
		~SDLRenderer();
		bool Init(const float xscale, const float yscale, Window* window) override;
		void DrawQuad(const Point2Di& topLeft, const Point2Di& bottomRight, const Color& color) const override;
		void DrawQuad(const Rect& rect, const Color& color) const override;
		void DrawEmptyQuad(const Rect& rect, const Color& color) const override;
		void DrawLine(const float x1, const float y1, const float x2, const float y2,
					  const Color& color) const override;
		void DrawCircle(Point2Df pos, float radius, const Color& color) const override;
		void SetDrawColor(const Color& color) const override;
		void RenderClear() const override;
		void Display() const override;
		void DrawRectTexture(const int textureId, const Rect& dstrect, const double radians,
							 Color color) const override;
		void DrawText(const std::string, const Point2Df pos, const double radians, Color color) override;
		void DrawText(const TextParams& params) override;
		void DrawText(const Texture& tex, const Point2Df pos, const double radians) const override;
		void DrawText(const int textId, const Point2Df pos, const double radians) const override;
		Point2Di WorldToScreenCoords(float x, float y) const override;
		Vec2f ScreenToWorldCoords(int x, int y) const override;

		//	int LoadTexture(std::string path) override;
		// int CreateTextTexture(const int fontId, const std::string text, const Color
		// color) override; Texture TempTextTexture(const int fontId, const
		// std::string text, const Color color) override;
		//	int LoadFont(std::string path, int size) override;

		// SDL_Texture* GetTextureData(const int id) const;
		void* GetInternal() const override;

	  private:
		bool _InitInternal(SDLWindow* window);
		//	Texture _LoadTexture(std::string path);
		//	Texture _CreateText(const int fontId, const std::string path, const
		// SDL_Color color) const; 	TTF_Font* _LoadFont(std::string path, int size);

		std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_renderer = {nullptr, SDL_DestroyRenderer};

		// TTF_Font* m_font = nullptr;
		SDLWindow* m_window = nullptr;
		Game* m_game = nullptr;

		//	static constexpr int MAX_FONTS = 16;
		//	TTF_Font* m_fonts[MAX_FONTS];
		//	int m_fontCount = 1;
		//	std::unordered_map<std::string, int> m_fontMap;
		//	std::unordered_map<int, int> m_testMap;
	};
} // namespace age
