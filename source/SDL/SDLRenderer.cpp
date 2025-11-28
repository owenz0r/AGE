#include <iostream>
#include <sstream>

#include "SDL.h"
#include "SDL/SDLRenderer.h"
#include "SDL/SDLWindow.h"
#include "SDL_image.h"
#include "core/Renderer.h"
#include "core/ResourceManager.h"

#define DRAWGRID 0

namespace age
{
	SDLRenderer::~SDLRenderer()
	{
	}

	bool SDLRenderer::Init(const float xs, const float ys, Window* window)
	{
		xscale = xs;
		yscale = ys;
		aspect = xscale / yscale;
		return _InitInternal(dynamic_cast<SDLWindow*>(window));
	}

	Point2Di SDLRenderer::WorldToScreenCoords(float x, float y) const
	{
		return Point2Di();
	}

	Vec2f SDLRenderer::ScreenToWorldCoords(int x, int y) const
	{
		return {(float)x / xscale, (float)y / yscale};
	}

	void SDLRenderer::DrawQuad(const Rect& rect, const Color& color) const
	{
		SDL_Rect dest;
		dest.x = rect.x * xscale;
		;						  //(rect.x - (rect.w / 2.0f)) * m_xscale; // SDL origin is top left
		dest.y = rect.y * yscale; //(rect.y - (rect.h / 2.0f)) * m_yscale; // whereas
								  // Game and Box2D are centered
		dest.w = rect.w * xscale;
		dest.h = rect.h * yscale;

		SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(m_renderer.get(), &dest);
	}

	void SDLRenderer::DrawEmptyQuad(const Rect& rect, const Color& color) const
	{
		SDL_Rect dest;
		dest.x = ((rect.x - (rect.w / 2.0f)) * xscale) + 0.5f; // SDL origin is top left
		dest.y = ((rect.y - (rect.h / 2.0f)) * yscale) + 0.5f; // whereas Game and Box2D are centered
		dest.w = (rect.w * xscale) + 0.5f;
		dest.h = (rect.h * yscale) + 0.5f;

		SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
		SDL_RenderDrawLine(m_renderer.get(), dest.x, dest.y, dest.x + dest.w, dest.y);
		SDL_RenderDrawLine(m_renderer.get(), dest.x + dest.w, dest.y, dest.x + dest.w, dest.y + dest.h);
		SDL_RenderDrawLine(m_renderer.get(), dest.x + dest.w, dest.y + dest.h, dest.x, dest.y + dest.h);
		SDL_RenderDrawLine(m_renderer.get(), dest.x, dest.y + dest.h, dest.x, dest.y);
	}

	void SDLRenderer::DrawLine(const float x1, const float y1, const float x2, const float y2, const Color& color) const
	{
		SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
		SDL_RenderDrawLine(m_renderer.get(), x1 * xscale, y1 * yscale, x2 * xscale, y2 * yscale);
	}

	void SDLRenderer::DrawCircle(Point2Df pos, float radius, const Color& color) const
	{
		radius *= yscale;

		pos.x *= xscale;
		pos.y *= yscale;

		int x = radius;
		int y = 0;

		SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
		SDL_RenderDrawPointF(m_renderer.get(), pos.x + x * aspect, pos.y + y);

		if (radius > 0.0f)
		{
			SDL_RenderDrawPointF(m_renderer.get(), pos.x + x * aspect, pos.y - y);
			SDL_RenderDrawPointF(m_renderer.get(), pos.x + y * aspect, pos.y + x);
			SDL_RenderDrawPointF(m_renderer.get(), pos.x - y * aspect, pos.y + x);
		}

		int P = 1 - radius;
		while (x > y)
		{
			y++;
			// y += yscale;

			if (P <= 0)
			{
				P = P + 2 * y + 1;
			}
			else
			{
				x--;
				// x -= xscale;
				P = P + 2 * y - 2 * x + 1;
			}

			if (x < y)
				break;

			SDL_RenderDrawPointF(m_renderer.get(), pos.x + x * aspect, pos.y + y);
			SDL_RenderDrawPointF(m_renderer.get(), pos.x - x * aspect, pos.y + y);
			SDL_RenderDrawPointF(m_renderer.get(), pos.x + x * aspect, pos.y - y);
			SDL_RenderDrawPointF(m_renderer.get(), pos.x - x * aspect, pos.y - y);

			if (x != y)
			{
				SDL_RenderDrawPointF(m_renderer.get(), pos.x + y * aspect, pos.y + x);
				SDL_RenderDrawPointF(m_renderer.get(), pos.x - y * aspect, pos.y + x);
				SDL_RenderDrawPointF(m_renderer.get(), pos.x + y * aspect, pos.y - x);
				SDL_RenderDrawPointF(m_renderer.get(), pos.x - y * aspect, pos.y - x);
			}
		}
	}

	void SDLRenderer::DrawQuad(const Point2Di& topLeft, const Point2Di& bottomRight, const Color& color) const
	{
		SDL_Rect fillRect = {topLeft.x, topLeft.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y};
		SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(m_renderer.get(), &fillRect);
	}

	void SDLRenderer::SetDrawColor(const Color& color) const
	{
		SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
	}

	void SDLRenderer::RenderClear() const
	{
		SDL_RenderClear(m_renderer.get());
	}

	void SDLRenderer::Display() const
	{
		SDL_RenderPresent(m_renderer.get());
	}

	void SDLRenderer::DrawRectTexture(const int textureId, const Rect& dstrect, const double radians,
									  const Color color) const
	{
		SDL_Rect dest;
		dest.x = ((dstrect.x - (dstrect.w / 2.0f)) * xscale) + 0.5f; // SDL origin is top left
		dest.y = ((dstrect.y - (dstrect.h / 2.0f)) * yscale) + 0.5f; // whereas Game and Box2D are centered
		dest.w = (dstrect.w * xscale) + 0.5f;
		dest.h = (dstrect.h * yscale) + 0.5f;

		auto sdltexture = (SDL_Texture*)Global::ResourceManager->GetTextureData(textureId);
		SDL_SetTextureColorMod(sdltexture, color.r, color.g, color.b);
		SDL_RenderCopyEx(m_renderer.get(), sdltexture, nullptr, &dest, radians * 57.295779513, nullptr,
						 SDL_RendererFlip::SDL_FLIP_NONE);

#if DRAWGRID
		SDL_SetRenderDrawColor(m_renderer.get(), 0, 255, 0, 255);
		for (float x = 0.0f; x < 40.0f; x = x + 1.0f)
		{
			SDL_RenderDrawLine(m_renderer.get(), x * xscale, 0, x * xscale, m_window->m_height);
		}
		for (float y = 0.0f; y < 32.0f; y = y + 1.0f)
		{
			SDL_RenderDrawLine(m_renderer.get(), 0, y * yscale, m_window->m_width, y * yscale);
		}
#endif
	}

	void SDLRenderer::DrawText(const int texId, const Point2Df pos, const double radians) const
	{
		DrawText(Global::ResourceManager->textures[texId], pos, radians);
	}

	void SDLRenderer::DrawText(const Texture& tex, const Point2Df pos, const double radians) const
	{
		SDL_Rect dest;

		dest.x = pos.x * xscale; // convert to pixel coords
		dest.y = pos.y * yscale; // convert to pixel coords
		dest.w = tex.w;
		dest.h = tex.h;

		SDL_RenderCopyEx(m_renderer.get(), static_cast<SDL_Texture*>(tex.data), nullptr, &dest, radians * 57.295779513,
						 nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
	}

	void SDLRenderer::DrawText(const TextParams& params)
	{
		if (params.temp)
		{
			Texture tex = Global::ResourceManager->TempTextTexture(params.fontId, params.text, params.color);
			if (tex.data)
			{
				SDL_Rect dest;

				dest.x = params.pos.x * xscale; // convert to pixel coords
				dest.y = params.pos.y * yscale; // convert to pixel coords
				dest.w = params.width == 0.0f ? tex.w : params.width * xscale;
				dest.h = params.height == 0.0f ? tex.h : params.height * yscale;

				auto sdltexture = static_cast<SDL_Texture*>(tex.data);
				SDL_SetTextureColorMod(sdltexture, params.color.r, params.color.g, params.color.b);
				SDL_RenderCopyEx(m_renderer.get(), sdltexture, nullptr, &dest, params.radians * 57.295779513, nullptr,
								 SDL_RendererFlip::SDL_FLIP_NONE);
				SDL_DestroyTexture(sdltexture);
			}
		}
		else
		{
			auto id = Global::ResourceManager->GetTextTexture(params.fontId, params.text, params.color);
			if (id < 0)
				return;
			Texture& tex = Global::ResourceManager->textures[id];

			SDL_Rect dest;

			dest.x = params.pos.x * xscale; // convert to pixel coords
			dest.y = params.pos.y * yscale; // convert to pixel coords
			dest.w = params.width == 0.0f ? tex.w : params.width * xscale;
			dest.h = params.height == 0.0f ? tex.h : params.height * yscale;

			SDL_SetTextureColorMod(static_cast<SDL_Texture*>(tex.data), params.color.r, params.color.g, params.color.b);
			SDL_RenderCopyEx(m_renderer.get(), static_cast<SDL_Texture*>(tex.data), nullptr, &dest,
							 params.radians * 57.295779513, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
		}
	}
	void SDLRenderer::DrawText(const std::string text, const Point2Df pos, const double radians, const Color color)
	{
		TextParams params;
		params.fontId = 0;
		params.text = text;
		params.pos = pos;
		params.radians = radians;
		params.color = color;
		params.temp = true;
		DrawText(params);
	}

	// private functions

	bool SDLRenderer::_InitInternal(SDLWindow* window)
	{
		m_window = window;

		m_renderer.reset(SDL_CreateRenderer(window->GetInternal(), -1, SDL_RENDERER_ACCELERATED));
		if (m_renderer == nullptr)
    {
      std::cout << "SDL Renderer Init Internal failed" << std::endl;
			return false;
    }

		return true;
	}

	void* SDLRenderer::GetInternal() const
	{
		return m_renderer.get();
	}
} // namespace age
