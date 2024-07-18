#ifndef ENGINE1_GRAPHICS_RENDERER_H
#define ENGINE1_GRAPHICS_RENDERER_H

#include <engine1/math/math.h>
#include <stdint.h>

typedef struct E1RenderObject {
    uint32_t shader_program;
    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;
    // Could be the ebo length or verts length / 3
    uint32_t count;
} E1RenderObject;

// TODO: Migrate to vecs, when I implement them
typedef struct E1Renderer {
    uint8_t render_object_count;
    E1RenderObject* render_objects;
} E1Renderer;

E1RenderObject e1renderobject_create(
    float32_t vertices[],
    uint32_t indices[],
    uint32_t vertices_length,
    uint32_t indices_length
);
E1RenderObject e1renderobject_create_triangle(
    Vec3(float32_t) p1,
    Vec3(float32_t) p2,
    Vec3(float32_t) p3
);
void render_temp(const E1RenderObject* const render_object);

void hack_wireframe();

#endif
