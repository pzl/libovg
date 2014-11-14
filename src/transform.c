#include "common.h"
#include "utility.h"

void ovg_translate(int x, int y){
	vgTranslate(x,y);
}
void ovg_rotate(int r){
	vgRotate(r);
}
void ovg_scale(float x, float y){
	vgScale(x,y);
}
void ovg_shear(int x, int y){
	vgShear(x,y);
}


void ovg_bounds(VGPath p, float *x, float *y, float *w, float *h){
	vgPathTransformedBounds(p,x,y,w,h);
}

void ovg_bounds_transformed(VGPath p, float *x, float *y, float *w, float *h){
	vgPathBounds(p,x,y,w,h);
}


void ovg_reset(void){
	vgLoadIdentity();
}

VGPath ovg_interpolate(VGPath start, VGPath end, float amount) {
	VGPath p = getpath();
	VGboolean success = vgInterpolatePath(p, start, end, amount);


	if (success == VG_TRUE){
		vgDrawPath(p, VG_STROKE_PATH | VG_FILL_PATH);
		return p;
	} else {
		return start;
	}
}
