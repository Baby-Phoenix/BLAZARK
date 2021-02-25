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

	static bool CheckBetween(float num, float min, float max) {

		return (min <= num && max >= num);
	}

	static bool CheckBetween(glm::vec2 num, glm::vec2 min, glm::vec2 max) {

		bool x = CheckBetween(num.x, min.x, max.x);
		bool y = CheckBetween(num.y, min.y, max.y);

		return (x && y);
	}

	static bool CheckBetween(glm::vec3 num, glm::vec3 min, glm::vec3 max) {

		bool x = CheckBetween(num.x, min.x, max.x);
		bool y = CheckBetween(num.y, min.y, max.y);
		bool z = CheckBetween(num.z, min.z, max.z);

		return (x && y && z);
	}

	static glm::vec3 GetPointBetween(glm::vec3 refrence, float radius, bool inside = false) {
		
		if (!inside) {
			auto tempPoint = OutsideCircle2f(radius);

			return glm::vec3(tempPoint.x + refrence.x, refrence.y, tempPoint.y + refrence.z);
		}

		else
		{
			auto tempPoint = Circle2f(radius);

			return glm::vec3(tempPoint.x + refrence.x, refrence.y, tempPoint.y + refrence.z);
		}

		return glm::vec3(0, 0, 0);

	}

	static glm::vec3 GetPointBetween(glm::vec2 from, glm::vec2 to) {

		float x = Range1f(from.x, to.x);
		float z = Range1f(from.y, to.y);

		return glm::vec3(x, 0, z);
	}

	static float Range1f(float min, float max)
	{
		float w = max - min;

		return Float() * w + min;
	}

	//Random vec3
	static glm::vec3 Range3f(float min, float max)
	{
		float w = max - min;

		return glm::vec3(Float() * w + min, Float() * w + min, Float() * w + min);
	}

	//Random vec3 only in the y is 0
	static glm::vec3 Range2f(float min, float max)
	{
		float w = max - min;

		return glm::vec3(Float() * w - (w / 2), 0, Float() * w - (w / 2));
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

	static glm::vec2 OutsideCircle2f(float radius) {
		float x, y;

		do
		{
			x = Range1f(-radius - 1, radius + 1);
			y = Range1f(-radius - 1, radius + 1);
		} while ((x * x + y * y) < radius * radius);

		return glm::vec2(x, y);
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};