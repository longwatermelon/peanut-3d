#pragma once
#include "object.h"
#include <memory>


namespace peanut
{
	std::unique_ptr<peautils::Graphics> gfx;
	std::vector<peautils::Object> objects;

	peautils::matrix4 mproj;
	
	void init(int w, int h, const char* title)
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

		for (auto& o : objects)
			o.project(*gfx, mproj, cam);

		SDL_RenderPresent(gfx->getrend());
	}
}