#pragma once

#include "vector3f.h"
#include "rigidbody.h"

namespace psim {

	class Sphere : public Shape {

	private:
		Vector3f p; // center
		float r;	// radius

	public:
		Sphere(Vector3f pos = 0, float r = 0) : Shape(ShapeType::SPHERE, BLUE), p(pos), r(r)
		{
		};

		void draw();
		bool checkCollision(RigidBody &other);
		void onCollision();

		Vector3f &getPos();
		float getRadius();

	};

}