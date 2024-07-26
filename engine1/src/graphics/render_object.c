#include <engine1/math/math.h>
#include <engine1/graphics/renderer.h>
#include <engine1/core/assert.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdbool.h>

// Forward declare
static Vector e1shadersource_to_vertices(const E1RenderObjectSource* const source);

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

E1RenderObjectSource e1renderobjectsource_create(
    const Vector vertices,
    const uint32_t shader_program
) {
    ASSERT(shader_program != 0, "Invalid shader program passed");
    ASSERT(
        !vector_null(&vertices) && vector_type_is(&vertices, sizeof(float32_t)),
        "Incorrect vertices passed"
    );

    return (E1RenderObjectSource){
        .vertices = vertices,
        .shader_program = shader_program,
    };
}


E1RenderObject e1renderobject_create(E1RenderObjectSource source) {
    ASSERT(vector_as_expected(&source.vertices, sizeof(float32_t)), "Can't pass in a null render source");
    ASSERT(source.shader_program != 0, "No shader program provided");

    // FIXME: Is this bad becaue of making a copy when it may not be needed?
    Vector vertices = { 0 };
    bool has_color = !vector_null(&source.colors);
    if (has_color) {
        ASSERT(
            source.vertices.size * 2 == source.vertices.size + source.colors.size,
            "Some vertices provided are missing a color"
        );

        vertices = e1shadersource_to_vertices(&source);
    }
    else {
        vertices = source.vertices;
    }

    /*
    for (int i = 0; i < vertices.size; i++) {
        float32_t vert = ((float32_t*)(vertices.elems))[i];
        fprintf(stderr, "%f\n", vert);
        if (i % 3 == 0) {
        }
    }
    fprintf(stderr, "Size: %u\n", vertices.size);
    */


    uint32_t VBO, VAO, EBO, count;
    if (!vector_null(&source.indices)) {
        ASSERT(vector_type_is(&source.indices, sizeof(uint32_t)), "Indices vector is the incorrect type");

        count = source.indices.size;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO); // DO NOT UNBIND THE EBO UNTIL DONE WITH VAO

        glBindVertexArray(VAO); // Bind VAO Then VBO Then EBO Then extras

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(float32_t) * vertices.size,
            (float32_t*)vertices.elems,
            GL_STATIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(uint32_t) * source.indices.size,
            (uint32_t*)source.indices.elems,
            GL_STATIC_DRAW
        );
    }
    else {
        EBO = 0;
        count = source.vertices.size / 3;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO); // Bind VAO Then VBO Then extras

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(float32_t) * vertices.size,
            (float32_t*)vertices.elems,
            GL_STATIC_DRAW
        );
    }


    // FIXME: Some multiple true multipler thing, i should understand what this means when it comes timw to do it
    if (has_color) {
        // Pos attrib
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float32_t), (void*)0);
        glEnableVertexAttribArray(0);

        // Color pos
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float32_t), (void*)(3 * sizeof(float32_t)));
        glEnableVertexAttribArray(1);
    }
    else {
        // Pos attrib
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float32_t), (void*)0);
        glEnableVertexAttribArray(0);
    }

    // The call to glVertexAttribPointer registers the vbo as the current one, so unbinding is safe
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind VAO to reduce bugs

    return (E1RenderObject){
        .VAO = VAO,
        .VBO = VBO,
        .EBO = EBO,
        .count = count,
        .shader_program = source.shader_program
    };
}

static Vector e1shadersource_to_vertices(const E1RenderObjectSource* const source) {
    // No need for asserts because the calling function (e1renderobject_create)
    // assures a safe environment

    bool has_color = !vector_null(&source->colors);

    Vector vertices = vector_create_empty(sizeof(float32_t));
    for (uint32_t i = 0; i < source->vertices.size; i += 3) {
        vector_push_back(&vertices, &((float32_t*)source->vertices.elems)[i]); // P1
        vector_push_back(&vertices, &((float32_t*)source->vertices.elems)[i + 1]); // P2
        vector_push_back(&vertices, &((float32_t*)source->vertices.elems)[i + 2]); // P3

        if (has_color) {
            vector_push_back(&vertices, &((float32_t*)source->colors.elems)[i]); // C1
            vector_push_back(&vertices, &((float32_t*)source->colors.elems)[i + 1]); // C2
            vector_push_back(&vertices, &((float32_t*)source->colors.elems)[i + 2]); // C3
        }
    }

    return vertices;
}
