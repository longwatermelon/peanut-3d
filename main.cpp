#include "include/peanut.h"
#include <iostream>


int main(int argc, char** argv)
{
	peanut::init(1000, 1000, "test");

	peanut::Camera cam(0.0f, 0.0f, 0.0f);

	bool running = true;
	SDL_Event evt;

	peanut::new_object(-0.5, 0, 3, "meshfiles/cube.facet");

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
				case SDLK_UP: cam.y += 0.1f; break;
				case SDLK_DOWN: cam.y -= 0.1f; break;
				}
			}
		}

		peanut::render(cam);
	}

	SDL_Quit();

	return 0;
}