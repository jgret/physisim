#include <cmath>
#include <iostream>
#include "rigidbody.h"
#include "raymath.h"
#include "collision.h"

int psim::RigidBody::nextId = 0;

psim::RigidBody::RigidBody(Shape* shape) : RigidBody(Vector3f::ZERO, shape, 1, 0.7, RED, true)
{
};

psim::RigidBody::RigidBody(const Vector3f& position, Shape* shape, const float density, const float restitution, const Color& color, bool drawVectors) 
	: pos(position), shape(shape), linearMomentum(0), density(density), restitution(restitution), color(color), drawVectors(drawVectors)
{
	this->id = nextId++;
	this->damping = AIR_DAMPING;
	this->mass = shape->getVolume() * this->density;

	this->rotation = QuaternionFromMatrix(MatrixRotateXYZ(Vector3f{ PI / 2.0f, 0.0f, 0.0f }));

	if (mass == 0) // don't allow zero mass
	{
		std::cout << "RigidBody has zero mass" << std::endl;
		mass = 1;
	}

	if (shape->getType() == SPHERE)
	{

		Sphere& s = static_cast<Sphere&>(*shape);
		float r = s.getRadius();

		inertia = 2.0f / 5.0f * mass * r * r;

    }
    else
    {
        inertia = 1;
    }

	inertiaTensor = shape->computeInertiaTensor() * mass;

    this->shape->transform(getTransform());
}

psim::RigidBody::~RigidBody()
{
	delete this->shape;
}

void psim::RigidBody::init() {
	this->acc = psim::Vector3f::ZERO;
	this->vel = psim::Vector3f::ZERO;
}

float angle;

void psim::RigidBody::update(float fElapsedTime) {

	//if (this->shape->getType() == SPHERE) 
	//{

	//	Sphere* s = static_cast<Sphere*>(shape);
	//	float y = this->getPos().y;
	//	float radius = s->getRadius();

	//	if (y - radius <= 0) {
	//	
	//		this->pos.y = radius;
	//		this->vel.y *= -this->restitution;

	//	}

	//}
	//else if (this->shape->getType() == CUBOID)
	//{

	//	Cuboid* c = static_cast<Cuboid*>(shape);

	//}

}

void psim::RigidBody::draw() {
	DrawLine3D(pos, pos + vel, BLUE);
	DrawLine3D(pos, pos + omega, RED);
	DrawLine3D(pos, pos + acc, GREEN);
	this->shape->draw(pos);
}

void psim::RigidBody::clearForces()
{
	acc = Vector3f::ZERO;
	force = Vector3f::ZERO;
	torque = Vector3f::ZERO;
}

void psim::RigidBody::applyForce(const psim::Vector3f &force) {
	this->acc += force / this->mass;
}

void psim::RigidBody::applyForce(const psim::Vector3f& force, const psim::Vector3f& p)
{
	applyForce(force);
	this->torque += p.cross(force);
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

const psim::Vector3f& psim::RigidBody::getForce() const
{
	return this->force;
}

const psim::Vector3f& psim::RigidBody::getTorque() const
{
	return this->torque;
}

const psim::Vector3f& psim::RigidBody::getLinearMomentum() const
{
	return this->linearMomentum;
}

const psim::Vector3f& psim::RigidBody::getAngularMomentum() const
{
	return this->angularMomentum;
}

const float psim::RigidBody::getInertia() const
{
	return inertia;
}

Matrix psim::RigidBody::getTransform() const
{
	Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix rotationMatrix = QuaternionToMatrix(rotation);
	Matrix translationMatrix = MatrixTranslate(pos.x, pos.y, pos.z);
	return MatrixMultiply(MatrixMultiply(matScale, rotationMatrix), translationMatrix);
}

psim::Vector3f psim::RigidBody::getVelAtPoint(const Vector3f& p) const
{
	return (vel + omega.cross(p));
}

BoundingBox psim::RigidBody::getAABB() const
{
	BoundingBox box = shape->getAABB();

	// translate the position of the box
	box.min += pos;
	box.max += pos;

	return box;

}
psim::ShapeType psim::RigidBody::getShapeType() const {
	return this->shape->getType();
}

float psim::RigidBody::getTotalEnergy()
{
	// 0.5 * mv² + 0.5 * Iw² + mgh
	return 0.5f * mass * vel.mag2() + 0.5f * inertia * omega.mag2()
		 + mass * 9.81f * pos.y;
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

int psim::RigidBody::appendToStateVector(StateVector& y, int idx)
{
	// position
	y[idx++] = pos.x;
	y[idx++] = pos.y;
	y[idx++] = pos.z;

	// velocity
	y[idx++] = vel.x;
	y[idx++] = vel.y;
	y[idx++] = vel.z;

	// rotation
	y[idx++] = rotation.x;
	y[idx++] = rotation.y;
	y[idx++] = rotation.z;
	y[idx++] = rotation.w;

	// omega
	y[idx++] = omega.x;
	y[idx++] = omega.y;
	y[idx++] = omega.z;

	// angular momentum
	y[idx++] = angularMomentum.x;
	y[idx++] = angularMomentum.y;
	y[idx++] = angularMomentum.z;

	return idx;
}

int psim::RigidBody::updateFromStateVector(const StateVector& y, int idx)
{

	// position
	pos.x = y[idx++];
	pos.y = y[idx++];
	pos.z = y[idx++];

	// velocity
	vel.x = y[idx++];
	vel.y = y[idx++];
	vel.z = y[idx++];

	// rotation
	rotation.x = y[idx++];
	rotation.y = y[idx++];
	rotation.z = y[idx++];
	rotation.w = y[idx++];

	// omega
	omega.x = y[idx++];
	omega.y = y[idx++];
	omega.z = y[idx++];

	// angular momentum
	angularMomentum.x = y[idx++];
	angularMomentum.y = y[idx++];
	angularMomentum.z = y[idx++];

	this->linearMomentum = mass * vel;
	this->force = mass * acc;
	this->omega = angularMomentum / inertia;

	this->shape->transform(getTransform());

	return idx;
}

psim::Vector3f psim::getLocalPosition(const Vector3f& origin, const Vector3f& p)
{
	return p - origin;
}

psim::Vector3f psim::getWorldPosition(const Vector3f& origin, const Vector3f lp)
{
	return origin + lp;
}
