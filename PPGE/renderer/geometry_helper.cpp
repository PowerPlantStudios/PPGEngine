#include "geometry_helper.h"

namespace PPGE
{
void GeometryPrimitives::CreateTriangle(Entity &entity)
{
}

void GeometryPrimitives::CreatePlaneMesh(Entity &entity)
{
    std::vector<StandardVertex> vertices{{.px  = 1.0f, .py  = 0.0f, .pz  = 1.0f,
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
    std::vector<unsigned int> indices{0, 3, 2, 2, 1, 0};

    GeometryHelper::CreateMeshFilter(entity, vertices, indices);
}

void GeometryPrimitives::CreateCubeMesh(Entity &entity)
{
}

void GeometryPrimitives::CreateCapsuleMesh(Entity &entity)
{
}

void GeometryPrimitives::CreateSphereMesh(Entity &entity)
{
}

} // namespace PPGE