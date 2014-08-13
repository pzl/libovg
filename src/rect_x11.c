#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "drawlib.h"

#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

static Display *x_display = NULL;


GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;
   
   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   	return 0;

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );
   
   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled ) 
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
         char* infoLog = malloc (sizeof(char) * infoLen );

         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         fprintf (stderr, "Error compiling shader:\n%s\n", infoLog );            
         
         free ( infoLog );
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;

}

static void createWindow(void){
	EGLint attribList[] = {
		EGL_RED_SIZE,       5,
		EGL_GREEN_SIZE,     6,
		EGL_BLUE_SIZE,      5,
		EGL_ALPHA_SIZE,     (0 & 1) ? 8 : EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     (0 & 2) ? 8 : EGL_DONT_CARE,
		EGL_STENCIL_SIZE,   (0 & 4) ? 8 : EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, (0 & 8) ? 1 : 0,
		EGL_NONE
	};


	/****************************
		Create XWindow
	*****************************/

	Window root;
	XSetWindowAttributes swa;
	XSetWindowAttributes xattr;
	Atom wm_state;
	XWMHints hints;
	XEvent xev;
	Window win;


	if ((x_display = XOpenDisplay(NULL)) == NULL){
		fprintf(stderr, "Error opening X Display\n");
		return;
	}

	root = DefaultRootWindow(x_display);
	swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;
	win = XCreateWindow(x_display,root,
	                    0,0,300,300,0,
	                    CopyFromParent,InputOutput,
	                    CopyFromParent, CWEventMask,
	                    &swa);

	xattr.override_redirect = 0;
	XChangeWindowAttributes(x_display, win, CWOverrideRedirect, &xattr);

	hints.input = 1;
	hints.flags = InputHint;
	XSetWMHints(x_display, win, &hints);

	XMapWindow(x_display, win);
	XStoreName(x_display, win, "MyWindow");

	wm_state = XInternAtom(x_display, "_NET_WM_STATE", 0);

	memset(&xev,0, sizeof(xev));
	xev.type = ClientMessage;
	xev.xclient.window = win;
	xev.xclient.message_type = wm_state;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = 1;
	xev.xclient.data.l[1] = 0;
	XSendEvent(x_display, DefaultRootWindow(x_display), 0, SubstructureNotifyMask, &xev);


	/**************************
		Create EGL Context
	***************************/

	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };



	// Get Display
	display = eglGetDisplay((EGLNativeDisplayType)x_display);
	if ( display == EGL_NO_DISPLAY ) {
		fprintf(stderr, "Could not get EGL Display\n");
		return;
	}


	// Initialize EGL
	if ( !eglInitialize(display, &majorVersion, &minorVersion) ) {
		fprintf(stderr, "Error getting EGL Version\n");
		return;
	}

	// Get configs
	if ( !eglGetConfigs(display, NULL, 0, &numConfigs) ) {
		fprintf(stderr, "Error getting EGL Configs\n");
		return;
	}

	// Choose config
	if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) ) {
		fprintf(stderr, "Error choosing EGL configs\n");
		return;
	}

	// Create a surface
	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)win, NULL);
	if ( surface == EGL_NO_SURFACE ) {
		fprintf(stderr, "Error creating EGL Surface\n");
		return;
	}

	// Create a GL context
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
	if ( context == EGL_NO_CONTEXT ) {
		fprintf(stderr, "Error creating EGL Context\n");
		return;
	}   

	// Make the context current
	if ( !eglMakeCurrent(display, surface, surface, context) ) {
		fprintf(stderr, "Error setting current EGL context\n");
		return;
	}

	
	/*******************************************
		Initialize Shader and program object
	********************************************/
	GLbyte vShaderStr[] =  
		"attribute vec4 vPosition;    \n"
		"void main()                  \n"
		"{                            \n"
		"   gl_Position = vPosition;  \n"
		"}                            \n";

	GLbyte fShaderStr[] =  
		"precision mediump float;\n"\
		"void main()                                  \n"
		"{                                            \n"
		"  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
		"}                                            \n";

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	// Load the vertex/fragment shaders
	vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
	fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

	// Create the program object
	programObject = glCreateProgram ( );

	if ( programObject == 0 ){
		fprintf(stderr, "Error creating program object\n");
		return;
	}

	glAttachShader ( programObject, vertexShader );
	glAttachShader ( programObject, fragmentShader );

	// Bind vPosition to attribute 0   
	glBindAttribLocation ( programObject, 0, "vPosition" );

	// Link the program
	glLinkProgram ( programObject );

	// Check the link status
	glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

	if ( !linked )  {
	  GLint infoLen = 0;

	  glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
	  
	  if ( infoLen > 1 )
	  {
	     char* infoLog = malloc (sizeof(char) * infoLen );

	     glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
	     fprintf (stderr, "Error linking program:\n%s\n", infoLog );            
	     
	     free ( infoLog );
	  }

	  glDeleteProgram ( programObject );
	  return;
	}

	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );



	/************************
		Draw a Triangle?
	*************************/
	GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f, 
							-0.5f, -0.5f, 0.0f,
							0.5f, -0.5f, 0.0f };
	
	// Set the viewport
	glViewport ( 0, 0, 300, 300);

	// Clear the color buffer
	glClear ( GL_COLOR_BUFFER_BIT );

	// Use the program object
	glUseProgram ( programObject );

	// Load the vertex data
	glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
	glEnableVertexAttribArray ( 0 );

	glDrawArrays ( GL_TRIANGLES, 0, 3 );

	eglSwapBuffers(display, surface);

	return;

}

void dl_init(void){

	createWindow();
}

void dl_cleanup(void){
	
}

void dl_rect(int x, int y, int w, int h){
	(void) x;
	(void) y;
	(void) w;
	(void) h;

}
