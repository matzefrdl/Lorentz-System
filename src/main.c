#define GLFW_DLL
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stdio.h>
#include "lorentz.h"

#define PI 3.14159265358979323846

int leftMousePressed = 0;
float radius = 30;

void GLAPIENTRY debug_callback(GLenum source, GLenum type, GLuint id,GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    printf(message);
}

GLuint createVertexArray() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    return vao;
}

GLuint createVertexBuffer(void * data, size_t n, GLenum usage, int index, int size) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, n, data, usage);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(index);

    return vbo;
}

GLuint createElementBuffer(void * data, size_t n, GLenum usage) {
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, n, data, usage);

    return ebo;
}

void btnCallback(GLFWwindow * window, int button, int action, int mods) {
    if(action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        leftMousePressed = 1;
    }
    if(action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT) {
        leftMousePressed = 0;
    }
}

void scrollCallback(GLFWwindow * window, double xoff, double yoff) {
    radius += yoff;
    if(radius <= 10)radius = 10.11;
}

int main(void)
{
    lorentz * lor = setupLorentz(10, 8.0f / 3.0f, 27.0f);
    lorentz_render_data * data = setupRenderData();

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(1600, 900, "Display", NULL, NULL);

    glfwSetMouseButtonCallback(window, btnCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    int version = gladLoadGL();
    if (version == 0) {
        printf("Failed to initialize OpenGL context\n");
        return -1;
    }

    glDebugMessageCallback(debug_callback, 0);

    GLuint vao = createVertexArray();
    GLuint ebo = createElementBuffer(data->indices, (data->pos - 1) * 2 * sizeof(int), GL_DYNAMIC_DRAW);
    GLuint vVbo = createVertexBuffer(data->vertices, data->pos * 3 * sizeof(float), GL_DYNAMIC_DRAW, 0, 3);
    GLuint cVbo = createVertexBuffer(data->colors, data->pos * sizeof(float), GL_DYNAMIC_DRAW, 1, 1);

    shader mainShader = createShader("main.vert", "main.frag");

    mat4x4 * proj = createMat4x4();
    perspective(proj, (70.0f / 180.0f) * PI, 1600.0f / 900.0f, 0.1f, 100.0f);

    vec3 t;
    t.x = 0;
    t.y = 0;
    t.z = 0;

    vec3 c;
    c.x = 0;
    c.y = 0;
    c.z = 5;

    vec3 u;
    u.x = 0;
    u.y = 1;
    u.z = 0;

    mat4x4 * view = createMat4x4();
    lookAt(view, t, c, u);
    glUseProgram(mainShader);

    loadUniformMat4x4(mainShader, "proj", proj);
    loadUniformMat4x4(mainShader, "view", view);
    float lastX, lastY;

    float yaw = 0, pitch = 0;

    printf("%s %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
    
    while (!glfwWindowShouldClose(window))
    {
        double mX, mY;
        glfwGetCursorPos(window, &mX, &mY);

        float dX = mX - lastX;
        float dY = mY - lastY;

        if(leftMousePressed) {
            yaw -= dX * 0.01f;
            pitch += dY * 0.01f;
        }
        
        c.x = sin(yaw) * cos(pitch) * radius;
        c.y = sin(pitch) * radius;
        c.z = cos(yaw) * cos(pitch) * radius;
        lookAt(view, t, c, u);
        loadUniformMat4x4(mainShader, "view", view);

        lastX = mX;
        lastY = mY;

        addNewPoint(lor, data);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (data->pos - 1) * 2 * sizeof(int), data->indices, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vVbo);
        glBufferData(GL_ARRAY_BUFFER, data->pos * 3 * sizeof(float), data->vertices, GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, cVbo);
        glBufferData(GL_ARRAY_BUFFER, data->pos * sizeof(float), data->colors, GL_DYNAMIC_DRAW);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_LINES, (data->pos - 1) * 2 , GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    deleteRenderData(data);
    deleteLorentz(lor);
    deleteMat4x4(view);
    deleteMat4x4(proj);
    deleteShader(mainShader);
    glDeleteBuffers(1, &ebo);   
    glDeleteBuffers(1, &vVbo);  
    glDeleteBuffers(1, &cVbo);
    glDeleteVertexArrays(1, &vao);

    glfwTerminate();

    return 0;
    
}