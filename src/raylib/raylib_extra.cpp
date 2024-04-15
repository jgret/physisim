#include "raylib_extra.h"
#include "../common/vector3f.h"
#include "raymath.h"

BoundingBox GetTransformedModelBoundingBox(const Model& model)
{
    BoundingBox bounds = { 0 };

    if (model.meshCount > 0)
    {
        Vector3 temp = { 0 };
        bounds = GetTransformedMeshBoundingBox(model.meshes[0], model.transform);

        for (int i = 1; i < model.meshCount; i++)
        {
            BoundingBox tempBounds = GetMeshBoundingBox(model.meshes[i]);

            temp.x = (bounds.min.x < tempBounds.min.x) ? bounds.min.x : tempBounds.min.x;
            temp.y = (bounds.min.y < tempBounds.min.y) ? bounds.min.y : tempBounds.min.y;
            temp.z = (bounds.min.z < tempBounds.min.z) ? bounds.min.z : tempBounds.min.z;
            bounds.min = temp;

            temp.x = (bounds.max.x > tempBounds.max.x) ? bounds.max.x : tempBounds.max.x;
            temp.y = (bounds.max.y > tempBounds.max.y) ? bounds.max.y : tempBounds.max.y;
            temp.z = (bounds.max.z > tempBounds.max.z) ? bounds.max.z : tempBounds.max.z;
            bounds.max = temp;
        }
    }

    return bounds;
}

BoundingBox GetTransformedMeshBoundingBox(const Mesh& mesh, const Matrix& transform)
{
    // Get min and max vertex to construct bounds (AABB)
    Vector3 minVertex = { 0 };
    Vector3 maxVertex = { 0 };

    if (mesh.vertices != NULL)
    {
        minVertex = transform * Vector3{ mesh.vertices[0], mesh.vertices[1], mesh.vertices[2] };
        maxVertex = transform * Vector3{ mesh.vertices[0], mesh.vertices[1], mesh.vertices[2] };

        for (int i = 1; i < mesh.vertexCount; i++)
        {
            minVertex = Vector3Min(minVertex, transform * Vector3
            {
                mesh.vertices[i * 3], mesh.vertices[i * 3 + 1], mesh.vertices[i * 3 + 2]
            });
            maxVertex = Vector3Max(maxVertex, transform * Vector3 
            {
                mesh.vertices[i * 3], mesh.vertices[i * 3 + 1], mesh.vertices[i * 3 + 2]
            });
        }
    }

    // Create the bounding box
    BoundingBox box = { 0 };
    box.min = minVertex;
    box.max = maxVertex;

    return box;
}
