#pragma once

#include "vector3f.h"
#include "raylib.h"

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
		

	public:

		Shape(ShapeType type, Color color) : type(type), color(color) {}
		
		virtual void draw(const Vector3f &pos) const = 0;
		virtual float getVolume() const = 0;

		ShapeType getType() {
			return this->type;
		}

		Color getColor() {
			return this->color;
		}

		void setColor(Color c) {
			this->color = c;
		}


	};

}
