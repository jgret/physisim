#pragma once

#include <vector>
#include "rigidbody.h"
#include "../solver/rk4solver.h"
#include "force_object.h"

namespace psim {

	

	class System
	{

	private:
		std::vector<psim::RigidBody*> objects;
		std::vector<ForceObject*> forceObjs;

	public:

		System();
		~System();

		void addRigidBody(psim::RigidBody* b);
		void addForceObject(ForceObject* force);
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

		StateVector system_dydt(float t, const StateVector& y);

	};

}