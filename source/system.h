#pragma once

#include <vector>
#include "rigidbody.h"
#include "rk4solver.h"

namespace psim {

	StateVector system_dydt(float t, StateVector& y);

	class System
	{

	private:
		std::vector<psim::RigidBody*> objects;

	public:

		System();

		void addRigidBody(psim::RigidBody* b);
		psim::RigidBody* removeRigidBody(psim::RigidBody* b);
		psim::RigidBody* removeRigidBodyById(int id);
		
		void clearForces();
		void applyForce(psim::Vector3f f);
		void applyGravity();
		void checkCollision();
		void resolveCollision(RigidBody* pObj1, RigidBody* pObj2, Vector3f& normal, float depth);
		void calculateImpulse(RigidBody* pObj1, RigidBody* pObj2, Vector3f& point, Vector3f& normal);
		void step(float time);

		StateVector getStateVector();
		void update(StateVector& y);


		RigidBody* raycastSelect(Ray &ray);
		RigidBody* raycastSelect(Ray& ray, Vector3f &contactPoint);

		float getTotalEnergy();

		std::vector<psim::RigidBody*>& getObjects();


	};

}