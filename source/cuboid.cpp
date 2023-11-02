#include "cuboid.h"
#include "collision.h"

psim::Cuboid::Cuboid(Vector3f dim) : Shape(ShapeType::CUBOID, RED), dim{ dim }
{
	Mesh mesh = GenMeshCube(dim.x, dim.y, dim.z);
	model = LoadModelFromMesh(mesh);
}

psim::Cuboid::~Cuboid()
{
	UnloadModel(model);
}

void psim::Cuboid::draw(const Vector3f& pos) const
{
	DrawModel(model, Vector3f::ZERO, 1, color);
	DrawModelWires(model, Vector3f::ZERO, 1, BLACK);
}

float psim::Cuboid::getVolume() const
{
	return dim.x * dim.y * dim.z;
}