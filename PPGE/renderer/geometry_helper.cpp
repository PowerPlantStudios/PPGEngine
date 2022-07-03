#include "geometry_helper.h"

namespace PPGE
{
void GeometryPrimitives::CreateTriangle(std::vector<StandardVertex> &vertices, std::vector<unsigned int> &indices)
{
    PPGE_ASSERT(vertices.empty(), "Non-empty container is passed to fill vertex data.");
    PPGE_ASSERT(indices.empty(), "Non-empty container is passed to fill index data");
}

void GeometryPrimitives::CreateQuad(std::vector<StandardVertex> &vertices, std::vector<unsigned int> &indices)
{
    PPGE_ASSERT(vertices.empty(), "Non-empty container is passed to fill vertex data.");
    PPGE_ASSERT(indices.empty(), "Non-empty container is passed to fill index data");

    vertices = {{.px = 1.0f, .py  = 0.0f, .pz  = 1.0f,
                 .nx  = 0.0f, .ny  = 1.0f, .nz  = 0.0f,
                 .tx  = 1.0f, .ty  = 0.0f, .tz  = 0.0f,
                 .btx = 0.0f, .bty = 0.0f, .btz = 1.0f,
                 .color = 0xE0E0E0FF,
                 .u1 = 0.0f, .v1 = 0.0f},
                {.px  = 1.0f, .py  = 0.0f, .pz  = -1.0f,
                 .nx  = 0.0f, .ny  = 1.0f, .nz  = 0.0f,
                 .tx  = 1.0f, .ty  = 0.0f, .tz  = 0.0f,
                 .btx = 0.0f, .bty = 0.0f, .btz = 1.0f,
                 .color = 0xE0E0E0FF,
                 .u1 = 1.0f, .v1 = 0.0f},
                {.px  = -1.0f, .py  = 0.0f, .pz  = -1.0f,
                 .nx  = 0.0f,  .ny  = 1.0f, .nz  = 0.0f,
                 .tx  = 1.0f,  .ty  = 0.0f, .tz  = 0.0f,
                 .btx = 0.0f,  .bty = 0.0f, .btz = 1.0f,
                 .color = 0xE0E0E0FF,
                 .u1 = 1.0f, .v1 = 1.0f},
                {.px  = -1.0f, .py  = 0.0f, .pz  = 1.0f,
                 .nx  = 0.0f,  .ny  = 1.0f, .nz  = 0.0f,
                 .tx  = 1.0f,  .ty  = 0.0f, .tz  = 0.0f,
                 .btx = 0.0f,  .bty = 0.0f, .btz = 1.0f,
                 .color = 0xE0E0E0FF,
                 .u1 = 0.0f, .v1 = 1.0f}};
    indices = {0, 3, 2, 2, 1, 0};
}

void GeometryPrimitives::CreateCube(std::vector<StandardVertex> &vertices, std::vector<unsigned int> &indices)
{
    PPGE_ASSERT(vertices.empty(), "Non-empty container is passed to fill vertex data.");
    PPGE_ASSERT(indices.empty(), "Non-empty container is passed to fill index data");
}

void GeometryPrimitives::CreateCapsule(std::vector<StandardVertex> &vertices, std::vector<unsigned int> &indices)
{
    PPGE_ASSERT(vertices.empty(), "Non-empty container is passed to fill vertex data.");
    PPGE_ASSERT(indices.empty(), "Non-empty container is passed to fill index data");
}

void GeometryPrimitives::CreateSphere(std::vector<StandardVertex> &vertices, std::vector<unsigned int> &indices)
{
    PPGE_ASSERT(vertices.empty(), "Non-empty container is passed to fill vertex data.");
    PPGE_ASSERT(indices.empty(), "Non-empty container is passed to fill index data");
}

} // namespace PPGE