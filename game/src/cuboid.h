#pragma once

#include "rigidbody.h"

namespace psim {

	class Cuboid : public Shape
	{
	private:
		Vector3f center;
		Vector3f dim;

	public:
		Cuboid(Vector3f center = Vector3f{ 0, 0, 0 }, Vector3f dim = Vector3f{ 1, 1, 1 }) 
			: Shape(ShapeType::CUBOID, RED), center(center), dim{ dim } 
		{
		};

		void draw();
		bool checkCollision(RigidBody& other);
		void onCollision();


	};

}


