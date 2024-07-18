#ifndef ENGINE1_GRAPHICS_WINDOW_H
#define ENGINE1_GRAPHICS_WINDOW_H

#include <engine1/math/math.h>
#include <engine1/core/vector.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef struct E1Window {
    GLFWwindow* glfw_window;

    // FIXME: E1Renderer
    void (*render_callback)(const struct E1Window* const, const Vector* const);
    // Window, Key, Action, Modifiers
    void (*input_callback)(const struct E1Window* const, int32_t, int32_t, int32_t);
} E1Window;

E1Window e1window_create(int16_t width, int16_t height, const char* const title);
void e1window_destroy(E1Window* window);
void e1window_start_render_loop(
    const E1Window* const window,
    const Vector* const render_objects
);

bool e1window_key_pressed(const E1Window* const window, int32_t key);
void e1window_should_close(const E1Window* const window);
void e1window_set_bg_color(const Vec4(float32_t) rgba);

#endif
