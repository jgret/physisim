#pragma once

#include "vector3f.h"
#include "shape.h"
#include "physic_constant.h"

namespace psim {

	class RigidBody
	{

	private:
		static int nextId;

		int id;
		bool drawVectors;
		Shape *shape;

	protected:
		Color color;
		Vector3f pos;
		Vector3f vel;
		Vector3f acc;

		float mass;
		float density;
		float restitution;
		float damping;

	public:

		RigidBody(Shape *shape);

		RigidBody(const Vector3f &position, Shape *shape, const float density = 1, const float restitution = DEFAULT_RESTITIUTION, const Color &color = RED,  bool drawVectors = true);

		~RigidBody();

		/**
		 * @brief called once to initialize object
		*/
		virtual void init();

		/**
		 * @brief update the objects physics
		 * @param fElapsedTime
		*/
		virtual void update(float fElapsedTime);

		/**
		 * @brief draw the object on the screen
		*/
		virtual void draw();

		/**
		 * @brief clear all forces on object. sets acc to 0
		*/
		void clearForces();

		/**
		 * @brief apply force to rigid body
		 * @param force
		*/
		void applyForce(Vector3f force);

		/**
		 * @brief add acceleration to the rigid body
		 * @param acc 
		*/
		void addAcceleration(Vector3f acc);

		/**
		 * @brief calculates the total energy of the object
		 * @return 
		*/
		float getTotalEnergy();

		void showVectors(bool b);

		bool isShowVectors();
		
		float getMass();

		float getRestitution();

		float getDamping();

		void setDamping(float d);

		void setRestitution(const float r);
		
		Shape &getShape() const;

		ShapeType getShapeType() const;
		
		Vector3f& getPos();
		
		Vector3f& getVel();
		
		Vector3f& getAcc();

		void setColor(Color c);

		int getId();


		bool operator==(const RigidBody& other) const
		{
			return this->id == other.id;
		}
};

}