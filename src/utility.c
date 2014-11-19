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

void ovg_mask(VGPath p, AlphaMask mask) {
#ifdef OPENVG_VERSION_1_1
	vgSeti(VG_MASKING, VG_TRUE);
	vgRenderToMask(p, VG_FILL_PATH, mask);
#else
	//only vgMask supported
	(void) p;
	(void) mask;
	fprintf(stderr, "ovg_mask not yet supported(OpenVG 1.0, no vgRenderToMask)\n");
#endif
}

void ovg_mask_off(void) {
#ifdef OPENVG_VERSION_1_1
	vgRenderToMask(VG_INVALID_HANDLE, VG_FILL_PATH, VG_FILL_MASK);
#endif
	vgSeti(VG_MASKING, VG_FALSE);
}

void ovg_blend(BlendMode bm){
	vgSeti(VG_BLEND_MODE, bm);
}

float ovg_length(VGPath p, int start, int n){
	return vgPathLength(p,start,n);
}
