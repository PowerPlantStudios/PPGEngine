#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/renderer_types.h"
#include "system/logger_system.h"

namespace PPGE
{
class GeometryPrimitives
{
  public:
    static void CreateTriangle(std::vector<StandardVertex> &vertices, std::vector<unsigned int> &indices);

    static void CreateQuad(std::vector<StandardVertex> &vertices, std::vector<unsigned int> &indices);

    static void CreateCube(std::vector<StandardVertex> &vertices, std::vector<unsigned int> &indices);

    static void CreateCapsule(std::vector<StandardVertex> &vertices, std::vector<unsigned int> &indices);

    static void CreateSphere(std::vector<StandardVertex> &vertices, std::vector<unsigned int> &indices);
};
} // namespace PPGE
