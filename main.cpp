#include "include/peanut.h"
#include <iostream>
#include <Windows.h>


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
				case SDLK_0: cubes.clear(); break;
				case SDLK_1: cubes.push_back(peanut::Object(-cam.x, -cam.y, -cam.z + 4, "meshfiles/cube.facet")); break;
				case SDLK_2: donuts.push_back(peanut::Object(-cam.x, -cam.y, -cam.z + 4, "meshfiles/donut.facet")); break;
				}
			}
		}

		if (GetAsyncKeyState('W') < 0) cam.z -= 0.1f;
		if (GetAsyncKeyState('S') < 0) cam.z += 0.1f;
		if (GetAsyncKeyState('A') < 0) cam.x += 0.1f;
		if (GetAsyncKeyState('D') < 0) cam.x -= 0.1f;
		if (GetAsyncKeyState(VK_UP) < 0) cam.va -= 0.1f;
		if (GetAsyncKeyState(VK_DOWN) < 0) cam.va += 0.1f;
		if (GetAsyncKeyState(VK_RIGHT) < 0) cam.ha += 0.1f;
		if (GetAsyncKeyState(VK_LEFT) < 0) cam.ha -= 0.1f;
		if (GetAsyncKeyState(VK_SHIFT) < 0) cam.y -= 0.1f;
		if (GetAsyncKeyState(VK_SPACE) < 0) cam.y += 0.1f;

		if (GetAsyncKeyState(VK_ESCAPE)) SDL_SetRelativeMouseMode(SDL_FALSE);
		if (GetAsyncKeyState(VK_TAB)) SDL_SetRelativeMouseMode(SDL_TRUE);

		int x, y;
		SDL_GetMouseState(&x, &y);

		cam.ha = ((float)x / peanut::gfx->getw()) * (4 * 3.1415) - (2 * 3.1415);
		cam.va = ((float)y / peanut::gfx->geth()) * (4 * 3.1415) - (2 * 3.1415);

		peanut::clear_screen();

		peanut::render(cam, cubes);
		peanut::render(cam, donuts);

		peanut::update_screen();
	}

	SDL_Quit();

	return 0;
}