#pragma once

#include <vector>
#include "rigidbody.h"

namespace psim {

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

		RigidBody* raycastSelect(Ray &ray);

		float getTotalEnergy();

		std::vector<psim::RigidBody*>& getObjects();


	};

}