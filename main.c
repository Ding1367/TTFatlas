// Test code
#include <math.h>
#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>
#include <stdio.h>
#include <stdlib.h>

#include "TTFat/font.h"

void reportGLFWError(void) {
    const char *description = NULL;
    glfwGetError(&description);
    fprintf(stderr, "GLFW Error: %s\n", description);
}

#define reportGLFWErrorAndExit(code) (reportGLFWError(), exit(code))

typedef struct point_t {
    float x, y;
} point_t;

float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}
point_t lerp_points(point_t a, point_t b, float t) {
    return (point_t){lerp(a.x, b.x, t), lerp(a.y, b.y, t)};
}

point_t quadratic_bezier(point_t p0, point_t p1, point_t p2, float t) {
    point_t p01 = lerp_points(p0, p1, t);
    point_t p12 = lerp_points(p1, p2, t);
    return lerp_points(p01, p12, t);
}

void drawBezier(point_t p0, point_t p1, point_t p2, int res) {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= res; ++i) {
        float t = (float)i / res;
        point_t p = quadratic_bezier(p0, p1, p2, t);
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

#define POINT(x,y) ((point_t){x,y})

typedef struct texture {
    uint32_t *pixels;
    int w, h;
} texture_t;

typedef struct color {
    unsigned char r, g, b, a;
} color_t;

int main(void) {
    if (!glfwInit()) {
        reportGLFWErrorAndExit(1);
    }
    GLFWwindow *window = glfwCreateWindow(800, 600, "Window", NULL, NULL);
    glfwMakeContextCurrent(window);

    FONT *font = TTFat_OpenFont("/Users/alifezithu/Downloads/JetBrainsMono/JetBrainsMonoNerdFont-Regular.ttf");
    if (!font) {
        fprintf(stderr, "Failed to open font\n");
        exit(1);
    }
    FONT_INFO *info = TTFat_GetInfo(font);
    printf("Signature: %4x\tVersion: %4x\tDirectory Offset: %4x\n", info->signature, info->version,
        info->directoryOffset);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }
    TTFat_DestroyFont(font);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
