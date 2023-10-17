#pragma once
#include "raylib.h"

namespace psim {

	class Vector3f : public Vector3
	{

	public:
		static const Vector3f ZERO;

		Vector3f(float x = 0, float y = 0, float z = 0);
		Vector3f(const Vector3f &other);
		Vector3f(const Vector3 &other);

		/**
		 * @brief adds two vectors
		 * @param other
		 * @return new vector
		*/
		Vector3f operator+(const Vector3f& other) const;

		/**
		* @brief adds two vectors
		* @param other
		* @return new vector
		*/
		void operator+=(const Vector3f& other);

		/**
		 * @brief subtracts two vectors
		 * @param other
		 * @return new vector
		*/
		Vector3f operator-(const Vector3f& other) const;

		/**
		* @brief subtracts two vectors
		* @param other
		* @return new vector
		*/
		void operator-=(const Vector3f& other);

		/**
		 * @brief computes the scalar product of two vectors
		 * @param other
		 * @return new vector
		*/
		float operator*(const Vector3f& other) const;

		/**
		 * @brief multiplies the vector by factor f
		 * @param f
		 * @return new vector
		*/
		Vector3f operator*(const float& f) const;

		/**
		 * @brief multiplies the vector by factor f
		 * @param f
		*/
		void operator*=(const float& f);

		/**
		 * @brief divides the vector by factor f
		 * @param f
		 * @return new vector
		*/
		Vector3f operator/(const float& f) const;

		/**
		 * @brief devides the vector by factor f
		 * @param f
		*/
		void operator/=(const float& f);

		/**
		 * @brief computes the cross product
		 * @param other
		 * @return new vector
		*/
		Vector3f cross(const Vector3f& other) const;

		/**
		 * @brief calculates the magnitude of the vector
		 * @return 
		*/
		float mag() const;

		/**
		 * @brief calclutes the square of the magnitude
		 * @return 
		*/
		float mag2() const;

		/**
		 * @brief assignment operator
		 * @param other 
		 * @return 
		*/
		Vector3f& operator=(const Vector3f& other);

		/**
		 * @brief normalize the vector to unit length
		 * @return
		*/
		Vector3f normalize() const;

		/**
		 * @brief calculate the angle between this vector and the other
		 * @param other 
		 * @return 
		*/
		float angle(const Vector3f& other);

	};

	Vector3f operator*(const float f, const Vector3f& v);
}
