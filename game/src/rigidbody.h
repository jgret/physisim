#pragma once

#include "vector3f.h"
#include "shape.h"

namespace psim {

	class RigidBody
	{

	private:
		bool drawVectors;
		Shape *shape;

	protected:
		Color color;
		Vector3f &pos;
		Vector3f vel;
		Vector3f acc;

		float mass;

	public:

		RigidBody(Shape *shape) : shape(shape), pos(shape->getPos()) {
			this->mass = 1;
			this->drawVectors = true;
			this->color = RED;
		};

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
		 * @brief check if objects overlaps the other
		 * @param other 
		 * @return 
		*/
		virtual bool checkCollision(RigidBody& other);

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

		void showVectors(bool b);

		bool isShowVectors();
		
		float getMass();
		
		Shape &getShape() const;

		ShapeType getShapeType() const;
		
		Vector3f& getPos();
		
		Vector3f& getVel();
		
		Vector3f& getAcc();

		void setColor(Color c);

	};

}