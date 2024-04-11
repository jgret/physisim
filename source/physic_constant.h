#pragma once
#include "vector3f.h"

namespace psim
{

	static const psim::Vector3f GRAVITY{ 0, -9.81f, 0 };
	static const float AIR_DAMPING = 1.0f;
	static const float SPRING_DAMPING = 10.0f;
	static const float DEFAULT_RESTITIUTION = 0.7f;

}
