#ifndef ENGINE1_GRAPHICS_RENDERER_H
#define ENGINE1_GRAPHICS_RENDERER_H

#include <engine1/math/math.h>
#include <engine1/core/vector.h>
#include <stdint.h>

/* Refactors: 2 */

typedef struct E1RenderObject {
    uint32_t shader_program;
    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;
    // I've got no clue what this is anymore
    uint32_t count;
} E1RenderObject;

typedef struct E1RenderObjectSource {
    Vector vertices;
    uint32_t shader_program;

    // Optionals
    Vector indices;
    Vector colors;
} E1RenderObjectSource;

typedef struct E1ShaderSource {
    enum E1ShaderType {
        E1_VERTEX_SHADER,
        E1_FRAGMENT_SHADER,
    } shader_type;
    const char* const shader_source;
} E1ShaderSource;


E1RenderObject e1renderobject_create(E1RenderObjectSource source);

void e1renderobject_render(const E1RenderObject* const render_object);
void e1renderobject_render_vector(const Vector* const render_objects);

E1ShaderSource e1shader_create(uint8_t shader_type, const char* const source);
uint32_t e1shadersource_create_program(const Vector* const shader_sources);

void e1renderobject_set_float4(
    const E1RenderObject* const render_object,
    const char* const name,
    Vec4(float32_t) floats
);

void hack_wireframe();

#endif
