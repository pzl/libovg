#include <stddef.h> /* NULL */
#include "common.h"

static void _colorstops(VGPaint paint, int nstops, int rule, float *pts, unsigned char *colors);
static void _paint(int type, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
static void _getpaint(int type, unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a);

void ovg_fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	_paint(VG_FILL_PATH, r,g,b,a);
}
void ovg_stroke(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	_paint(VG_STROKE_PATH, r,g,b,a);
}

void ovg_fill_current(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a) {
	_getpaint(VG_FILL_PATH, r,g,b,a);
}
void ovg_stroke_current(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a) {
	_getpaint(VG_STROKE_PATH, r,g,b,a);
}


void ovg_dash(int *pattern, int n){
	if (n<=0 || pattern == NULL){
		vgSetfv(VG_STROKE_DASH_PATTERN, 0, (VGfloat *)0);
	} else {
		VGfloat pat[n];
		for (int i=0; i<n; i++){
			pat[i] = *pattern++;
		}
		vgSetfv(VG_STROKE_DASH_PATTERN, n, pat);
	}
}

void ovg_fill_rule(FillRule rule){
	vgSeti(VG_FILL_RULE, rule);
}

void ovg_clear_rect(int x, int y, int w, int h, CoordSystem c){
	VGfloat bg[] = BG_COLOR;
	//vgSeti(VG_SCISSORING, VG_FALSE); //disable scissoring to clear?
	//set clear color and clear the screen
	vgSetfv(VG_CLEAR_COLOR, 4, bg);

	if (c == C_ABSOLUTE) {
		vgClear(x,y,w,h);
	} else {
		float mat[9];
		int tx, ty, tw, th;
		/* @todo: what if matrix mode is not user to surface */
		ovg_mat_current(mat);

		
		tx = x*mat[0] + y*mat[1] + mat[2];
		ty = x*mat[3] + y*mat[4] + mat[5];
		tw = w*mat[0] + w*mat[1];
		th = h*mat[3] + h*mat[4];

		vgClear(tx,ty,tw,th);
	}
}

void ovg_quality(DrawQuality dq){
	vgSeti(VG_RENDERING_QUALITY, dq);
}

void ovg_stroke_width(float w) {
	vgSetf(VG_STROKE_LINE_WIDTH, w);
}

void ovg_stroke_cap(CapStyle c){
	vgSeti(VG_STROKE_CAP_STYLE, c);
}

void ovg_stroke_join(JoinStyle j){
	vgSeti(VG_STROKE_JOIN_STYLE,j);
}

void ovg_stroke_miter(float limit){
	vgSeti(VG_STROKE_MITER_LIMIT, limit);
}

void ovg_dash_phase(int phase){
	vgSetf(VG_STROKE_DASH_PHASE, phase);
}



void ovg_gradient_linear(int nstops, GradRule rule,
                         int sx, int sy, int ex, int ey,
                         float *pts, unsigned char *colors){

	VGfloat grad_points[4] = { sx, sy, ex, ey};
	VGPaint grad_paint = vgCreatePaint();
	vgSetParameteri(grad_paint,VG_PAINT_TYPE,VG_PAINT_TYPE_LINEAR_GRADIENT);
	vgSetParameterfv(grad_paint, VG_PAINT_LINEAR_GRADIENT,4,grad_points);
	_colorstops(grad_paint, nstops, rule, pts, colors);
	vgSetPaint(grad_paint, VG_FILL_PATH);
}

void ovg_gradient_radial(int nstops, GradRule rule,
                         int cx, int cy,
                         int fx, int fy, int r,
                         float *pts, unsigned char *colors){

	VGfloat grad_points[5] = { cx, cy, fx, fy, r};
	VGPaint grad_paint = vgCreatePaint();
	vgSetParameteri(grad_paint,VG_PAINT_TYPE,VG_PAINT_TYPE_RADIAL_GRADIENT);
	vgSetParameterfv(grad_paint, VG_PAINT_RADIAL_GRADIENT,5,grad_points);
	_colorstops(grad_paint, nstops, rule, pts, colors);
	vgSetPaint(grad_paint, VG_FILL_PATH);
}


void _colorstops(VGPaint grad_paint, int nstops, int rule, float *pts, unsigned char *colors){

	int i, j;
	VGfloat stops[5*nstops];

	for (i=0; i<nstops; i++){
		stops[i*5] = pts[i]; //copy decimal location
		for (j=0; j<4; j++){
			stops[i*5+j+1] = colors[i*4+j]/255.0f; //cpy 4 rgba colors, convert to float
		}
	}

	vgSetParameteri(grad_paint,VG_PAINT_COLOR_RAMP_PREMULTIPLIED, VG_FALSE);
	vgSetParameteri(grad_paint, VG_PAINT_COLOR_RAMP_SPREAD_MODE, rule);
	vgSetParameterfv(grad_paint, VG_PAINT_COLOR_RAMP_STOPS, 5*nstops, stops);
}

static void _paint(int type, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	VGfloat fill[4] = {r/255.0f, g/255.0f, b/255.0f, a/255.0f};
	VGPaint fillPaint = vgCreatePaint();

	vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
	vgSetParameterfv(fillPaint, VG_PAINT_COLOR, 4, fill);
	vgSetPaint(fillPaint, type);
}

static void _getpaint(int type, unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a) {
	VGfloat fill[4];
	VGPaintMode mode = (VGPaintMode) type;
	VGPaint fillPaint = vgGetPaint(mode);

	vgGetParameterfv(fillPaint, VG_PAINT_COLOR, 4, fill);
	*r = (unsigned char) (fill[0]*255.0f + 0.5f);
	*g = (unsigned char) (fill[1]*255.0f + 0.5f);
	*b = (unsigned char) (fill[2]*255.0f + 0.5f);
	*a = (unsigned char) (fill[3]*255.0f + 0.5f);
}

