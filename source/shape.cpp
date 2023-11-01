#include "shape.h"

psim::Shape::Shape(ShapeType type, Color color) : type(type), color(color)
{
}

psim::Shape::~Shape()
{
	UnloadModel(model);
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
