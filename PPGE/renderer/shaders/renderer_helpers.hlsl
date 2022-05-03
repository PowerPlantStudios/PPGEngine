/*  Helpers for renderer options
*/

#ifndef PPGE_RENDERER_HELPERS
#define PPGE_RENDERER_HELPERS

#include "common_buffers.hlsl"
#include "common_defines.hlsl"

#define RENDERER_ENABLE_NORMAL_MAP   uint(1U << 0)

bool renderer_is_normal_map_enabled() { return g_renderer_options & RENDERER_ENABLE_NORMAL_MAP; }

#endif // PPGE_RENDERER_HELPERS
