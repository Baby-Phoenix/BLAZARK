#pragma once

#include <random>
#include <glm.hpp>

class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

	static float Range1f(float min, float max)
	{
		float w = max - min;

		return Float() * w - (w / 2);
	}

	//Random points in cube
	static glm::vec3 Range3f(float min, float max)
	{
		float w = max - min;

		return glm::vec3(Float() * w - (w / 2), Float() * w - (w / 2), Float() * w - (w / 2));
	}

	//Random points in circle
	static glm::vec2 Circle2f(float radius)
	{
		float x, y;

		do
		{
			x = Range1f(-radius, radius);
			y = Range1f(-radius, radius);
		} while ((x * x + y * y) > radius * radius);

		return glm::vec2(x, y);
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};