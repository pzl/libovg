#include <stdio.h>
#include <VG/openvg.h>
#include <VG/vgu.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <bcm_host.h>
#include "ovg.h"

static EGLConfig eglConfig;
static EGLDisplay eglDisplay;
static EGLSurface eglSurface;

void ovg_draw(void){
	eglSwapBuffers(eglDisplay, eglSurface);
}

void ovg_clear(void){
	int x,y,w,h;
	ovg_wininfo(&x,&y,&w,&h);
	ovg_clear_rect(0,0,w,h);
}

void ovg_wininfo(int *x, int *y, int *w, int *h){
	*x=0;
	*y=0;
	*w=1366;
	*h=768;
}

void ovg_dispinfo(unsigned int *w, unsigned int *h) {
	int32_t success = graphics_get_display_size(0/*Displau num*/, w, h);
	if (success < 0){
		fprintf(stderr, "LibOVG: Error getting display size\n");
	}
}

void ovg_init(void){
	bcm_host_init();
	EGLContext eglContext;
	EGLint num_config;

	static const EGLint attribute_list[] = {
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};

	if ((eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY){
		fprintf(stderr, "LibOVG: Error: Could not get EGL Display\n");
	}
	if (eglInitialize(eglDisplay, NULL, NULL) == EGL_FALSE){
		fprintf(stderr, "LibOVG: Error initializing EGL display\n");
	}
	eglBindAPI(EGL_OPENVG_API);

	if (eglChooseConfig(eglDisplay, attribute_list, &eglConfig, 1, &num_config) == EGL_FALSE){
		fprintf(stderr, "LibOVG: Error setting EGL buffer configuration\n");
	}
	if ((eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, NULL)) == EGL_NO_CONTEXT){
		fprintf(stderr, "LibOVG: Error creating EGL Content\n");
	}
}

void ovg_cleanup(void){
	EGLContext eglContext = eglGetCurrentContext();
	eglDestroySurface(eglDisplay, eglSurface);
	

	//clear screen
	glClear(GL_COLOR_BUFFER_BIT);

	//release resources
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(eglDisplay, eglContext);
	eglTerminate(eglDisplay);
}

void ovg_open(int x, int y, int w, int h){
	static EGL_DISPMANX_WINDOW_T window;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T dispman_update;
	VC_RECT_T dest;
	VC_RECT_T src;
	EGLContext eglContext = eglGetCurrentContext();

	dest.x = x;
	dest.y = y;
	dest.width = w;
	dest.height = h;

	src.x = x;
	src.y = y;
	src.width = w << 16; //setting these to displayW/H with smaller w/h leads to squishing
	src.height = h << 16;

	dispman_display = vc_dispmanx_display_open(0 /* LCD */ );
	dispman_update = vc_dispmanx_update_start(10);

	window.element = vc_dispmanx_element_add(dispman_update, dispman_display, 0 /*layer */ , &dest, 0 /*src */ ,
						  &src, DISPMANX_PROTECTION_NONE, 0 /*alpha */ , 0 /*clamp */ ,
						  0 /*transform */ );
	window.width = w;
	window.height = h;
	vc_dispmanx_update_submit_sync(dispman_update);

	if ((eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, &window, NULL)) == EGL_NO_SURFACE){
		fprintf(stderr, "LibOVG: Error Creating EGL window surface\n");
	}

	// preserve the buffers on swap
	if (eglSurfaceAttrib(eglDisplay, eglSurface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED) == EGL_FALSE){
		fprintf(stderr, "LibOVG: Error setting EGL Surface attributes\n");
	}
	if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE){
		fprintf(stderr, "LibOVG: Error setting current surface\n");
	}

	// set up screen ratio
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio = (float)w / (float)h;
	glFrustumf(-ratio, ratio, -1.0f, 1.0f, 1.0f, 10.0f);

	ovg_clear();
}

