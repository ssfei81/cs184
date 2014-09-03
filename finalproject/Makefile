CC = g++  -I./ -L./ -I./ffmpeg/include -L./ffmpeg/lib/ -lfreeimage -stdlib=libc++ -wall
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL \
	  -L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++ -lavcodec -lavutil -lswscale -lavformat
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	LDFLAGS = -lglut -lGLU 
endif
	
RM = /bin/rm -f 
all: main 
main:  
	$(CC) $(CFLAGS) -o game main.cpp $(LDFLAGS) 
clean: 
	$(RM) *.o game
 


