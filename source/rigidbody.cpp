#include <cmath>
#include "rigidbody.h"
#include "collision.h"

int psim::RigidBody::nextId = 0;

psim::RigidBody::~RigidBody()
{
	delete this->shape;
}

void psim::RigidBody::init() {
	this->acc = psim::Vector3f::ZERO;
	this->vel = psim::Vector3f::ZERO;
}

void psim::RigidBody::update(float fElapsedTime) {

	if (this->shape->getType() == SPHERE) {

		Sphere* s = (Sphere*) this->shape;

		if (this->getPos().y - s->getRadius() <= 0) {
		
			this->pos.y = s->getRadius();
			this->vel.y *= -this->restitution;

		}

	}

	pos += vel * fElapsedTime;

	vel += (acc - (vel * damping) ) * fElapsedTime;

}

void psim::RigidBody::draw() {
	DrawLine3D(pos, pos + vel, BLUE);
	this->shape->draw();
}

void psim::RigidBody::clearForces()
{
	acc = Vector3f::ZERO;
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

float psim::RigidBody::getRestitution()
{
	return this->restitution;
}

float psim::RigidBody::getDamping()
{
	return this->damping;
}

void psim::RigidBody::setDamping(float d)
{
	this->damping = d;
}

void psim::RigidBody::setRestitution(const float r)
{
	this->restitution = r;
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

float psim::RigidBody::getTotalEnergy()
{
	// Ekin + Epot
	return this->mass * this->vel.mag2() * 0.5f + this->mass * 9.81 * this->pos.y;
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

int psim::RigidBody::getId()
{
	return this->id;
}

