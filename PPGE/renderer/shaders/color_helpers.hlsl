/*  Helper functions to encode/decode RGBA colors and 
    color space conversions
*/

#ifndef PPGE_COLOR_HELPERS
#define PPGE_COLOR_HELPERS

float4 Decode32bitRGBA(uint color)
{
    float r = ((color >> 24) & 0xff) / 255.0f;
    float g = ((color >> 16) & 0xff) / 255.0f;
    float b = ((color >>  8) & 0xff) / 255.0f;
    float a = ((color      ) & 0xff) / 255.0f;

    return float4(r, g, b, a);
}

#endif // PPGE_COLOR_HELPERS
