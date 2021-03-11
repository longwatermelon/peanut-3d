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


		void project(Graphics& gfx, matrix4& proj, matrix3& rotx, matrix3& roty, Camera& cam)
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

				// rotation
				origp = matrix3_multiply(roty, origp);
				origp = matrix3_multiply(rotx, origp);

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

			gfx.reset_texbuf();

			for (auto& t : m.tris)
			{
				if (origpoints[t.indexes[0]].z > 1.0f || origpoints[t.indexes[1]].z > 1.0f || origpoints[t.indexes[2]].z > 1.0f)
				{
					gfx.draw_filled_triangle(
						{ projpoints[t.indexes[0]], projpoints[t.indexes[1]], projpoints[t.indexes[2]] },
						{ origpoints[t.indexes[0]], origpoints[t.indexes[1]], origpoints[t.indexes[2]] },
						xb, zb,
						{ (Uint8)((t.indexes[0] * 100) % 255), (Uint8)((t.indexes[1] * 100) % 255), (Uint8)((t.indexes[2] * 100) % 255) }
					);
				}
			}

			gfx.update_texture();
		}

	private:
		float x, y, z;
		boundsarray xb;
		boundsarray zb;

		peautils::mesh m;
	};
}