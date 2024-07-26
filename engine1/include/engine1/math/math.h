#ifndef ENGINE1_MATH_MATH_H
#define ENGINE1_MATH_MATH_H

#include <stdint.h>

typedef float float32_t;
_Static_assert(sizeof(float32_t) * 8 == 32, "float32_t is not 32 bits");

typedef double float64_t;
_Static_assert(sizeof(float64_t) * 8 == 64, "float64_t is not 64 bits");

#define Vec2Init(T) typedef struct { T x; T y; } T##_Vec2
#define Vec2(T) T##_Vec2

Vec2Init(int8_t);
Vec2Init(int16_t);
Vec2Init(int32_t);
Vec2Init(int64_t);
Vec2Init(uint8_t);
Vec2Init(uint16_t);
Vec2Init(uint32_t);
Vec2Init(uint64_t);
Vec2Init(float32_t);
Vec2Init(float64_t);

#define Vec3Init(T) typedef struct { T x; T y; T z; } T##_Vec3
#define Vec3(T) T##_Vec3

Vec3Init(int8_t);
Vec3Init(int16_t);
Vec3Init(int32_t);
Vec3Init(int64_t);
Vec3Init(uint8_t);
Vec3Init(uint16_t);
Vec3Init(uint32_t);
Vec3Init(uint64_t);
Vec3Init(float32_t);
Vec3Init(float64_t);

#define Vec4Init(T) typedef struct { T x; T y; T z; T w; } T##_Vec4
#define Vec4(T) T##_Vec4

Vec4Init(int8_t);
Vec4Init(int16_t);
Vec4Init(int32_t);
Vec4Init(int64_t);
Vec4Init(uint8_t);
Vec4Init(uint16_t);
Vec4Init(uint32_t);
Vec4Init(uint64_t);
Vec4Init(float32_t);
Vec4Init(float64_t);

#define RATIO_16_9 (Vec2(uint8_t)){ 16, 9 }
#define RATIO_4_3  (Vec2(uint8_t)){ 4, 3 }

uint64_t nearest_containing_power_of_2(uint64_t num);

Vec3(float32_t) arc_to_glc(Vec3(float32_t) arc, Vec2(uint8_t) ratio);
Vec3(float32_t) glc_to_arc(Vec3(float32_t) glc, Vec2(uint8_t) ratio);

#endif
