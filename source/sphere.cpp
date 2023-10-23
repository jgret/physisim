#include "sphere.h"
#include "raylib.h"
#include "collision.h"

using namespace psim;

void Sphere::draw() {
	const int rings = 14, slices = 14;
	DrawSphereEx(this->p, this->r, rings, slices, this->color);
	DrawSphereWires(this->p, this->r, rings, slices, BLACK);
}

float psim::Sphere::getVolume()
{
	return r * r * PI;
}

//bool Sphere::checkCollision(RigidBody &other) {
//
//	bool collision = false;
//	Vector3f normal;
//	float depth;
//
//	switch (other.getShapeType()) {
//	case(ShapeType::SPHERE):
//
//		Sphere &s1 = *this;
//		Sphere &s2 = (Sphere&) other.getShape();
//
//		if (Collision::checkSphereSphere(s1, s2, normal, depth)) {
//			
//			Vector3f move = normal * depth / 2;
//
//			// s1.getPos() += move;
//			// s2.getPos() -= move;
//
//			collision = true;
//
//		}
//
//		break;
//	}
//
//	return collision;
//}

void Sphere::onCollision() {
	
}

Vector3f& psim::Sphere::getPos()
{
	return this->p;
}

float Sphere::getRadius() {
	return this->r;
}