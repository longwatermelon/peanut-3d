#pragma once


namespace peanut
{
	class Camera
	{
	public:
		float x{ 0.0f }, y{ 0.0f }, z{ 0.0f };
		float ha{ 0.0f }, va{ 0.0f };

		Camera() = default;

		Camera(float x, float y, float z)
			: x(x), y(y), z(z) {}
	};
}