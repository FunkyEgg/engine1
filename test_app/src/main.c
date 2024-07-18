#include <engine1/engine1.h>

// TODO: Bundle window + render_ctx + ... into a engine struct to store and pass things around nicely

// TODO: Fix input callback, make it nicer and easier
// This could be done by making a key struct, with nice and easy methods
void input_callback(
    const E1Window* const window,
    int32_t key,
    int32_t action,
    int32_t modifiers
) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        e1window_should_close(window);
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        hack_wireframe();
    }
}
void render_callback(
    const E1Window* const window,
    const E1RenderObject* const render_ctx
) {
    e1window_set_bg_color( (Vec4(float32_t)){ 0.1f, 0.2f, 0.2f, 1.0f } );

    render_temp(render_ctx);
}

int main(void) {
    E1Window window = e1window_create(1280, 720, "Hello World");

    /*
    E1RenderObject render_ctx = e1renderobject_create(
        (float32_t[]){
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
        },
        (uint32_t[]){
            0, 1, 3,
            1, 2, 3
        },
        4*3,
        2*3
    ); */

    /*E1RenderObject render_ctx = e1renderobject_create(
        (float32_t[]){
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
        },
        NULL,
        3*3,
        0
    );*/

    E1RenderObject render_ctx = e1renderobject_create_triangle(
        (Vec3(float32_t)){ 0.5f, 0.5f, 0.0f },
        (Vec3(float32_t)){ 0.5f, -0.5f, 0.0f },
        (Vec3(float32_t)){ -0.5f, 0.5f, 0.0f }
    );

    window.input_callback = input_callback;
    window.render_callback = render_callback;

    e1window_start_render_loop(&window, &render_ctx);

    e1window_destroy(&window);
    return 0;
}
