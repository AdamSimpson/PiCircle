#include <stdio.h>
#include <assert.h>

#include "egl_utils.h"

#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"

#include "circles_gl.h"

#include "bcm_host.h"

void create_vertices()
{
    // Vertices: Pos(x,y) Tex(x,y)
    // For simplicity only single vbo is generated and offset used as needed
    float vertices[] = {
        -0.5f,  0.5f, 0.0f, 0.0f, // Top left
         0.5f,  0.5f, 1.0f, 0.0f, // Top right
         0.5f, -0.5f, 1.0f, 1.0f, // Bottom right
	-0.5f, -0.5f, 0.0f, 1.0f,  // Bottom left

         0.5f,  0.5f, 0.0f, 0.0f, // Top left
         1.0f,  0.5f, 1.0f, 0.0f, // Top right
         1.0f, -0.5f, 1.0f, 1.0f, // Bottom right
	 0.5f, -0.5f, 0.0f, 1.0f  // Bottom left
    };

    // Generate vertex buffer
    GLuint vbo;
    glGenBuffers(1, &vbo);
    // Set buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Fill buffer
    glBufferData(GL_ARRAY_BUFFER, 2*4*4*sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    // Elements
    GLushort elements[] = {
        2, 3, 0,
        0, 1, 2,

        6, 7, 4,
	4, 5, 6
    };

    // Generate element buffer
    GLuint ebo;
    glGenBuffers(1, &ebo);
    // Set buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // Fill buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2*2*3*sizeof(GLushort), elements, GL_STATIC_DRAW);
}

void create_shaders(STATE_T *state)
{
    // Shader source
    const GLchar* vertexSource =
        "attribute vec2 position;"
        "attribute vec2 tex_coord;"
        "varying vec2 frag_tex_coord;"
        "void main() {"
        "   gl_Position = vec4(position, 0.0, 1.0);"
        "   frag_tex_coord = tex_coord;"
        "}";
    const GLchar* fragmentSource =
        "precision mediump float;"
        "varying vec2 frag_tex_coord;"
        "const mediump vec2 center = vec2(0.5, 0.5);"
        "const mediump float radius = 0.5;"
        "void main() {"
	"    mediump float distance_from_center = distance(center, frag_tex_coord);"
	"    lowp float in_radius = step(distance_from_center, radius);"
        "    gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0) * in_radius;"
        "}";

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    showlog(vertexShader);   

    // Compile frag shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    showlog(fragmentShader);

    // Create shader program
    state->program = glCreateProgram();
    glAttachShader(state->program, vertexShader);
    glAttachShader(state->program, fragmentShader); 
  
    // Link and use program
    glLinkProgram(state->program);
    glUseProgram(state->program);
    check();

    // Get position location
    state->position_location = glGetAttribLocation(state->program, "position");
    // Get tex_coord location
    state->tex_coord_location = glGetAttribLocation(state->program, "tex_coord");
}

void draw_circles(STATE_T *state)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Size of each vertex in bytes
    size_t vert_size = 4*sizeof(GL_FLOAT);

    glVertexAttribPointer(state->position_location, 2, GL_FLOAT, GL_FALSE, vert_size, 0);
    glEnableVertexAttribArray(state->position_location);
    glVertexAttribPointer(state->tex_coord_location, 2, GL_FLOAT, GL_FALSE, vert_size,(void*)(2*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(state->tex_coord_location);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);
}
