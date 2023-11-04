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

psim::Matrix3x3 psim::Cuboid::computeInertiaTensor() const
{

	float k = 1.0f / 12.0f;

	float w2 = dim.x * dim.x;
	float h2 = dim.y * dim.y;
	float d2 = dim.z * dim.z;

	float ixx = k * (h2 + d2);
	float iyy = k * (w2 + d2);
	float izz = k * (w2 + h2);

	return Matrix3x3
	{
		ixx, 0, 0,
		0, iyy, 0,
		0, 0, izz
	};
}
