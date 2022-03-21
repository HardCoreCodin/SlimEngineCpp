#pragma once

#include "../core/types.h"

//INLINE f32 toneMappedBaked(f32 LinearColor) {
//    f32 x = LinearColor - 0.004f;
//    if (x < 0.0f) x = 0.0f;
//    f32 x2_times_sholder_strength = x * x * 6.2f;
//    return (x2_times_sholder_strength + x*0.5f)/(x2_times_sholder_strength + x*1.7f + 0.06f);
//}
//
//INLINE vec3 reflectWithDot(const vec3 &V, const vec3 &N, f32 NdotV) {
//    return N.scaleAdd(-2 * NdotV, V);
//}
//
//INLINE vec3 shadePointOnSurface(const Shaded &shaded, f32 NdotL) {
//    MaterialHas material_has{shaded.material->flags};
//    MaterialUses material_uses{shaded.material->flags};
//    if (material_has.specular) {
//        vec3 half_vector, color;
//        if (material_uses.blinn) {
//            half_vector = (shaded.light_direction - shaded.viewing_direction).normalized();
//            color = shaded.material->specular * powf(clampedValue(shaded.normal | half_vector), 16.0f * shaded.material->shininess);
//        } else
//            color = shaded.material->specular * powf(clampedValue(shaded.reflected_direction | shaded.light_direction), 4.0f * shaded.material->shininess);
//
//        if (material_has.diffuse)
//            return shaded.diffuse.scaleAdd(clampedValue(NdotL), color);
//        else
//            return color;
//    } else
//        return shaded.diffuse * clampedValue(NdotL);
//}