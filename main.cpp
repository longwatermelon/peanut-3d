#include "include/peanut.h"
#include <iostream>


int main(int argc, char** argv)
{
	peanut::init(1000, 1000, "test");

	peanut::Camera cam(0.0f, 0.0f, 0.0f);

	bool running = true;
	SDL_Event evt;

	peanut::new_object(-1, 0, 3, "meshfiles/cube.facet");

	while (running)
	{
		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_QUIT: running = false; break;
			}
		}

		peanut::render(cam);
	}

	SDL_Quit();

	return 0;
}