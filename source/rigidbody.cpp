#include "rigidbody.h"
#include "collision.h"

static float velEpsilon = 0.0f; // smallest velocity. Objects with less velocity are considered "at rest"

psim::RigidBody::~RigidBody()
{
	delete this->shape;
}

void psim::RigidBody::init() {
	this->acc = psim::Vector3f{ 0, 0, 0 };
	this->vel = psim::Vector3f{ 0, 0, 0 };
}

void psim::RigidBody::update(float fElapsedTime) {

	if (this->shape->getType() == SPHERE) {

		Sphere* s = (Sphere*) this->shape;

		if (this->getPos().y - s->getRadius() <= 0) {
		
			if (this->vel.mag() < velEpsilon) {
				this->vel.y = 0;
			}
			else {
				this->pos.y = s->getRadius();
				this->vel.y *= -0.5;
			}

		}

	}

	// v = integral a dt
	this->vel += this->acc * fElapsedTime;

	// s = integral v dt
	this->pos += this->vel * fElapsedTime;

}

bool psim::RigidBody::checkCollision(RigidBody& other) {

	bool collision = false;
	Vector3f normal;
	float depth;

	if (Collision::checkCollision(*this, other, normal, depth)) {
		Vector3f move = normal * depth / 2;

		this->getPos() += move;
		other.getPos() -= move;

		collision = true;
	}

	return collision;
}

void psim::RigidBody::draw() {
	DrawLine3D(pos, pos + vel, BLUE);
	this->shape->draw();
}

void psim::RigidBody::applyForce(psim::Vector3f force) {
	this->acc += force / this->mass;
}

void psim::RigidBody::addAcceleration(psim::Vector3f acc) {
	this->acc += acc;
}

float psim::RigidBody::getMass() {
	return this->mass;
}

psim::Shape &psim::RigidBody::getShape() const
{
	return *this->shape;
}

psim::Vector3f& psim::RigidBody::getPos() {
	return this->pos;
}

psim::Vector3f& psim::RigidBody::getVel() {
	return this->vel;
}

psim::Vector3f& psim::RigidBody::getAcc() {
	return this->acc;
}

psim::ShapeType psim::RigidBody::getShapeType() const {
	return this->shape->getType();
}

void psim::RigidBody::showVectors(bool b) {
	this->drawVectors = b;
}

bool psim::RigidBody::isShowVectors() {
	return this->drawVectors;
}

void psim::RigidBody::setColor(Color c) {
	this->color = c;
	this->shape->setColor(c);
}

