all:	circles

LDFLAGS+=-L$(SDKSTAGE)/opt/vc/lib/ -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -L../libs/ilclient -L../libs/vgfont
INCLUDES+=-I$(SDKSTAGE)/opt/vc/include/ -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux -I./ -I../libs/ilclient -I../libs/vgfont
CCFLAGS+= -mfloat-abi=hard -mfpu=vfp -ffast-math -O3
DEBUG+=-g -pg

.PHONY:	clean

circles:circles.c circles_gl.c egl_utils.c
	mkdir -p bin
	mpicc -O3 $(CCFLAGS) $(INCLUDES) $(LDFLAGS) egl_utils.c circles_gl.c circles.c -o bin/circles

debug:	circles egl_utils.c
	mkdir -p bin
	mpicc $(CCFLAGS) $(DEBUG) $(INCLUDES) $(LDFLAGS) pibrot.c circles_gl.c egl_utils.c -o bin/debug


clean:
	rm -rf *.o
