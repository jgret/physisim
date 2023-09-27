#include <cmath>
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
		
			this->pos.y = s->getRadius();
			this->vel.y *= -this->restitution;

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

		this->resolveCollision(other, normal, depth);
		this->calculateVelocities(other, normal);
		collision = true;
	}

	return collision;
}


void psim::RigidBody::draw() {
	DrawLine3D(pos, pos + vel, BLUE);
	this->shape->draw();
}

void psim::RigidBody::resolveCollision(RigidBody& other, const Vector3f& normal, const float depth)
{
	Vector3f move = normal * depth / 2;

	this->getPos() += move;
	other.getPos() -= move;
}

void psim::RigidBody::calculateVelocities(RigidBody& other, const Vector3f& n)
{
	float v1bn = this->vel * n;
	float v2bn = other.vel * n;

	psim::Vector3f v1a; // new velocity for pBody
	psim::Vector3f v2a; // new velocity for pOther

	
	float m1 = this->mass;
	float m2 = other.mass;

	float e = std::fminf(this->restitution, other.restitution);

	float v1an = (m1 * v1bn + m2 * v2bn + m2 * e * (v2bn - v1bn)) / (m1 + m2);
	float v2an = (m1 * v1bn + m2 * v2bn - m2 * e * (v2bn - v1bn)) / (m1 + m2);

	v1a = n * v1an;
	v2a = n * v2an;

	this->vel = v1a;
	other.vel = v2a;
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

