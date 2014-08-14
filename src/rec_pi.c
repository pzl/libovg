#include <stdio.h>
#include <VG/openvg.h>
#include <VG/vgu.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <bcm_host.h>
#include "drawlib.h"

static EGLConfig eglConfig;
static EGLDisplay eglDisplay;
static EGLContext eglContext;
static EGLSurface eglSurface;

static uint32_t displayH;
static uint32_t displayW;


static void gfx_update(void){
	eglSwapBuffers(eglDisplay, eglSurface);
}

static void gfx_clear(void){
	VGfloat bg[4] = {1.0f,0.5f,0.1f,1};

	//set clear color and clear the screen
	vgSetfv(VG_CLEAR_COLOR, 4, bg);
	vgClear(0,0,displayW,displayH);
}

static void init_surface(int x, int y, int w, int h){
	static EGL_DISPMANX_WINDOW_T window;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T dispman_update;
	VC_RECT_T dest;
	VC_RECT_T src;

	dest.x = x;
	dest.y = y;
	dest.width = w;
	dest.height = h;

	src.x = 0;
	src.y = 0;
	src.width = w << 16; //setting these to displayW/H with smaller w/h leads to squishing
	src.height = h << 16;

	dispman_display = vc_dispmanx_display_open(0 /* LCD */ );
	dispman_update = vc_dispmanx_update_start(0);

	window.element = vc_dispmanx_element_add(dispman_update, dispman_display, 0 /*layer */ , &dest, 0 /*src */ ,
						  &src, DISPMANX_PROTECTION_NONE, 0 /*alpha */ , 0 /*clamp */ ,
						  0 /*transform */ );
	window.width = displayW;
	window.height = displayH;
	vc_dispmanx_update_submit_sync(dispman_update);

	if ((eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, &window, NULL)) == EGL_NO_SURFACE){
		fprintf(stderr, "DRAWLIB: Error Creating EGL window surface\n");
	}

	// preserve the buffers on swap
	if (eglSurfaceAttrib(eglDisplay, eglSurface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED) == EGL_FALSE){
		fprintf(stderr, "DRAWLIB: Error setting EGL Surface attributes\n");
	}
	if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE){
		fprintf(stderr, "DRAWLIB: Error setting current surface\n");
	}
}



void dl_init(void){
	bcm_host_init();
	int32_t success = 0;
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
		fprintf(stderr, "DRAWLIB: Error: Could not get EGL Display\n");
	}
	if (eglInitialize(eglDisplay, NULL, NULL) == EGL_FALSE){
		fprintf(stderr, "DRAWLIB: Error initializing EGL display\n");
	}
	eglBindAPI(EGL_OPENVG_API);

	if (eglChooseConfig(eglDisplay, attribute_list, &eglConfig, 1, &num_config) == EGL_FALSE){
		fprintf(stderr, "DRAWLIB: Error setting EGL buffer configuration\n");
	}
	if ((eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, NULL)) == EGL_NO_CONTEXT){
		fprintf(stderr, "DRAWLIB: Error creating EGL Content\n");
	}

	// create an EGL window surface
	success = graphics_get_display_size(0/*LCD*/, &displayW,&displayH);
	if (success < 0){
		fprintf(stderr, "DRAWLIB: Error getting display size\n");
	}

	init_surface(0,0,displayW,displayH);

	// set up screen ratio
	glViewport(0, 0, (GLsizei) displayW, (GLsizei) displayH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio = (float)displayW / (float)displayH;
	glFrustumf(-ratio, ratio, -1.0f, 1.0f, 1.0f, 10.0f);

	gfx_clear();

}

void dl_cleanup(void){
	eglDestroySurface(eglDisplay, eglSurface);
	
	//clear screen
	glClear(GL_COLOR_BUFFER_BIT);

	//release resources
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(eglDisplay, eglContext);
	eglTerminate(eglDisplay);
}

void dl_rect(int x, int y, int w, int h){
	vgLoadIdentity();

	//fill, stroke colors
	VGfloat fill[4] = {1.0, 0.2, 0.2, 1.0};
	VGfloat stroke[4] = { 0.1, 0.5, 1.0, 1.0 };

	VGPaint fillPaint = vgCreatePaint();
	vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
	vgSetParameterfv(fillPaint, VG_PAINT_COLOR, 4, fill);
	vgSetPaint(fillPaint, VG_FILL_PATH);
	vgDestroyPaint(fillPaint);

	VGPaint strokePaint = vgCreatePaint();
	vgSetParameteri(strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
	vgSetParameterfv(strokePaint, VG_PAINT_COLOR, 4, stroke);
	vgSetPaint(strokePaint, VG_STROKE_PATH);
	vgDestroyPaint(strokePaint);


	//do rect
	VGPath p = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
	vguRect(p, x, y, w, h);
	vgDrawPath(p, VG_FILL_PATH | VG_STROKE_PATH);
	vgDestroyPath(p);

	gfx_update();

}
