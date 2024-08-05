#define LOG_CONTEXT "test_app"
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
    printf("Make the world your own.\n");

    LOG_TRACE("Good Luck");
    LOG_DEBUG("C");
    LOG_INFO("O");
    LOG_WARN("L");
    LOG_ERROR("O");
    LOG_CRIT("R");

    E1Window window = e1window_create(1280, 720, "world");

    Vector default_shaders = vector_create_from_array((E1ShaderSource[]){
            e1shader_create(E1_VERTEX_SHADER, first_vert_shader_source),
            e1shader_create(E1_FRAGMENT_SHADER, first_frag_shader_source)
        }, 2, sizeof(E1ShaderSource)
    );

    E1RenderObject object_1 = e1renderobject_create((E1RenderObjectSource){
        .shader_program = e1shadersource_create_program(&default_shaders),
        .vertices = vector_create_from_array(
            (float32_t[]){
                -0.75f, -0.75f, 0.0f,
                -0.25f, -0.75f, 0.0f,
                -0.75f, -0.25f, 0.0f,
                -0.25f, -0.25f, 0.0f
            }, 12, sizeof(float32_t)
        ),
        .colors = vector_create_from_array((float32_t[]){
                0.09f, 0.01f, 0.65f,
                0.12f, 0.89f, 0.35f,
                0.89f, 0.35f, 0.71f,
                0.67f, 0.12f, 0.45f
            }, 12, sizeof(float32_t)
        ),
        .indices = vector_create_from_array((uint32_t[]){
                0, 1, 3,
                0, 2, 3
            }, 6, sizeof(uint32_t)
        )
    });

    E1RenderObject object_2 = e1renderobject_create((E1RenderObjectSource){
        .shader_program = e1shadersource_create_program(&default_shaders),
        .vertices = vector_create_from_array(
            (float32_t[]){
                0.8f, 0.8f, 0.0f,
                0.2f, 0.8f, 0.0f,
                0.5f, 0.2f, 0.0f
            }, 9, sizeof(float32_t)
        ),
        .colors = vector_create_from_array((float32_t[]){
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.1f
            }, 9, sizeof(float32_t)
        ),
    });


    E1RenderObject object_3 = e1renderobject_create((E1RenderObjectSource){
        .shader_program = e1shadersource_create_program(&default_shaders),
        .vertices = vector_create_from_array(
            (float32_t[]){
                Vec3Unwrap(arc_to_glc(COORD(8, 5, 0), RATIO_16_9)),
                Vec3Unwrap(arc_to_glc(COORD(8, 6, 0), RATIO_16_9)),
                Vec3Unwrap(arc_to_glc(COORD(9, 5, 0), RATIO_16_9)),
                Vec3Unwrap(arc_to_glc(COORD(9, 6, 0), RATIO_16_9)),
            }, 12, sizeof(float32_t)
        ),
        .colors = vector_create_from_array((float32_t[]){
                0.09f, 0.01f, 0.65f,
                0.12f, 0.89f, 0.35f,
                0.89f, 0.35f, 0.71f,
                0.67f, 0.12f, 0.45f
            }, 12, sizeof(float32_t)
        ),
        .indices = vector_create_from_array((uint32_t[]){
                0, 1, 3,
                0, 2, 3
            }, 6, sizeof(uint32_t)
        )
    });

    Vector render_objects = vector_create_from_array((E1RenderObject[]){
            object_1, object_2, object_3
        }, 3, sizeof(E1RenderObject)
    );

    window.input_callback = input_callback;
    window.render_callback = render_callback;
    e1window_start_render_loop(&window, &render_objects);

    e1window_destroy(&window);
    return 0;
}
