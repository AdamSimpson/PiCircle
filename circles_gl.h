#ifndef MULTI_TEX_H
#define MULTI_TEX_H

#include "GLES2/gl2.h"
#include "egl_utils.h"

#define NUM_TEXTURES 2
#define LEFT 0
#define RIGHT 1

typedef struct
{
    // OpenGL|ES state
    EGL_STATE_T egl_state;

    // Program handle
    GLuint program;

    // Locations
    GLint position_location;
    GLint tex_coord_location;

    int terminate;
} STATE_T;

void create_vertices();
void create_shaders(STATE_T *state);
void draw_circles(STATE_T *state);

#endif
