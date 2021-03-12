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
					// imagine using a projection matrix lmfao
					// use linear interpolation to project onto the screen

					/*
					*                       origp (x, y, z)
					*                        /
					* --------------------projp-------------------- near plane (0, 0, 1)
					*                      /
					*                    cam (0, 0, 0)
					*/

					/* (projp.x - cam.x) / (origp.x - cam.x)
					* 
					*  the ratio between (projp.x - cam.x) / (origp.x - cam.x) is the same as the ratio between (projp.y - cam.y) / (orip.y - cam.y), which is the same as the ratio between (projp.z - cam.z) / (origp.z - cam.z).
					*  this only works because all lines we calculate are linear.
					* 
					*  we can treat camera as position (0, 0, 0) and origp as position (origp.x, origp.y, origp.z) because the point positions have already been adjusted for object position and camera position.
					*  meaning we can remove every value associated with cam in the equation.
					* 
					*  projp.x / origp.x == projp.y / origp.y == projp.z / origp.z
					*  we can treat projp.z as 1 because we define the near plane's z as 1.f
					*  i should not have hard coded that but it doesnt really matter
					*  projp.x / origp.x == 1 / origp.z
					*/

					// wow this is a pretty simple equation
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
						xb, zb
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
						// ratio is the same value for x, y, and z meaning we can get the clipped points' x and y using this ratio
						// percentage of clipped z point value out of the full segment from p0 to p2
						float ratio = (1.f - opg.p0.z) / (opg.p2.z - opg.p0.z);

						// take the ratio and use it as a percentage, multiply it against total length of x and y segments
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

						gfx.draw_filled_triangle(ppg, opg, xb, zb);
					}
					else // 1 point behind near plane
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

						// create two new triangles out of 4 sided clipped shape
						point projp01, projp02;
						projp01 = p01;
						projp02 = p02;

						center_and_scale(projp01, gfx.getw(), gfx.geth());
						center_and_scale(projp02, gfx.getw(), gfx.geth());

						/*
						*     p1 ___________p2
						*        \         /
						*         \       /
						*------projp01---projp02------- (near plane)
						*           \   /
						*		     p0
						*/

						gfx.draw_filled_triangle({ projp01, projp02, ppg.p2 }, { p01, p02, opg.p2 }, xb, zb);
						gfx.draw_filled_triangle({ projp01, ppg.p1, ppg.p2 }, { p01, opg.p1, opg.p2 }, xb, zb);
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