#pragma once

#include "vector3f.h"
#include "rigidbody.h"

namespace psim {

	class Sphere : public Shape {

	private:
		float r;	// radius

	public:
		Sphere(float r = 0);
		~Sphere();

		virtual void draw(const Vector3f& pos) const;
		float getVolume() const;

		float getRadius() const;

		virtual BoundingBox getAABB() const;

		virtual Matrix3x3 computeInertiaTensor() const;

	};

}