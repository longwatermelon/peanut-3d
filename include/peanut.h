#pragma once
#include "object.h"
#include <memory>


namespace peanut
{
	inline std::unique_ptr<peautils::Graphics> gfx;
	
	inline peautils::matrix3 roty;
	inline peautils::matrix3 rotx;
	
	inline void init(int w, int h, const char* title, Camera& cam)
	{
		gfx = std::make_unique<peautils::Graphics>(w, h, title);
	}


	inline void render(Camera& cam, std::vector<Object>& objects)
	{
		rotx = { {
			{1, 0, 0},
			{0, cosf(cam.va), sinf(cam.va)},
			{0, -sinf(cam.va), cosf(cam.va)}
		} };

		roty = { {
			{cosf(cam.ha), 0, sinf(cam.ha)},
			{0, 1, 0},
			{-sinf(cam.ha), 0, cosf(cam.ha)}
		} };

		for (auto& o : objects)
			o.project(*gfx, rotx, roty, cam);

		gfx->update_texture();
	}


	inline void clear_screen()
	{
		SDL_RenderClear(gfx->getrend());
		gfx->reset_texbuf();
	}

	inline void update_screen()
	{
		SDL_RenderPresent(gfx->getrend());
	}
}