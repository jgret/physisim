#pragma once

#include "rigidbody.h"

namespace psim {

	class Cuboid : public Shape
	{
	private:
		Vector3f dim;

	public:
		Cuboid(Vector3f dim = Vector3f{ 1, 1, 1 });
		~Cuboid();

		void draw(const Vector3f& pos) const;
		float getVolume() const;

		virtual Matrix3x3 computeInertiaTensor() const;
	};

}


