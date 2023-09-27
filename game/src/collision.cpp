#include "collision.h"

using namespace psim;

bool psim::Collision::checkCollision(RigidBody& a, RigidBody& b, Vector3f& normal, float& depth)
{

	switch (a.getShapeType()) {

	case SPHERE:

		switch (b.getShapeType()) {

		case SPHERE:
			return checkSphereSphere((Sphere&) a.getShape(), (Sphere&) b.getShape(), normal, depth);

		}

		break;


	}

	return false;
}

bool Collision::checkSphereSphere(Sphere &a, Sphere &b, Vector3f &normal, float &depth) {

	bool collision = false;
	normal = Vector3f::ZERO;
	depth = 0;

	/*
	* to check if two spheres collide simple calulate the
	* distance between their center positions (a). If this distance
	* is smaller than the sum of their radii (b), the two spheres
	* collide
	*
	* Optimization: compare the square of the distance so no sqrt()
	* call is needed
	*/

	Vector3f c = a.getPos() - b.getPos();
	float distance = c.mag2();

	float radii = a.getRadius() + b.getRadius();
	radii = radii * radii;

	if (distance < radii) {
		collision = true;
		normal = c.normalize();
		depth = radii - distance;
	}

	return collision;
}

bool Collision::checkSphereCuboid(Sphere &a, Cuboid &b, Vector3f &normal, float &depth) {

	bool collision = false;


	return collision;
}

bool Collision::checkCuboidCuboid(Cuboid &a, Cuboid &b, Vector3f &normal, float &depth) {

	bool collision = false;


	return collision;
}
