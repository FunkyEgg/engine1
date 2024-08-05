#include <engine1/math/math.h>

uint64_t nearest_containing_power_of_2(uint64_t num) {
    if (num == 0) { return 1; }

    // Do some bitwie hackery to see if a power of 2, if so retrun num
    if (!(num & (num - 1))) { return num; }

    uint64_t pow = 1;
    while (pow < num) { pow = pow * 2; }
    return pow;
}

Vec3(float32_t) arc_to_glc(Vec3(float32_t) arc, Vec2(uint8_t) ratio) {
    return (Vec3(float32_t)){
        .x = ( ( ( arc.x / ratio.x ) * 2 ) - 1 ),
        .y = ( ( ( arc.y / ratio.y ) * 2 ) - 1 ),
        .z = arc.z
    };
}

Vec3(float32_t) glc_to_arc(Vec3(float32_t) glc, Vec2(uint8_t) ratio) {
    return (Vec3(float32_t)){
        .x = ( ( ( glc.x + 1 ) / 2 ) * ratio.x ),
        .y = ( ( ( glc.y + 1 ) / 2 ) * ratio.y ),
        .z = glc.z
    };
}
