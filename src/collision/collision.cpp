#include "collision.h"

using namespace psim;

bool psim::Collision::checkCollision(RigidBody& a, RigidBody& b, Vector3f& contactPoint, Vector3f& normal, float& depth)
{

	switch (a.getShapeType()) {

	case SPHERE:

		switch (b.getShapeType()) {

		case SPHERE:
			return checkShperes(a.getPos(), static_cast<Sphere&>(a.getShape()), b.getPos(), static_cast<Sphere&>(b.getShape()), contactPoint, normal, depth);

		case CUBOID:
			return false;
		}

		break;


	}

	return false;
}

bool Collision::checkShperes(const Vector3f &posA, const Sphere &a, const Vector3f& posB, const Sphere &b, Vector3f& contactPoint, Vector3f &normal, float &depth) {

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

	Vector3f c = posB - posA;
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

//bool Collision::checkSphereCuboid(Sphere &a, Cuboid &b, Vector3f& contactPoint, Vector3f &normal, float &depth) {
//
//	bool collision = false;
//
//
//	return collision;
//}
//
//bool Collision::checkCuboids(Cuboid &a, Cuboid &b, Vector3f& contactPoint, Vector3f &normal, float &depth) {
//
//	bool collision = false;
//
//
//	return collision;
//}
