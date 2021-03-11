#pragma once
#include "utils.h"
#include <SDL.h>


namespace peanut::peautils
{
	class Graphics
	{
	public:
		Graphics() = default;

		Graphics(int w, int h, const char* title)
			: screen_w(w), screen_h(h)
		{
			SDL_Init(SDL_INIT_EVERYTHING);

			window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h, SDL_WINDOW_SHOWN);
			rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			tex = SDL_CreateTexture(rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screen_w, screen_h);
			texbuf = new uint32_t[screen_w * screen_h];
		}

		~Graphics()
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(rend);

			delete[] texbuf;
			SDL_DestroyTexture(tex);

			SDL_Quit();
		}


		void draw_wireframe_triangle(point& p1, point& p2, point& p3)
		{
			SDL_RenderDrawLine(rend, p1.x, p1.y, p2.x, p2.y);
			SDL_RenderDrawLine(rend, p2.x, p2.y, p3.x, p3.y);
			SDL_RenderDrawLine(rend, p3.x, p3.y, p1.x, p1.y);
		}


		int getw() { return screen_w; }
		int geth() { return screen_h; }

		SDL_Renderer* getrend() { return rend; }

	private:
		SDL_Window* window{ nullptr };
		SDL_Renderer* rend{ nullptr };

		int screen_w{ 0 }, screen_h{ 0 };

		SDL_Texture* tex;
		uint32_t* texbuf;
	};
}