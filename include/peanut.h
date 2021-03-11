#pragma once
#include "object.h"
#include <memory>


namespace peanut
{
	std::unique_ptr<peautils::Graphics> gfx;
	std::vector<peautils::Object> objects;

	peautils::matrix4 mproj;
	peautils::matrix3 roty;
	peautils::matrix3 rotx;
	
	void init(int w, int h, const char* title, Camera& cam)
	{
		gfx = std::make_unique<peautils::Graphics>(w, h, title);

		mproj = peautils::make_projection(90.0f, w / h, 1.0f, 1000.0f);
	}


	void new_object(float x, float y, float z, const char* path)
	{
		objects.push_back(peautils::Object(x, y, z, path));
	}


	void render(Camera& cam)
	{
		SDL_RenderClear(gfx->getrend());
		gfx->reset_texbuf();

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
			o.project(*gfx, mproj, rotx, roty, cam);

		gfx->update_texture();

		SDL_RenderPresent(gfx->getrend());
	}
}