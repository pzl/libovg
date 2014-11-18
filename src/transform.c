#include <stddef.h> /* NULL */
#include <stdio.h>
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

void ovg_mat_mode(MatrixMode mode){
	vgSeti(VG_MATRIX_MODE,mode);
}

void ovg_mat_set(float *matrix){
	VGMatrixMode curmode = vgGeti(VG_MATRIX_MODE);
	float mcopy[9];

	//flip matrix
	mcopy[0] = matrix[0];
	mcopy[3] = matrix[1];
	mcopy[6] = matrix[2];
	mcopy[1] = matrix[3];
	mcopy[4] = matrix[4];
	mcopy[7] = matrix[5];

	//only need these elements if current matrix is not affine
	if (curmode == VG_MATRIX_IMAGE_USER_TO_SURFACE){
		mcopy[2] = matrix[6];
		mcopy[5] = matrix[7];
		mcopy[8] = matrix[8];
	} else {
		mcopy[2] = 0;
		mcopy[5] = 0;
		mcopy[8] = 1;
	}

	vgLoadMatrix(mcopy);
}

void ovg_mat_current(float *matrix){
	if (matrix == NULL){
		return;
	}
	VGfloat mcopy[9];
	vgGetMatrix(mcopy);
	matrix[0] = mcopy[0];
	matrix[1] = mcopy[3];
	matrix[2] = mcopy[6];
	matrix[3] = mcopy[1];
	matrix[4] = mcopy[4];
	matrix[5] = mcopy[7];
	matrix[6] = mcopy[2];
	matrix[7] = mcopy[5];
	matrix[8] = mcopy[8];

}

void ovg_mat_multiply(float *matrix){
	VGMatrixMode curmode = vgGeti(VG_MATRIX_MODE);
	float mcopy[9];

	//flip matrix
	mcopy[0] = matrix[0];
	mcopy[3] = matrix[1];
	mcopy[6] = matrix[2];
	mcopy[1] = matrix[3];
	mcopy[4] = matrix[4];
	mcopy[7] = matrix[5];

	//only need these elements if current matrix is not affine
	if (curmode == VG_MATRIX_IMAGE_USER_TO_SURFACE){
		mcopy[2] = matrix[6];
		mcopy[5] = matrix[7];
		mcopy[8] = matrix[8];
	} else {
		mcopy[2] = 0;
		mcopy[5] = 0;
		mcopy[8] = 1;
	}

	vgMultMatrix(mcopy);
}


VGPath ovg_interpolate(VGPath start, VGPath end, float amount) {
	int segstart,segend;
	VGPath p = getpath();
	VGboolean success = vgInterpolatePath(p, start, end, amount);


	if (success == VG_TRUE){
		return p;
	} else {
		segstart = vgGetParameteri(start, VG_PATH_NUM_SEGMENTS);
		segend = vgGetParameteri(end, VG_PATH_NUM_SEGMENTS);

		if (segstart != segend) {
			fprintf(stderr, "Unable to interpolate, number of segments much match between paths\n");
		}
		return start;
	}
}
