#include "include/peanut.h"
#include <iostream>
#include <Windows.h>


int main(int argc, char** argv)
{
	peanut::Camera cam(0.f, 0.f, 0.f);

	peanut::init(1000, 1000, "test", cam);	

	bool running = true;
	bool focused = false;
	SDL_Event evt;

	std::vector<peanut::Object> cubes;
	std::vector<peanut::Object> donuts;

	cubes.push_back(peanut::Object(0.0f, 0.0f, 30.0f, "meshfiles/spirala.facet"));

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
				case SDLK_9: donuts.clear(); break;
				case SDLK_1: cubes.push_back(peanut::Object(-cam.x, -cam.y, -cam.z + 4, "meshfiles/sphere.facet")); break;
				case SDLK_2: donuts.push_back(peanut::Object(-cam.x, -cam.y, -cam.z + 4, "meshfiles/donut.facet")); break;
				case SDLK_3: cubes.push_back(peanut::Object(-cam.x, -cam.y, -cam.z + 4, "meshfiles/cube.facet")); break;
				case SDLK_4: cubes.push_back(peanut::Object(-cam.x, -cam.y, -cam.z + 4, "meshfiles/spirala.facet")); break;
				case SDLK_5: cubes.push_back(peanut::Object(-cam.x, -cam.y, -cam.z + 4, "meshfiles/triangle.facet")); break;
				//case SDLK_6: std::cout << cam.ha << " | " << cam.va << " | " << cam.x << ", " << cam.y << ", " << cam.z << "\n"; break;
				}
			}
		}

		if (focused)
		{
			if (GetAsyncKeyState('W') < 0) cam.z -= 0.1f;
			if (GetAsyncKeyState('S') < 0) cam.z += 0.1f;
			if (GetAsyncKeyState('A') < 0) cam.x += 0.1f;
			if (GetAsyncKeyState('D') < 0) cam.x -= 0.1f;
			if (GetAsyncKeyState(VK_UP) < 0) cam.va -= 0.05f;
			if (GetAsyncKeyState(VK_DOWN) < 0) cam.va += 0.05f;
			if (GetAsyncKeyState(VK_RIGHT) < 0) cam.ha += 0.05f;
			if (GetAsyncKeyState(VK_LEFT) < 0) cam.ha -= 0.05f;
			if (GetAsyncKeyState(VK_SHIFT) < 0) cam.y -= 0.05f;
			if (GetAsyncKeyState(VK_SPACE) < 0) cam.y += 0.05f;

			if (GetAsyncKeyState(VK_ESCAPE))
			{
				SDL_SetRelativeMouseMode(SDL_FALSE);
				focused = false;
			}
		}
		
		if (GetAsyncKeyState(VK_TAB))
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			focused = true;
		}

		

		peanut::clear_screen();

		peanut::render(cam, cubes);
		peanut::render(cam, donuts);

		peanut::update_screen();
	}

	SDL_Quit();

	return 0;
}