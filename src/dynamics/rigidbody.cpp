#include <cmath>
#include <iostream>
#include "rigidbody.h"
#include "../collision/sphere.h"
#include "raymath.h"

//#include "../collision/collision.h"

int psim::RigidBody::nextId = 0;

psim::RigidBody::RigidBody(Shape* shape) : RigidBody(Vector3f::ZERO, shape, 1, 0.7, RED, true)
{
};

psim::RigidBody::RigidBody(const Vector3f& position, Shape* shape, const float density, const float restitution, const Color& color, bool drawVectors, bool isStatic) 
{

	this->shape = shape;

	data.damping = AIR_DAMPING;
	data.density = density;
	data.restitution = restitution;
	data.pos = position;
	data.mass = shape->getVolume() * data.density;
	data.vel = psim::Vector3f::ZERO;
	data.force = psim::Vector3f::ZERO;

	data.rotation = QuaternionFromMatrix(MatrixRotateXYZ(Vector3f{ PI / 2.0f, 0.0f, 0.0f }));

	if (data.mass == 0) // don't allow zero mass
	{
		std::cout << "RigidBody has zero mass. Set mass to 1" << std::endl;
		data.mass = 1;
	}

	if (shape->getType() == SPHERE)
	{

		Sphere& s = static_cast<Sphere&>(*shape);
		float r = s.getRadius();

		data.inertia = 2.0f / 5.0f * data.mass * r * r;

    }
    else
    {
        data.inertia = 1;
    }

	data.inertiaTensor = shape->computeInertiaTensor() * data.mass;

	this->drawVectors = true;
	this->isStatic = isStatic;
}

psim::RigidBody::RigidBody(RigidBodyData data)
{
	this->id = nextId++;
	this->data = data;

}

psim::RigidBody::~RigidBody()
{
	delete this->shape;
}

void psim::RigidBody::init() {

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
	//		this->data.vel.y *= -this->restitution;

	//	}

	//}
	//else if (this->shape->getType() == CUBOID)
	//{

	//	Cuboid* c = static_cast<Cuboid*>(shape);

	//}

}

void psim::RigidBody::draw() {
	Matrix transform = getTransform();
	this->shape->transform(transform);
	DrawLine3D(data.pos, data.pos + data.vel, BLUE);
	DrawLine3D(data.pos, data.pos + data.omega, RED);
	DrawLine3D(data.pos, data.pos + data.force / data.mass, GREEN);
	this->shape->draw(data.pos);
}

void psim::RigidBody::clearForces()
{
	data.force = Vector3f::ZERO;
	data.torque = Vector3f::ZERO;
}

void psim::RigidBody::applyForce(const psim::Vector3f &force) {
	this->data.force += force;
}

void psim::RigidBody::applyForce(const psim::Vector3f& force, const psim::Vector3f& p)
{
	applyForce(force);
	this->data.torque += p.cross(force);
}

void psim::RigidBody::addAcceleration(psim::Vector3f acc) {
	this->data.force += acc * data.mass;
}

float psim::RigidBody::getMass() {
	return this->data.mass;
}

float psim::RigidBody::getRestitution()
{
	return this->data.restitution;
}

float psim::RigidBody::getDamping()
{
	return this->data.damping;
}

void psim::RigidBody::setDamping(float d)
{
	this->data.damping = d;
}

void psim::RigidBody::setRestitution(const float r)
{
	this->data.restitution = r;
}

psim::Shape &psim::RigidBody::getShape() const
{
	return *this->shape;
}

psim::Vector3f& psim::RigidBody::getPos() {
	return this->data.pos;
}

psim::Vector3f& psim::RigidBody::getVel() {
	return this->data.vel;
}

psim::Vector3f psim::RigidBody::getAcc() {
	return this->data.force / data.mass;
}

const psim::Vector3f& psim::RigidBody::getForce() const
{
	return this->data.force;
}

const psim::Vector3f& psim::RigidBody::getTorque() const
{
	return this->data.torque;
}

psim::Vector3f psim::RigidBody::getLinearMomentum() const
{
	return data.mass * data.vel;
}

const psim::Vector3f& psim::RigidBody::getAngularMomentum() const
{
	return this->data.angularMomentum;
}

const float psim::RigidBody::getInertia() const
{
	return data.inertia;
}

Matrix psim::RigidBody::getTransform() const
{
	Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix rotationMatrix = QuaternionToMatrix(data.rotation);
	Matrix translationMatrix = MatrixTranslate(data.pos.x, data.pos.y, data.pos.z);
	return MatrixMultiply(MatrixMultiply(matScale, rotationMatrix), translationMatrix);
}

psim::Vector3f psim::RigidBody::getVelAtPoint(const Vector3f& p) const
{
	return (data.vel + data.omega.cross(p));
}

BoundingBox psim::RigidBody::getAABB() const
{
	BoundingBox box = shape->getAABB();

	// translate the position of the box
	box.min += data.pos;
	box.max += data.pos;

	return box;

}
psim::ShapeType psim::RigidBody::getShapeType() const {
	return this->shape->getType();
}

float psim::RigidBody::getTotalEnergy()
{
	// 0.5 * mv^2 + 0.5 * Iw^2 + mgh
	return 0.5f * data.mass * data.vel.mag2() + 0.5f * data.inertia * data.omega.mag2()
		 + data.mass * 9.81f * data.pos.y;
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
	y[idx++] = data.pos.x;
	y[idx++] = data.pos.y;
	y[idx++] = data.pos.z;

	// velocity
	y[idx++] = data.vel.x;
	y[idx++] = data.vel.y;
	y[idx++] = data.vel.z;

	// rotation
	y[idx++] = data.rotation.x;
	y[idx++] = data.rotation.y;
	y[idx++] = data.rotation.z;
	y[idx++] = data.rotation.w;

	// omega
	y[idx++] = data.omega.x;
	y[idx++] = data.omega.y;
	y[idx++] = data.omega.z;

	// angular momentum
	y[idx++] = data.angularMomentum.x;
	y[idx++] = data.angularMomentum.y;
	y[idx++] = data.angularMomentum.z;

	return idx;
}

int psim::RigidBody::updateFromStateVector(const StateVector& y, int idx)
{

	// position
	data.pos.x = y[idx++];
	data.pos.y = y[idx++];
	data.pos.z = y[idx++];

	// velocity
	data.vel.x = y[idx++];
	data.vel.y = y[idx++];
	data.vel.z = y[idx++];

	// rotation
	data.rotation.x = y[idx++];
	data.rotation.y = y[idx++];
	data.rotation.z = y[idx++];
	data.rotation.w = y[idx++];

	// omega
	data.omega.x = y[idx++];
	data.omega.y = y[idx++];
	data.omega.z = y[idx++];

	// angular momentum
	data.angularMomentum.x = y[idx++];
	data.angularMomentum.y = y[idx++];
	data.angularMomentum.z = y[idx++];

	this->data.omega = data.angularMomentum / data.inertia;

	// Matrix transform = getTransform();
	// this->shape->transform(transform);

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
