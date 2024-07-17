#include <engine1/graphics/renderer.h>
#include <glad/glad.h>
#include <stdio.h>

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

void render_temp(const E1RenderContext* const render_contex) {
    glUseProgram(render_contex->shader_program);
    glBindVertexArray(render_contex->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

E1RenderContext e1rendercontext_create() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    uint32_t vertex_shader;
    {
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
            return (E1RenderContext){ 0, 0, 0 };
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
            return (E1RenderContext){ 0, 0, 0 };
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
            return (E1RenderContext){ 0, 0, 0 };
        }
        
        glDeleteShader(vertex_shader);
        glDeleteShader(frag_shader);
    }

    uint32_t VBO, VAO;
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO); // Bind VAO Then VBO + extras 

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // The call to glVertexAttribPointer registers the vbo as the current one, so unbinding is safe
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0); // Unbind VAO to reduce bugs
    }
    
    return (E1RenderContext){ shader_program, VAO, VBO };
}
