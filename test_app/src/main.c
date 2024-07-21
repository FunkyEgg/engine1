#include <engine1/engine1.h>
#include <stdio.h>

#include "shaders.h"

// TODO: Bundle window + render_ctx + ... into a engine struct to store and pass things around nicely

// TODO: Fix input callback, make it nicer and easier
// This could be done by making a key struct, with nice and easy methods

static bool render = true;

void input_callback(
    const E1Window* const window,
    int32_t key,
    int32_t action,
    int32_t modifiers
) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        e1window_should_close(window);
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        hack_wireframe();
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        render = !render;
    }
}
void render_callback(
    const E1Window* const window,
    const Vector* const render_objects
) {
    e1window_set_bg_color( (Vec4(float32_t)){ 0.1f, 0.2f, 0.2f, 1.0f } );

    if (render) {
        e1renderobject_render_vector(render_objects);
    }
}

int main(void) {
    E1Window window = e1window_create(1280, 720, "Hello World");

    Vector default_shaders = vector_create_from_array(
        (E1Shader[]){
            e1shader_create(E1_VERTEX_SHADER, first_vert_shader_source),
            e1shader_create(E1_FRAGMENT_SHADER, first_frag_shader_source)
        },
        2,
        sizeof(E1Shader)
    );

    Vector alt_shaders = vector_create_from_array(
        (E1Shader[]){
            e1shader_create(E1_VERTEX_SHADER, first_vert_shader_source),
            e1shader_create(E1_FRAGMENT_SHADER, first_alt_frag_shader_source)
        },
        2,
        sizeof(E1Shader)
    );

    Vector verticies = vector_create_from_array(
        (float32_t[]){
            -0.25f, -0.25f, 0.0f,
            -0.25f, -0.75f, 0.0f,
            -0.75f, -0.75f, 0.0f
        },
        9,
        sizeof(float32_t)
    );

    E1RenderObject render_obj_1 = e1renderobject_create(
        &verticies,
        NULL,
        &alt_shaders
    );

    E1RenderObject render_obj_2 = e1renderobject_create_triangle(
        (Vec3(float32_t)){ 0.25f, 0.25f, 0.0f },
        (Vec3(float32_t)){ 0.25f, 0.75f, 0.0f },
        (Vec3(float32_t)){ 0.75f, 0.75f, 0.0f },
        &default_shaders
    );

    Vector render_objects = vector_create_from_array(
        (E1RenderObject[]){ render_obj_1, render_obj_2 },
        2,
        sizeof(E1RenderObject)
    );

    window.input_callback = input_callback;
    window.render_callback = render_callback;

    e1window_start_render_loop(&window, &render_objects);

    e1window_destroy(&window);
    return 0;
}
