#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <iostream>


namespace peanut::peautils
{
	struct point
	{
		float x, y, z;
	};

	struct triangle
	{
		int indexes[3];
	};

	struct mesh
	{
		std::vector<point> points;
		std::vector<triangle> tris;
	};

	struct matrix4
	{
		float m[4][4];
	};

	struct matrix3
	{
		float m[3][3];
	};

	struct pointgrp
	{
		point p0, p1, p2;
	};

	struct boundsarray
	{
		std::vector<float> l{ 0 }, r{ 0 };
	};


	mesh read_facet(const char* path)
	{
		std::fstream f(path);

		std::stringstream ss;
		std::string line;

		for (int i = 0; i < 5; ++i) std::getline(f, line);

		int point_num = -1;
		std::stringstream(line) >> point_num;

		double minx = 1e10, miny = 1e10, minz = 1e10, maxx = -1e10, maxy = -1e10, maxz = -1e10;

		std::vector<point> points;
		for (int i = 0; i < point_num; ++i)
		{
			std::getline(f, line);

			std::stringstream ss(line);

			double arr[3];
			for (int j = 0; j < 3; ++j)
			{
				double num;
				ss >> num;

				arr[j] = num;
			}

			if (arr[0] < minx) minx = arr[0];
			if (arr[0] > maxx) maxx = arr[0];

			if (arr[1] < miny) miny = arr[1];
			if (arr[1] > maxy) maxy = arr[1];

			if (arr[2] < minz) minz = arr[2];
			if (arr[2] > maxz) maxz = arr[2];

			points.push_back({ (float)arr[0], (float)arr[1], (float)arr[2] });
		}

		for (int i = 0; i < 3; ++i) std::getline(f, line);

		int tri_num = -1;
		std::stringstream(line) >> tri_num;

		std::vector<triangle> tris;
		for (int i = 0; i < tri_num; ++i)
		{
			std::getline(f, line);

			std::stringstream ss(line);

			triangle temp;
			for (int j = 0; j < 3; ++j)
			{
				int num;
				ss >> num;

				temp.indexes[j] = num - 1;
			}

			tris.push_back(temp);
		}

		f.close();

		return { points, tris };
	}


	void matrix4_multiply(point& i, point& o, matrix4& m)
	{
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
		{
			o.x /= w;
			o.y /= w;
			o.z /= w;
		}
	}

	point matrix3_multiply(matrix3& mat, point& p)
	{
		point ret;

		ret.x = p.x * mat.m[0][0] + p.y * mat.m[1][0] + p.z * mat.m[2][0];
		ret.y = p.x * mat.m[0][1] + p.y * mat.m[1][1] + p.z * mat.m[2][1];
		ret.z = p.x * mat.m[0][2] + p.y * mat.m[1][2] + p.z * mat.m[2][2];

		return ret;
	}


	matrix4 make_projection(float fov, float ratio, float near, float far)
	{
		float fovrad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.1415f);

		matrix4 mat = { {
			{ratio * fovrad, 0, 0, 0},
			{0, fovrad, 0, 0},
			{0, 0, far / (far - near), 1.0f},
			{0, 0, (-far * near) / (far - near), 0}
		} };

		return mat;
	}


	void swap_points(point& p1, point& p2)
	{
		point temp = p2;
		p2 = p1;
		p1 = temp;
	}


	void interpolate(point p0, point p1, std::vector<float>& vx, std::vector<float>& vz, int w)
	{
		float ystart, yend;
		if ((int)p0.y < (int)p1.y)
		{
			float slope = (p1.x - p0.x) / (p1.y - p0.y);

			if (p0.y < 0.0f) ystart = 0.f;
			else ystart = p0.y;

			if (p1.y > 1000.0f) yend = 1000.f;
			else yend = p1.y;

			for (float y = ystart; y < yend; ++y)
			{
				float x = p0.x + (slope * (y - p0.y));

				if (x < 0) x = 0;
				if (x > 1000) x = 1000;

				vx[(int)y] = x;
				vz[(int)y] = p0.z + (y - p0.y) * ((p1.z - p0.z) / (p1.y - p0.y));
			}
		}
	}


	point normalize_vector(point p)
	{
		float len = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);

		p.x /= len;
		p.y /= len;
		p.z /= len;

		return p;
	}


	point triangle_normal_vector(pointgrp pg)
	{
		point v, w, n;
		v.x = pg.p1.x - pg.p0.x;
		v.y = pg.p1.y - pg.p0.y;
		v.z = pg.p1.z - pg.p0.z;

		w.x = pg.p2.x - pg.p0.x;
		w.y = pg.p2.y - pg.p0.y;
		w.z = pg.p2.z - pg.p0.z;

		n.x = (v.y * w.z) - (v.z * w.y);
		n.y = (v.z * w.x) - (v.x * w.z);
		n.z = (v.x * w.y) - (v.y * w.x);

		n = normalize_vector(n);

		return n;
	}

}