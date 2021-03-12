#pragma once
#include "utils.h"
#include "graphics.h"
#include "camera.h"
#include <string>


namespace peanut
{
	using namespace peautils;

	class Object
	{
	public:
		Object(float x, float y, float z, const char* path)
			: x(x), y(y), z(z)
		{
			m = read_facet(path);
		}


		void project(Graphics& gfx, matrix3& rotx, matrix3& roty, Camera& cam)
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
				
				if (origp.z != 0.f)
				{
					projp.x = origp.x / origp.z;
					projp.y = origp.y / origp.z;
				}

				// center and scale
				center_and_scale(projp, gfx.getw(), gfx.geth());

				projpoints[i] = projp;
			}


			for (auto& t : m.tris)
			{
				if (origpoints[t.indexes[0]].z >= 1.0f && origpoints[t.indexes[1]].z >= 1.0f && origpoints[t.indexes[2]].z >= 1.0f)
				{
					gfx.draw_filled_triangle(
						{ projpoints[t.indexes[0]], projpoints[t.indexes[1]], projpoints[t.indexes[2]] },
						{ origpoints[t.indexes[0]], origpoints[t.indexes[1]], origpoints[t.indexes[2]] },
						xb, zb,
						{ 255, 255, 255 }
					);
				}
				else if (origpoints[t.indexes[0]].z >= 1.0f || origpoints[t.indexes[1]].z >=1.0f || origpoints[t.indexes[2]].z >= 1.0f)
				{
					pointgrp opg{ origpoints[t.indexes[0]], origpoints[t.indexes[1]], origpoints[t.indexes[2]] };
					pointgrp ppg{ projpoints[t.indexes[0]], projpoints[t.indexes[1]], projpoints[t.indexes[2]] };

					if (opg.p1.z < opg.p0.z)
					{
						swap_points(ppg.p1, ppg.p0);
						swap_points(opg.p1, opg.p0);
					}

					if (opg.p2.z < opg.p0.z)
					{
						swap_points(ppg.p2, ppg.p0);
						swap_points(opg.p2, opg.p0);
					}

					if (opg.p2.z < opg.p1.z)
					{
						swap_points(ppg.p2, ppg.p1);
						swap_points(opg.p2, opg.p1);
					}

					if (opg.p1.z < 1.f)
					{
						float ratio = (1.f - opg.p0.z) / (opg.p2.z - opg.p0.z);

						opg.p0.x += (opg.p2.x - opg.p0.x) * ratio;
						opg.p0.y += (opg.p2.y - opg.p0.y) * ratio;
						opg.p0.z = 1.f;

						ratio = (1.f - opg.p1.z) / (opg.p2.z - opg.p1.z);

						opg.p1.x += (opg.p2.x - opg.p1.x) * ratio;
						opg.p1.y += (opg.p2.y - opg.p1.y) * ratio;
						opg.p1.z = 1.f;

						ppg.p0 = opg.p0;
						ppg.p1 = opg.p1;

						center_and_scale(ppg.p0, gfx.getw(), gfx.geth());
						center_and_scale(ppg.p1, gfx.getw(), gfx.geth());

						gfx.draw_filled_triangle(ppg, opg, xb, zb, { 255, 255, 255 });
					}
					else
					{
						point p01, p02;

						float ratio = (1.f - opg.p0.z) / (opg.p1.z - opg.p0.z);

						p01.x = opg.p0.x + (opg.p1.x - opg.p0.x) * ratio;
						p01.y = opg.p0.y + (opg.p1.y - opg.p0.y) * ratio;
						p01.z = 1.f;

						ratio = (1.f - opg.p0.z) / (opg.p2.z - opg.p0.z);

						p02.x = opg.p0.x + (opg.p2.x - opg.p0.x) * ratio;
						p02.y = opg.p0.y + (opg.p2.y - opg.p0.y) * ratio;
						p02.z = 1.f;

						point projp01, projp02;
						projp01 = p01;
						projp02 = p02;

						center_and_scale(projp01, gfx.getw(), gfx.geth());
						center_and_scale(projp02, gfx.getw(), gfx.geth());

						gfx.draw_filled_triangle({ projp01, projp02, ppg.p2 }, { p01, p02, opg.p2 }, xb, zb, { 255, 255, 255 });
						gfx.draw_filled_triangle({ projp01, ppg.p1, ppg.p2 }, { p01, opg.p1, opg.p2 }, xb, zb, { 255, 255, 255 });
					}
				}
			}

		}

	private:
		float x, y, z;
		boundsarray xb;
		boundsarray zb;

		mesh m;
	};
}