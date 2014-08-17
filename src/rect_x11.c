#include <stdio.h>
#include <stdlib.h>
#include <vg/openvg.h>
#include <vg/vgu.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include "drawlib.h"

#include <unistd.h>

static Display *x_display = NULL;
Window win;
int displayW = 1366,
	displayH = 768;

void dl_draw(void){
	glXSwapBuffers(x_display, win);
}

void dl_clear(void){
	vgClear(0,0,displayW,displayH);
}

void dl_init(void){
	Window root;
	GLXContext glc;
	XSetWindowAttributes swa,xattr;
	XWindowAttributes gwa;
	XVisualInfo *vi;
	GLint att[] = {
		GLX_RGBA,
		GLX_DEPTH_SIZE, 24,
		GLX_DOUBLEBUFFER, None 
	};

	if ((x_display = XOpenDisplay(NULL)) == NULL){
		fprintf(stderr, "Error opening X Display\n");
		return;
	}
	root = DefaultRootWindow(x_display);
	if ((vi = glXChooseVisual(x_display, 0, att)) == NULL){
		fprintf(stderr, "No visual found\n");
		return;
	}

	swa.event_mask = ExposureMask | KeyPressMask;
	win = XCreateWindow(x_display,root,
	                    15,35,displayW,displayH,0,
	                    CopyFromParent,InputOutput,
	                    CopyFromParent, CWEventMask,
	                    &swa);

	xattr.override_redirect=1;
	XChangeWindowAttributes(x_display, win, CWOverrideRedirect, &xattr);

	XMapWindow(x_display, win);

	glc = glXCreateContext(x_display, vi, NULL, GL_TRUE);
	glXMakeCurrent(x_display, win, glc);

	// set up screen ratio
	XGetWindowAttributes(x_display, win, &gwa);

 	if (vgCreateContextSH(displayW, displayH) != VG_TRUE){
    	fprintf(stderr, "Error creating context\n");
    	return;
    }

	VGfloat bg[4] = {1.0f,0.5f,0.1f,1};
	vgSetfv(VG_CLEAR_COLOR, 4, bg);

    dl_clear();
}

void dl_cleanup(void){
	
}
