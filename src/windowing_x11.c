#include <stdio.h>
#include <stdlib.h>
#include <vg/openvg.h>
#include <vg/vgu.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include "ovg.h"

#include <unistd.h>

static Display *x_display = NULL;
Window win;
int displayW = 1366,
	displayH = 768;

void ovg_draw(void){
	glXSwapBuffers(x_display, win);
}

void ovg_clear(void){
	int x,y,w,h;

	//get screen dimensions
	ovg_wininfo(&x,&y,&w,&h);
	vgResizeSurfaceSH(w,h); //why am i doing this again?

	ovg_clear_rect(0,0,w,h);
}



void ovg_init(void){
	Window root;
	GLXContext glc;
	XSetWindowAttributes swa,xattr;
	XWindowAttributes gwa;
	XClassHint *classHint;
	XVisualInfo *vi;
	GLint att[] = {
		GLX_RGBA,
		GLX_DEPTH_SIZE, 24,
		GLX_DOUBLEBUFFER, None 
	};
	VGfloat bg[] = BG_COLOR;

	char instance[] = "blue",
		 className[] = "red";

	XInitThreads();

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

	xattr.override_redirect=0;
	XChangeWindowAttributes(x_display, win, CWOverrideRedirect, &xattr);

	XStoreName(x_display, win, "pi-unnamed");

	classHint = XAllocClassHint();
	if (classHint){
		classHint->res_name = instance;
		classHint->res_class = className;
	}
	XSetClassHint(x_display, win, classHint);
	XFree(classHint);

	XMapWindow(x_display, win);

	glc = glXCreateContext(x_display, vi, NULL, GL_TRUE);
	glXMakeCurrent(x_display, win, glc);

	// set up screen ratio
	XGetWindowAttributes(x_display, win, &gwa);

 	if (vgCreateContextSH(displayW, displayH) != VG_TRUE){
    	fprintf(stderr, "Error creating context\n");
    	return;
    }

    //set background color
	vgSetfv(VG_CLEAR_COLOR, 4, bg);

    ovg_clear();
}

void ovg_cleanup(void){
	
}


void ovg_wininfo(int *x, int *y, int *w, int *h){
	XWindowAttributes gwa;
	XGetWindowAttributes(x_display, win, &gwa);

	*x = gwa.x;
	*y = gwa.y;
	*w = gwa.width;
	*h = gwa.height;
}
