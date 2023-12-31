#pragma once

#include "vector3f.h"
#include "raylib.h"
#include "matrix3x3.h"

namespace psim {

	enum ShapeType {
		CUBOID,
		SPHERE,
	};

	class Shape
	{

	private:
		ShapeType type;
		
	protected:
		Color color;
		Model model;
		

	public:

		Shape(ShapeType type, Color color);
		~Shape();
		
		virtual void draw(const Vector3f &pos) const = 0;
		virtual float getVolume() const = 0;

		ShapeType getType();

		Color getColor();

		void setColor(Color c);

		void transform(Matrix& matrix);

		virtual BoundingBox getAABB() const;

		virtual Matrix3x3 computeInertiaTensor() const = 0;

	};

}
