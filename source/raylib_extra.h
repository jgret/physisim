#pragma once
#include "raylib.h"

BoundingBox GetTransformedModelBoundingBox(const Model& model);

BoundingBox GetTransformedMeshBoundingBox(const Mesh& mesh, const Matrix& transform);