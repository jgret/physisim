#include "cuboid.h"
#include "collision.h"

void psim::Cuboid::draw() {
	DrawCubeV(center, dim, this->color);
	DrawCubeWiresV(center, dim, MAROON);
}

float psim::Cuboid::getVolume()
{
	return dim.x * dim.y * dim.z;
}

void psim::Cuboid::onCollision() {
	
}

bool psim::Cuboid::checkCollision(RigidBody &other) {

	bool collision = false;

	switch (other.getShapeType()) {

	case(ShapeType::SPHERE):

		break;


	}

	return collision;
}