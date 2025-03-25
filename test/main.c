// Test code
#include <math.h>
#include <glad/glad.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <TTFat/font.h>

void reportGLFWError(void) {
    const char *description = NULL;
    glfwGetError(&description);

    fprintf(stderr, "GLFW Error: %s\n", description);
}

#define reportGLFWErrorAndExit(code) (reportGLFWError(), exit(code))

int main(void) {
    if (!glfwInit()) {
        reportGLFWErrorAndExit(1);
    }

    printf("Opening font %s\n", TEST_FONT_PATH);
    FONT *font = TTFat_OpenFont(TEST_FONT_PATH);
    if (!font) {
        fprintf(stderr, "Failed to open font\n");
        exit(1);
    }
    GLFWwindow *window = glfwCreateWindow(800, 600, "Window", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glad_glClearColor(1,1,1,1);
        glad_glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }
    TTFat_DestroyFont(font);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
