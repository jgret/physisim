#pragma once

#include "rigidbody.h"
#include "vector3f.h"
#include "sphere.h"
#include "cuboid.h"

namespace psim {

	class Collision
	{

	public:

		static bool checkCollision(RigidBody& a, RigidBody& b, Vector3f &contactPoint, Vector3f& normal, float& depth);
		static bool checkShperes(Sphere& a, Sphere& b, Vector3f& contactPoint, Vector3f& normal, float& depth);
		static bool checkSphereCuboid(Sphere& a, Cuboid& b, Vector3f& contactPoint, Vector3f& normal, float& depth);
		static bool checkCuboids(Cuboid& a, Cuboid& b, Vector3f& contactPoint, Vector3f& normal, float& depth);

	};

}
