#include "shape.h"
#include "raymath.h"
#include "vector3f.h"
#include "raylib_extra.h"

psim::Shape::Shape(ShapeType type, Color color) : type(type), color(color)
{
}

psim::Shape::~Shape()
{
}

psim::ShapeType psim::Shape::getType()
{
	return this->type;
}

Color psim::Shape::getColor()
{
	return this->color;
}

void psim::Shape::setColor(Color c)
{
	this->color = c;
}

void psim::Shape::transform(Matrix& matrix)
{
	model.transform = matrix;
}

BoundingBox psim::Shape::getAABB()
{
	BoundingBox box = GetTransformedModelBoundingBox(model);

	// transform bounding box
	//box.min = model.transform * box.min;
	//box.max = model.transform * box.max;

	return box;
}
