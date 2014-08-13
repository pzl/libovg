//#include <stdio.h>
#include <VG/openvg.h>
#include <VG/vgu.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
//#include <bcm_host.h>
#include "drawlib.h"

#include <assert.h>

static EGLConfig eglConfig;
static EGLDisplay eglDisplay;
static EGLContext eglContext;

static int displayH;
static int displayW;


static void gfx_update(EGLSurface sf){
	eglSwapBuffers(eglDisplay, sf);
}

static void init_surface(int x, int y, int w, int h){
	VC_RECT_T dest;
	VC_RECT_T src;
	DISPMANX_DISPLAY_HANDLE_T dispmanDisplay;
	DISPMANX_UPDATE_HANDLE_T dispmanUpdate;

	EGL_DISPMANX_WINDOW_T dispmanWindow;
	EGLSurface eglSurface;
	EGLBoolean eglResult;

	dest.x=x;
	dest.y=displayH - y - h;
	dest.width=w;
	dest.height=h;

	src.x=0;
	src.y=0;
	src.width = w << 16;
	src.height = h << 16;

	dispmanDisplay = vc_dispmanx_display_open(0);
	dispmanUpdate = vc_dispmanx_update_start(0);

	dispmanWindow.element = vc_dispmanx_element_add(dispmanUpdate, dispmanDisplay, 0/*layer*/, 
													&dest, 0/*src*/, &src, DISPMANX_PROTECTION_NONE,
													0/*alpha*/, 0/*clamp*/, 0/*transform*/);
	dispmanWindow.width = w;
	dispmanWindow.height = h;
	vc_dispmanx_update_submit_sync(dispmanUpdate);

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, &dispmanWindow, NULL);
	assert(eglSurface != EGL_NO_SURFACE);

	eglResult = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	assert(EGL_FALSE != eglResult);

	eglSurfaceAttrib(eglDisplay, eglSurface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);

	gfx_update(eglSurface);

}


void dl_init(void){

	/************************
		Open Display
	*************************/
	int32_t success = 0;
	unsigned int width, height;

	EGLBoolean eglResult;
	EGLint eglNum_config;
	const EGLint eglAttrib_list[] = {
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};

	bcm_host_init();


	eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(eglDisplay != EGL_NO_DISPLAY);

	eglResult = eglInitialize(eglDisplay, NULL, NULL);
	assert(EGL_FALSE != eglResult);

	eglBindAPI(EGL_OPENVG_API);

	eglResult = eglChooseConfig(eglDisplay, eglAttrib_list, &eglConfig, 1, &eglNum_config);
	assert(EGL_FALSE != eglResult);

	eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, NULL);
	assert(eglContext != EGL_NO_CONTEXT);

	success = graphics_get_display_size(0/*LCD*/, &width, &height);
	assert(success >= 0);

	glViewport(0,0,(GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio = (float)width/(float)height;
	glFrustumf(-ratio,ratio,-1.0f, 1.0f, 1.0f, 10.0f);

	displayH = height;
	displayW = width;

	/**************************
		Initialize Surface
	***************************/
	init_surface(0,0,width,height);
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
	(void) x;
	(void) y;
	(void) w;
	(void) h;

}
