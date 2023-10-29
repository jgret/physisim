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
		static bool checkShperes(const Vector3f& posA, const Sphere& a, const Vector3f& posB, const Sphere& b, Vector3f& contactPoint, Vector3f& normal, float& depth);
	};

}
