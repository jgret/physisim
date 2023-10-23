#include "vector3f.h"
#include <iostream>
#include <cmath>
#include <raymath.h>

using namespace psim;

const Vector3f Vector3f::ZERO = { 0, 0, 0 };

Vector3f::Vector3f(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
};

Vector3f::Vector3f(const Vector3f& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Vector3f::Vector3f(const Vector3& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Vector3f Vector3f::operator+(const Vector3f& other) const {
	return Vector3f{ 
		this->x + other.x,
		this->y + other.y,
		this->z + other.z 
	};
}

void Vector3f::operator+=(const Vector3f& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
}

Vector3f Vector3f::operator-(const Vector3f& other) const {
	return Vector3f{ 
		this->x - other.x,
		this->y - other.y,
		this->z - other.z 
	};
}

void Vector3f::operator-=(const Vector3f& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
}

float Vector3f::operator*(const Vector3f& other) const {
	return this->x * other.x +
		this->y * other.y +
		this->z * other.z;

}

Vector3f operator*(const float f, const Vector3f& v)
{
	return Vector3f{
		v.x * f,
		v.y * f,
		v.z * f,
	};
}

Vector3f Vector3f::operator*(const float& f) const {
	return Vector3f{
		this->x * f,
		this->y * f,
		this->z * f,
	};
}

void Vector3f::operator*=(const float& f) {
	this->x *= f;
	this->y *= f;
	this->z *= f;
}

Vector3f Vector3f::operator/(const float& f) const {
	return Vector3f{
		this->x / f,
		this->y / f,
		this->z / f
	};
}

void Vector3f::operator/=(const float& f) {
	this->x /= f;
	this->y /= f;
	this->z /= f;
}

Vector3f Vector3f::cross(const Vector3f& other) const {
	return Vector3f{ 
		this->y * other.z - this->z * other.y,
		this->z * other.x - this->x * other.z,
		this->x * other.y - this->y * other.x 
	};
}

float Vector3f::mag() const {
	return std::sqrt(x * x + y * y + z * z);
}

float Vector3f::mag2() const {
	return x * x + y * y + z * z;
}

Vector3f& Vector3f::operator=(const Vector3f& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

Vector3f Vector3f::normalize() const {

	float len = mag();
	if (len != 0.0f)
	{
		float n = 1.0f / len;

		Vector3f normal = *this;

		normal.x *= n;
		normal.y *= n;
		normal.z *= n;
	
		return normal;
	}
	else
	{
		return Vector3f::ZERO;
	}
	
}

float psim::Vector3f::angle(const Vector3f& other)
{
	float dotproduct;
	float cos_angle;
	float angle;

	// cos(a) = (a.b) / |a|*|b|  
	dotproduct = (*this) * other;
	cos_angle = dotproduct / (this->mag() * other.mag());
	
	angle = acosf(cos_angle);
	return angle;
}

Vector3f psim::operator*(const float f, const Vector3f& v)
{
	return Vector3f{
		v.x * f,
		v.y * f,
		v.z * f
	};
}
