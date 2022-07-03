/*  Common textures
*/

#ifndef PPGE_COMMON_TEXTURES
#define PPGE_COMMON_TEXTURES

Texture2D g_texture_material_albedo     : register(t0);
Texture2D g_texture_material_roughness  : register(t1);
Texture2D g_texture_material_metalic    : register(t2);
Texture2D g_texture_material_normal     : register(t3);
Texture2D g_texture_material_occlusion  : register(t4);
Texture2D g_texture_material_emission   : register(t5);
Texture2D g_texture_material_height     : register(t6);
Texture2D g_texture_material_alpha_mask : register(t7);

Texture2DArray g_shadow_map_dir_light   : register(t8);
TextureCube    g_shadow_map_point_light : register(t9);
Texture2D      g_shadow_map_spot_light  : register(t10);

#endif // PPGE_COMMON_TEXTURES
