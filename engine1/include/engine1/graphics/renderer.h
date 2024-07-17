#ifndef ENGINE1_GRAPHICS_RENDERER_H
#define ENGINE1_GRAPHICS_RENDERER_H

#include <stdint.h>

typedef struct E1RenderObject {
    uint32_t shader_program;
    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;
} E1RenderContext;

// typedef struct E1Renderer {
//     uint8_t size;
// };

E1RenderContext e1rendercontext_create();
void render_temp(const E1RenderContext* const render_context);

void hack_wireframe();

#endif
