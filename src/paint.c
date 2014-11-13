#include <stddef.h> /* NULL */
#include "common.h"

void ovg_fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	VGfloat fill[4] = {r/255.0f, g/255.0f, b/255.0f, a/255.0f};
	VGPaint fillPaint = vgCreatePaint();

	vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
	vgSetParameterfv(fillPaint, VG_PAINT_COLOR, 4, fill);
	vgSetPaint(fillPaint, VG_FILL_PATH);
}

void ovg_stroke(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	VGfloat stroke[4] = {r/255.0f, g/255.0f, b/255.0f, a/255.0f};
	VGPaint strokePaint = vgCreatePaint();

	vgSetParameteri(strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
	vgSetParameterfv(strokePaint, VG_PAINT_COLOR, 4, stroke);
	vgSetPaint(strokePaint, VG_STROKE_PATH);
}

void ovg_dash(int *pattern, int n){
	if (n<=0 || pattern == NULL){
		vgSetfv(VG_STROKE_DASH_PATTERN, 0, (VGfloat *)0);
	} else {
		VGfloat pat[n];
		VGfloat *p = pat;
		while (n--){
			*p++ = *pattern++;
		}
		vgSetfv(VG_STROKE_DASH_PATTERN, n, pat);
	}
}

void ovg_clear_rect(int x, int y, int w, int h){
	VGfloat bg[] = BG_COLOR;

	//vgSeti(VG_SCISSORING, VG_FALSE); //disable scissoring to clear?

	//set clear color and clear the screen
	vgSetfv(VG_CLEAR_COLOR, 4, bg);
	vgClear(x,y,w,h);
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
