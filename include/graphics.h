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

			depths = new float[screen_w * screen_h];

			fullscreen_rect = { 0, 0, w, h };
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


		void draw_filled_triangle(pointgrp ps, pointgrp ops, boundsarray& xb, boundsarray& zb)
		{
			// sort all points from lowest to highest y
			if (ps.p1.y < ps.p0.y)
			{
				swap_points(ps.p1, ps.p0);
				swap_points(ops.p1, ops.p0);
			}

			if (ps.p2.y < ps.p0.y)
			{
				swap_points(ps.p2, ps.p0);
				swap_points(ops.p2, ops.p0);
			}

			if (ps.p2.y < ps.p1.y)
			{
				swap_points(ps.p2, ps.p1);
				swap_points(ops.p2, ops.p1);
			}

			point normal = triangle_normal_vector({ ops.p0, ops.p1, ops.p2 });
			point light = { 0, 0, 0 };

			// take distance from light to triangle
			// brightness should be inversely proportional to distance so the shade is divided by the distance at the end of the calculation
			light.x -= (ops.p0.x + ops.p1.x + ops.p2.x) / 3.f;
			light.y -= (ops.p0.y + ops.p1.y + ops.p2.y) / 3.f;
			light.z -= (ops.p0.z + ops.p1.z + ops.p2.z) / 3.f;

			float dist = std::sqrt(light.x * light.x + light.y * light.y + light.z * light.z);

			// light is bugged since not all triangles have the same normal direction despite being right next to each other
			// so i used the absolute value of the dot product of normal and light
			float shade = 50 + 10000 * std::abs(normal.x * light.x + normal.y * light.y + normal.z * light.z) / (dist * dist * dist);
			if (shade > 255.f) shade = 255.f;

			xb.l = std::vector<float>(screen_w);
			xb.r = std::vector<float>(screen_w);
			zb.l = std::vector<float>(screen_w);
			zb.r = std::vector<float>(screen_w);

			
			interpolate({ ps.p0.x, ps.p0.y, 1.f / ops.p0.z }, { ps.p1.x, ps.p1.y, 1.f / ops.p1.z }, xb.l, zb.l, screen_w);
			interpolate({ ps.p1.x, ps.p1.y, 1.f / ops.p1.z }, { ps.p2.x, ps.p2.y, 1.f / ops.p2.z }, xb.l, zb.l, screen_w);
			interpolate({ ps.p0.x, ps.p0.y, 1.f / ops.p0.z }, { ps.p2.x, ps.p2.y, 1.f / ops.p2.z }, xb.r, zb.r, screen_w);

			if (ps.p2.y > 1000) ps.p2.y = 1000;
			if (ps.p0.y < 0) ps.p0.y = 0;

			for (int y = (int)ps.p0.y; y < (int)ps.p2.y; ++y)
			{
				int minx = std::min(xb.l[y], xb.r[y]);
				int maxx = std::max(xb.l[y], xb.r[y]);
				
				// start from the leftmost x at xb.l[y] (left bound) and put a pixel down until xb.r[y] (right bound)
				for (int i = minx; i < maxx; ++i)
				{
					// algebraically manipulated equality of slopes calculated using points on the same line
					float iz = zb.l[y] + (i - xb.l[y]) * ((zb.r[y] - zb.l[y]) / (xb.r[y] - xb.l[y]));

					if (iz > depths[y * 1000 + i])
					{
						// drawing an array of colors to the screen is much faster than individually drawing out pixels
						texbuf[y * 1000 + i] = 0x00000000 | (int)shade << 16 | (int)shade << 8 | (int)shade;
						depths[y * 1000 + i] = iz;
					}
				}
			}
		}


		void reset_texbuf()
		{
			for (int i = 0; i < screen_w * screen_h; ++i)
			{
				texbuf[i] = 0x000000;
				depths[i] = 0.0f;
			}
		}

		void update_texture()
		{
			SDL_UpdateTexture(tex, 0, texbuf, screen_w * sizeof(uint32_t));

			SDL_RenderCopy(rend, tex, 0, &fullscreen_rect);
		}


		int getw() { return screen_w; }
		int geth() { return screen_h; }

		SDL_Renderer* getrend() { return rend; }
		SDL_Window* getwin() { return window; }

	private:
		SDL_Window* window{ nullptr };
		SDL_Renderer* rend{ nullptr };

		int screen_w{ 0 }, screen_h{ 0 };

		SDL_Texture* tex;
		uint32_t* texbuf;

		float* depths;

		SDL_Rect fullscreen_rect;
	};
}