#include "common.h"

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


void ovg_mat_identity(void){
	vgLoadIdentity();
}
