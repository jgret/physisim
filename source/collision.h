#pragma once

#include "rigidbody.h"
#include "vector3f.h"
#include "sphere.h"
#include "cuboid.h"

namespace psim {

	class Collision
	{

	public:

		static bool checkCollision(RigidBody& a, RigidBody& b, Vector3f& normal, float& depth);

		static bool checkSphereSphere(Sphere& a, Sphere& b, Vector3f& normal, float& depth);
		static bool checkSphereCuboid(Sphere& a, Cuboid& b, Vector3f& normal, float& depth);
		static bool checkCuboidCuboid(Cuboid& a, Cuboid& b, Vector3f& normal, float& depth);

	};

}
