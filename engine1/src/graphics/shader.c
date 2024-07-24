#include <engine1/graphics/renderer.h>
#include <engine1/math/math.h>
#include <glad/glad.h>
#include <stdio.h>

E1Shader e1shader_create(uint8_t shader_type, const char* const source) {
    if (shader_type != E1_VERTEX_SHADER && shader_type != E1_FRAGMENT_SHADER) {
        fprintf(stderr, "Unknown shader type\n");
        return (E1Shader){ 0, 0 };
    }

    if (source == NULL) {
        fprintf(stderr, "Empty shader source provided.");
        return (E1Shader){ 0, 0 };
    }

    return (E1Shader) {
        shader_type,
        source
    };
}

void e1renderobject_set_float4(
    const E1RenderObject* const render_object,
    const char* const name,
    Vec4(float32_t) floats
) {
    glUniform4f(
        glGetUniformLocation(render_object->shader_program, name),
        floats.x, floats.y, floats.z, floats.w
    );
}
