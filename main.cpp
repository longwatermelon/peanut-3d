#include "include/peanut.h"
#include <iostream>


int main(int argc, char** argv)
{
	peanut::Camera cam(0.0f, 0.0f, 0.0f);

	peanut::init(1000, 1000, "test", cam);	

	bool running = true;
	SDL_Event evt;

	std::vector<peanut::Object> cubes;
	std::vector<peanut::Object> donuts;

	while (running)
	{
		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_QUIT: running = false; break;
			case SDL_KEYDOWN:
				switch (evt.key.keysym.sym)
				{
				case SDLK_SPACE: cam.y += 0.1f; break;
				case SDLK_LSHIFT: cam.y -= 0.1f; break;
				case SDLK_w: cam.z -= 0.1f; break;
				case SDLK_s: cam.z += 0.1f; break;
				case SDLK_a: cam.x += 0.1f; break;
				case SDLK_d: cam.x -= 0.1f; break;
				case SDLK_UP: cam.va -= 0.1f; break;
				case SDLK_DOWN: cam.va += 0.1f; break;
				case SDLK_RIGHT: cam.ha += 0.1f; break;
				case SDLK_LEFT: cam.ha -= 0.1f; break;
				case SDLK_0: cubes.clear(); break;
				case SDLK_1: cubes.push_back(peanut::Object(-cam.x, -cam.y, -cam.z + 4, "meshfiles/cube.facet")); break;
				case SDLK_2: donuts.push_back(peanut::Object(-cam.x, -cam.y, -cam.z + 4, "meshfiles/donut.facet")); break;
				}
			}
		}

		peanut::clear_screen();

		peanut::render(cam, cubes);
		peanut::render(cam, donuts);

		peanut::update_screen();
	}

	SDL_Quit();

	return 0;
}