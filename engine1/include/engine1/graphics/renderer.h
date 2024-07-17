#ifndef ENGINE1_GRAPHICS_RENDERER_H
#define ENGINE1_GRAPHICS_RENDERER_H

#include <stdint.h>

typedef struct E1RenderContext {
    uint32_t shader_program;
    uint32_t VAO;
    uint32_t VBO;
} E1RenderContext;

E1RenderContext e1rendercontext_create();
void render_temp(const E1RenderContext* const render_context);

#endif
