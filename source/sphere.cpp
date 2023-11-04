#include "sphere.h"
#include "raylib.h"
#include "raymath.h"
#include "collision.h"

using namespace psim;

psim::Sphere::Sphere(float r) : Shape(ShapeType::SPHERE, BLUE), r(r)
{
	const int rings = 7;
	const int slices = 14;
	Mesh mesh = GenMeshSphere(r, rings, slices);
	this->model = LoadModelFromMesh(mesh);
}
psim::Sphere::~Sphere()
{
	UnloadModel(model);
}

void Sphere::draw(const Vector3f& pos) const
{
	//const int rings = 14, slices = 14;
	//DrawSphereEx(pos, this->r, rings, slices, this->color);
	//DrawSphereWires(pos, this->r, rings, slices, BLACK);

	DrawModel(model, Vector3f::ZERO, 1, color);
	DrawModelWires(model, Vector3f::ZERO, 1, BLACK);
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

float Sphere::getRadius() const
{
	return this->r;
}

BoundingBox psim::Sphere::getAABB() const
{
	BoundingBox box;
	box.min = Vector3f{ -r, -r, -r };
	box.max = Vector3f{ r, r, r };
	return box;
}

Matrix3x3 psim::Sphere::computeInertiaTensor() const
{

	float i = 2.0f / 5.0f * r * r;

	return Matrix3x3
	{
		i, 0, 0,
		0, i, 0,
		0, 0, i,
	};
}
