#pragma once
#include "utils.h"
#include "graphics.h"
#include "camera.h"
#include <string>


namespace peanut::peautils
{
	class Object
	{
	public:
		Object(float x, float y, float z, const char* path)
			: x(x), y(y), z(z)
		{
			m = peautils::read_facet(path);
		}


		void project(Graphics& gfx, matrix4& proj, Camera& cam)
		{
			std::vector<point> projpoints(m.points.size());
			std::vector<point> origpoints(m.points.size());

			for (int i = 0; i < m.points.size(); ++i)
			{
				point origp = m.points[i], projp;

				// adjust for cam and object position
				origp.x += x + cam.x;
				origp.y += y + cam.y;
				origp.z += z + cam.z;

				origpoints[i] = origp;

				// projection
				matrix4_multiply(origp, projp, proj);

				// center and scale
				projp.x += 1.0f;
				projp.y += 1.0f;

				projp.x *= 0.5f * gfx.getw();
				projp.y *= 0.5f * gfx.geth();

				projpoints[i] = projp;
			}

			SDL_SetRenderDrawColor(gfx.getrend(), 255, 255, 255, 255);

			for (auto& t : m.tris)
			{
				
				gfx.draw_wireframe_triangle(projpoints[t.indexes[0]], projpoints[t.indexes[1]], projpoints[t.indexes[2]]);
			}

			SDL_SetRenderDrawColor(gfx.getrend(), 0, 0, 0, 255);
		}

	private:
		float x, y, z;
		peautils::mesh m;
	};
}