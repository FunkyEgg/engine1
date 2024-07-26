#include <engine1/graphics/renderer.h>
#include <engine1/core/assert.h>
#include <glad/glad.h>
#include <stdio.h>

// Forward declare
static uint32_t e1shadersource_compile_shader(const E1ShaderSource* const source);

E1ShaderSource e1shader_create(uint8_t shader_type, const char* const source) {
    if (shader_type != E1_VERTEX_SHADER && shader_type != E1_FRAGMENT_SHADER) {
        fprintf(stderr, "Unknown shader type\n");
        return (E1ShaderSource){ 0 };
    }

    if (source == NULL) {
        fprintf(stderr, "Empty shader source provided.");
        return (E1ShaderSource){ 0 };
    }

    return (E1ShaderSource) {
        shader_type,
        source
    };
}

uint32_t e1shadersource_create_program(const Vector* const shader_sources) {
    uint32_t vert_shader = 0, frag_shader = 0;

    for (uint8_t i = 0; i < shader_sources->size; i++) {
        // If this works I am never touching it again unless it breaks
        E1ShaderSource* shader = &((E1ShaderSource*)shader_sources->elems)[i];
        switch (shader->shader_type) {
            case E1_VERTEX_SHADER:
                vert_shader = e1shadersource_compile_shader(shader);
                break;
            case E1_FRAGMENT_SHADER:
                frag_shader = e1shadersource_compile_shader(shader);
                break;
        }
    }

    // I feel safe to do this since shaders are read in at compile time
    ASSERT(vert_shader != 0 && frag_shader != 0, "Vertex/Fragment shaders is unbound");

    uint32_t shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);

    int compile_good;
    char log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &compile_good);
    if (!compile_good) {
        glGetProgramInfoLog(shader_program, 512, NULL, log);
        fprintf(stderr, "Shader program failed to link:\n%s\n", log);

        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);
        return 0;
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    return shader_program;
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

static uint32_t e1shadersource_compile_shader(const E1ShaderSource* const source) {
    uint32_t shader = 0;

    switch (source->shader_type) {
        case E1_VERTEX_SHADER: {
            shader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(shader, 1, &source->shader_source, NULL);
            glCompileShader(shader);

            int compile_good;
            char log[512];
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_good);
            if (!compile_good) {
                glGetShaderInfoLog(shader, 512, NULL, log);
                fprintf(stderr, "Vertex shader failed to compile:\n%s\n", log);

                glDeleteShader(shader);
                return 0;
            }
            break;
        }

        case E1_FRAGMENT_SHADER: {
            if (shader != 0) {
                fprintf(stderr, "Multiple fragment shaders provided, only using first one\n");
                break;
            }

            shader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(shader, 1, &source->shader_source, NULL);
            glCompileShader(shader);

            int compile_good;
            char log[512];
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_good);
            if (!compile_good) {
                glGetShaderInfoLog(shader, 512, NULL, log);
                fprintf(stderr, "Fragment shader failed to compile:\n%s\n", log);

                glDeleteShader(shader);
                return 0;
            }
            break;
        }
    }

    return shader;
}
