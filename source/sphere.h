#pragma once

#include "vector3f.h"
#include "rigidbody.h"

namespace psim {

	class Sphere : public Shape {

	private:
		float r;	// radius

	public:
		Sphere(Vector3f pos = 0, float r = 0) : Shape(ShapeType::SPHERE, BLUE), r(r)
		{
		};

		virtual void draw(const Vector3f& pos) const;
		float getVolume() const;
		void onCollision();

		float getRadius() const;

	};

}