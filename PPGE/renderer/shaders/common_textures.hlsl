/*  Common textures
*/

#ifndef PPGE_COMMON_TEXTURES
#define PPGE_COMMON_TEXTURES

Texture2D g_texture_material_albedo   : register(t0);
Texture2D g_texture_material_specular : register(t1);
Texture2D g_texture_material_normal   : register(t2);

Texture2DArray g_shadow_map_dir_light   : register(t3);
TextureCube    g_shadow_map_point_light : register(t4);
Texture2D      g_shadow_map_spot_light  : register(t5);

#endif // PPGE_COMMON_TEXTURES
