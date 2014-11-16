#include <stddef.h>
#include <stdio.h>
#include "common.h"

VGPath getpath(void){
	return vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
}

void ovg_free(VGPath p) {
	vgDestroyPath(p);
}

VGPath ovg_draw_path(VGPath p, PaintMode pm) {
	vgDrawPath(p, pm);
	return p;
}
