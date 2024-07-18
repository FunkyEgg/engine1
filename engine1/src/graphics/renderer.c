#include <engine1/graphics/renderer.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdbool.h>

// FIXME: Pre compile step for shaders?
const char* vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* frag_shader_source = "#version 330 core\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

void e1renderobject_render(const E1RenderObject* const render_object) {
    if (render_object == NULL) { return; }

    glUseProgram(render_object->shader_program);
    glBindVertexArray(render_object->VAO);

    if (render_object->EBO == 0) {
        glDrawArrays(GL_TRIANGLES, 0, render_object->count);
    }
    else {
        glDrawElements(GL_TRIANGLES, render_object->count, GL_UNSIGNED_INT, 0);
    }
}

void e1renderobject_render_vector(const Vector* const render_objects) {
    if (render_objects == NULL || render_objects->elems == NULL) { return; }

    for (uint32_t i = 0; i < render_objects->size; i++) {
        E1RenderObject* render_object = render_objects->elems[i];
        e1renderobject_render(render_object);
    }
}

// FIXME: This is very hacky, do it better
void hack_wireframe() {
    static bool is_wireframe = false;

    if (is_wireframe) {
        is_wireframe = false;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else {
        is_wireframe = true;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

E1RenderObject e1renderobject_create_triangle(
    Vec3(float32_t) p1,
    Vec3(float32_t) p2,
    Vec3(float32_t) p3
) {
    return e1renderobject_create(
        (float32_t[]){
            p1.x, p1.y, p1.z,
            p2.x, p2.y, p2.z,
            p3.x, p3.y, p3.z
        },
        NULL,
        9,
        0
    );
}

E1RenderObject e1renderobject_create(
    float32_t vertices[],
    uint32_t indices[],
    uint32_t vertices_length,
    uint32_t indices_length
) {
    E1RenderObject render_object = { 0, 0, 0, 0, 0 };

    if (vertices == NULL || vertices_length < 9) {
        fprintf(stderr, "Passed in NULL vertices, vertices_length = %u\n", vertices_length);
        return render_object;
    }

    uint32_t vertex_shader;
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
        glCompileShader(vertex_shader);

        int compile_good;
        char log[512];
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_good);
        if (!compile_good) {
            glGetShaderInfoLog(vertex_shader, 512, NULL, log);
            fprintf(stderr, "Vertex shader failed to compile:\n%s\n", log);

            glDeleteShader(vertex_shader);
            return render_object;
        }
    }

    uint32_t frag_shader;
    {
        frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
        glCompileShader(frag_shader);

        int compile_good;
        char log[512];
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &compile_good);
        if (!compile_good) {
            glGetShaderInfoLog(frag_shader, 512, NULL, log);
            fprintf(stderr, "Fragment shader failed to compile:\n%s\n", log);

            glDeleteShader(vertex_shader);
            glDeleteShader(frag_shader);
            return render_object;
        }
    }

    uint32_t shader_program;
    {
        shader_program = glCreateProgram();
        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, frag_shader);
        glLinkProgram(shader_program);

        int compile_good;
        char log[512];
        glGetProgramiv(shader_program, GL_LINK_STATUS, &compile_good);
        if (!compile_good) {
            glGetProgramInfoLog(shader_program, 512, NULL, log);
            fprintf(stderr, "Shader program failed to link:\n%s\n", log);

            glDeleteShader(vertex_shader);
            glDeleteShader(frag_shader);
            return render_object;
        }

        glDeleteShader(vertex_shader);
        glDeleteShader(frag_shader);
    }

    uint32_t VBO, VAO, EBO, count;
    if (indices != NULL) {
        count = indices_length;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO); // DO NOT UNBIND THE EBO UNTIL DONE WITH VAO

        glBindVertexArray(VAO); // Bind VAO Then VBO Then EBO Then extras

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float32_t) * vertices_length, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices_length, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // The call to glVertexAttribPointer registers the vbo as the current one, so unbinding is safe
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0); // Unbind VAO to reduce bugs
    }
    else {
        EBO = 0;
        count = vertices_length / 3;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO); // Bind VAO Then VBO Then extras

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float32_t) * vertices_length, vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // The call to glVertexAttribPointer registers the vbo as the current one, so unbinding is safe
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0); // Unbind VAO to reduce bugs
    }

    render_object = (E1RenderObject){
        .shader_program = shader_program,
        .VAO = VAO, .VBO = VBO, .EBO = EBO,
        .count = count
    };

    return render_object;
}
