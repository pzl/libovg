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


void ovg_bounds(float *x, float *y, float *w, float *h){
	vgPathTransformedBounds(lastPath,x,y,w,h);
}

void ovg_bounds_transformed(float *x, float *y, float *w, float *h){
	vgPathBounds(lastPath,x,y,w,h);
}


void ovg_mat_identity(void){
	vgLoadIdentity();
}
