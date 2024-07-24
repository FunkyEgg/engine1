#ifndef ENGINE1_GRAPHICS_RENDERER_H
#define ENGINE1_GRAPHICS_RENDERER_H

#include <engine1/math/math.h>
#include <engine1/core/vector.h>
#include <stdint.h>

typedef struct E1RenderObject {
    uint32_t shader_program;
    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;
    // Could be the ebo length or verts length / 3
    uint32_t count;
} E1RenderObject;

typedef struct E1Shader {
    enum E1ShaderType {
        E1_VERTEX_SHADER,
        E1_FRAGMENT_SHADER,
    } shader_type;
    const char* const shader_source;
} E1Shader;

E1RenderObject e1renderobject_create(
    Vector* vertices,
    Vector* indices,
    Vector* shaders
);
E1RenderObject e1renderobject_create_triangle(
    Vec3(float32_t) p1,
    Vec3(float32_t) p2,
    Vec3(float32_t) p3,
    Vec3(float32_t) c1,
    Vec3(float32_t) c2,
    Vec3(float32_t) c3,
    Vector* shaders
);
void e1renderobject_render(const E1RenderObject* const render_object);
void e1renderobject_render_vector(const Vector* const render_objects);

E1Shader e1shader_create(uint8_t shader_type, const char* const source);

void e1renderobject_set_float4(
    const E1RenderObject* const render_object,
    const char* const name,
    Vec4(float32_t) floats
);

void hack_wireframe();

#endif
