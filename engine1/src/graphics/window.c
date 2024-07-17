#include <glad/glad.h>
#include <engine1/graphics/window.h>
#include <engine1/math/math.h>

#include <stdio.h>
#include <stdbool.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

E1Window e1window_create(
    const int16_t width,
    const int16_t height,
    const char* const title
) {
    E1Window window;
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window.glfw_window == NULL) {
        fprintf(stderr, "Failed to create glfw window");
        glfwTerminate();
        return window;
    }
    glfwMakeContextCurrent(window.glfw_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Glad failed to initalize");
        glfwTerminate();
        window.glfw_window = NULL;
        return window;
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window.glfw_window, framebuffer_size_callback);

    return window;
}

void e1window_destroy(E1Window* window) {
    glfwDestroyWindow(window->glfw_window);
    glfwTerminate();
}

void e1window_start_render_loop(
    const E1Window* const window,
    const E1RenderContext* const render_ctx
) {
    while (!glfwWindowShouldClose(window->glfw_window)) {
        window->input_callback(window);
        window->render_callback(window, render_ctx);

        glfwSwapBuffers(window->glfw_window);
        glfwPollEvents();
    }
}

bool e1window_key_pressed(const E1Window* const window, int32_t key) {
    if (glfwGetKey(window->glfw_window, key) == GLFW_PRESS) {
        return true;
    }

    return false;
}

void e1window_should_close(const E1Window* const window) {
    glfwSetWindowShouldClose(window->glfw_window, true);
}

void e1window_set_bg_color(const Vec4(float32_t) rgba) {
    glClearColor(rgba.x, rgba.y, rgba.z, rgba.w);
    glClear(GL_COLOR_BUFFER_BIT);
}
