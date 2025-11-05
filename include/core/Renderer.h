#pragma once

#include <stdint.h>
#include <string>
#include <unordered_map>

#include "Texture.h"
#include "Utils.h"

namespace age
{
	class Window;

	struct TextParams
	{
		int fontId = 0;
		std::string text = "";
		Point2Df pos = {0.0f, 0.0f};
		double radians = 0.0;
		Color color = Color::White();
		float width = 0.0f;
		float height = 0.0f;
		bool temp = false;
	};

	class Renderer
	{
	  protected:
		//	static constexpr int MAX_TEXTURES = 64;
		//	int m_textureCount = 1;
		//	std::unordered_map<std::string, int> m_textureHashMap;

	  public:
		virtual ~Renderer() = default;
		virtual bool Init(const float xscale, const float yscale, Window* window) = 0;
		virtual void DrawQuad(const Point2Di& topLeft, const Point2Di& bottomRight, const Color& color) const = 0;
		virtual void DrawQuad(const Rect& rect, const Color& color) const = 0;
		virtual void DrawEmptyQuad(const Rect& rect, const Color& color) const = 0;
		virtual void DrawLine(const float x1, const float y1, const float x2, const float y2,
							  const Color& color) const = 0;
		virtual void DrawCircle(Point2Df pos, float radius, const Color& color) const = 0;
		virtual void SetDrawColor(const Color& color) const = 0;
		virtual void RenderClear() const = 0;
		virtual void Display() const = 0;
		virtual void DrawRectTexture(const int textureId, const Rect& dstrect, const double radians,
									 const Color) const = 0;
		virtual void DrawText(const std::string, const Point2Df pos, const double radians, Color color) = 0;
		virtual void DrawText(const TextParams& params) = 0;
		virtual void DrawText(const Texture& tex, const Point2Df pos, const double radians = 0.0) const = 0;
		virtual void DrawText(const int texId, const Point2Df pos, const double radians = 0.0) const = 0;
		virtual Point2Di WorldToScreenCoords(float x, float y) const = 0;
		virtual Vec2f ScreenToWorldCoords(int x, int y) const = 0;

		// virtual int LoadTexture(std::string path) = 0;
		// virtual int CreateTextTexture(const int fontId, const std::string text,
		// const Color color) = 0; virtual Texture TempTextTexture(const int fontId,
		// const std::string text, const Color color) = 0; virtual int
		// LoadFont(std::string path, int size) = 0;

		virtual void* GetInternal() const = 0;

		// Texture textures[MAX_TEXTURES];
		float xscale = 1.0f;
		float yscale = 1.0f;
		float aspect = 1.0f;
	};
} // namespace age
