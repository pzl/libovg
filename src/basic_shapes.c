#ifndef PI
#include <vg/openvg.h>
#include <vg/vgu.h>
#else
#include <VG/openvg.h>
#include <VG/vgu.h>
#endif
#include "drawlib.h"

void dl_rect(int x, int y, int w, int h){
	VGPaint p;

	p = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
	vguRect(p, x, y, w, h);
	vgDrawPath(p, VG_FILL_PATH | VG_STROKE_PATH);

	vgDestroyPath(p);

}

