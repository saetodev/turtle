#ifndef TURTLE_TYPES_H
#define TURTLE_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef union Vec2 {
    struct {
        f32 x;
        f32 y;
    };

    struct {
        f32 w;
        f32 h;
    };

} v2;

static inline v2 make_vec2(f32 x, f32 y) {
    return (v2){ x, y };
}

typedef union Vec4 {
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };

    struct {
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };

} v4;

#endif
