//#include <stdio.h>
#include <VG/openvg.h>
#include <VG/vgu.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <bcm_host.h>
#include "drawlib.h"

#include <assert.h>

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

	//some sort of setup
	vgSetfv(VG_CLEAR_COLOR, 4, bg);
	vgClear(0,0,displayW,displayH);
}

static void init_surface(int x, int y, int w, int h){
	static EGL_DISPMANX_WINDOW_T window;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T dispman_update;
	VC_RECT_T dest;
	VC_RECT_T src;
	EGLBoolean result;

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

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, &window, NULL);
	assert(eglSurface != EGL_NO_SURFACE);

	// preserve the buffers on swap
	result = eglSurfaceAttrib(eglDisplay, eglSurface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);
	assert(EGL_FALSE != result);

	// connect the context to the surface
	result = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	assert(EGL_FALSE != result);

}



void dl_init(void){
	bcm_host_init();
	int32_t success = 0;
	EGLBoolean result;
	EGLint num_config;

	static const EGLint attribute_list[] = {
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};

	// get an EGL display connection
	eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(eglDisplay != EGL_NO_DISPLAY);

	// initialize the EGL display connection
	result = eglInitialize(eglDisplay, NULL, NULL);
	assert(EGL_FALSE != result);

	// bind OpenVG API
	eglBindAPI(EGL_OPENVG_API);

	// get an appropriate EGL frame buffer configuration
	result = eglChooseConfig(eglDisplay, attribute_list, &eglConfig, 1, &num_config);
	assert(EGL_FALSE != result);

	// create an EGL rendering context
	eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, NULL);
	assert(eglContext != EGL_NO_CONTEXT);

	// create an EGL window surface
	success = graphics_get_display_size(0 /* LCD */ , &displayW,
					    &displayH);
	assert(success >= 0);

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
	//eglDestroySurface(eglDisplay, eglSurface);
	
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
