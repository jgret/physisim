#include "sphere.h"
#include "raylib.h"
#include "collision.h"

using namespace psim;

void Sphere::draw(const Vector3f& pos) const
{
	const int rings = 14, slices = 14;
	DrawSphereEx(pos, this->r, rings, slices, this->color);
	DrawSphereWires(pos, this->r, rings, slices, BLACK);
}

float psim::Sphere::getVolume() const
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

void Sphere::onCollision()
{

}

float Sphere::getRadius() const
{
	return this->r;
}