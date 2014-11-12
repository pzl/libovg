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
