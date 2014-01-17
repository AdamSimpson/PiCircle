#include "egl_utils.h"
#include "circles_gl.h"
#include "stdio.h"
#include "linux/input.h"

int main(int argc, char *argv[])
{
        // Setup initial OpenGL ES state
        STATE_T state;
        memset(&state, 0, sizeof(STATE_T));

        bcm_host_init();

        // Start OGLES
        init_ogl(&state.egl_state);

        float points[] = {
             // Center(x,y) Color(r,g,b)
	     -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	      0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
	      0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
	     -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
	};

        // Create and bind points
        create_points(points, 4, &state);

        // Create and set shaders
        create_shaders(&state);

	while(1){
            draw_circles(&state);
            egl_swap(&state.egl_state);
	};

	exit_ogl(&state.egl_state);

    return 0;
}
