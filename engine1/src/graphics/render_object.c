#include "engine1/math/math.h"
#include <engine1/graphics/renderer.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdbool.h>

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
        E1RenderObject* render_object = &((E1RenderObject*)render_objects->elems)[i];
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
    Vec3(float32_t) p3,
    Vec3(float32_t) c1,
    Vec3(float32_t) c2,
    Vec3(float32_t) c3,
    Vector* shaders
) {
    Vector points = vector_create_from_array(
        (float32_t[]){
            p1.x, p1.y, p1.z, c1.x, c1.y, c1.z,
            p2.x, p2.y, p2.z, c2.x, c2.y, c2.z,
            p3.x, p3.y, p3.z, c3.x, c3.y, c3.z
        },
        18,
        sizeof(float32_t)
    );

    E1RenderObject render_object = e1renderobject_create(
        &points,
        NULL,
        shaders
    );

    vector_destroy(&points);
    return render_object;
}

E1RenderObject e1renderobject_create(
    Vector* vertices,
    Vector* indices,
    Vector* shaders
) {
    E1RenderObject render_object = { 0, 0, 0, 0, 0 };

    if (vector_null(vertices) || vertices->size < 9 || !vector_type_is(vertices, sizeof(float32_t))) {
        fprintf(
            stderr,
            "Passed in NULL vertices OR vertices type is wrong, vertices_length = %u\n",
            vertices->size
        );
        return render_object;
    }

    if (vector_null(shaders) || shaders->size < 2 || !vector_type_is(shaders, sizeof(E1Shader))) {
        fprintf(
            stderr,
            "Passed in NULL shaders or type wrong or not enough shaders, shader count: %u\n",
            shaders->size
        );
        return render_object;
    }

    uint32_t vertex_shader = 0, frag_shader = 0;
    for (uint32_t i = 0; i < shaders->size; i++) {
        // If this works I am never touching it again unless it breaks
        E1Shader* shader = &((E1Shader*)shaders->elems)[i];

        switch (shader->shader_type) {
            case E1_VERTEX_SHADER: {
                if (vertex_shader != 0) {
                    fprintf(stderr, "Multiple vertex shaders provided, only using first one\n");
                    break;
                }

                vertex_shader = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vertex_shader, 1, &shader->shader_source, NULL);
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
                break;
            }

            case E1_FRAGMENT_SHADER: {
                if (frag_shader != 0) {
                    fprintf(stderr, "Multiple fragment shaders provided, only using first one\n");
                    break;
                }

                frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(frag_shader, 1, &shader->shader_source, NULL);
                glCompileShader(frag_shader);

                int compile_good;
                char log[512];
                glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &compile_good);
                if (!compile_good) {
                    glGetShaderInfoLog(frag_shader, 512, NULL, log);
                    fprintf(stderr, "Fragment shader failed to compile:\n%s\n", log);

                    glDeleteShader(frag_shader);
                    return render_object;
                }
                break;
            }
        }
    }

    if (vertex_shader == 0 || frag_shader == 0) {
        fprintf(stderr, "Vertex/Fragment shader is unbound\n");
        return render_object;
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
    if (!vector_null(indices)) {
        if (!vector_type_is(indices, sizeof(uint32_t))) {
            fprintf(stderr, "Indices vector isn't the right type");
            return render_object;
        }

        count = indices->size;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO); // DO NOT UNBIND THE EBO UNTIL DONE WITH VAO

        glBindVertexArray(VAO); // Bind VAO Then VBO Then EBO Then extras

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(float32_t) * vertices->size,
            (float32_t*)vertices->elems,
            GL_STATIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(uint32_t) * indices->size,
            (uint32_t*)indices->elems,
            GL_STATIC_DRAW
        );

        // Pos attrib
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float32_t), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attrib
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float32_t), (void*)(3 * sizeof(float32_t)));
        glEnableVertexAttribArray(1);

        // The call to glVertexAttribPointer registers the vbo as the current one, so unbinding is safe
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0); // Unbind VAO to reduce bugs
    }
    else {
        EBO = 0;
        count = vertices->size / 6;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO); // Bind VAO Then VBO Then extras

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(float32_t) * vertices->size,
            (float32_t*)vertices->elems,
            GL_STATIC_DRAW
        );

        // Pos attrib
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float32_t), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attrib
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float32_t), (void*)(3 * sizeof(float32_t)));
        glEnableVertexAttribArray(1);

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
