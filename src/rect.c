#include <vg/openvg.h>
#include <vg/vgu.h>
#include "drawlib.h"

void dl_rect(int x, int y, int w, int h){
	VGPaint p;

	//set up colors
	VGfloat fill[4] = {1.0f, 0.2f, 0.2f, 1.0f};
	VGfloat stroke[4] = { 0.1f, 0.5f, 1.0f, 1.0f };
	VGPaint fillPaint = vgCreatePaint();
	VGPaint strokePaint = vgCreatePaint();

	//vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
	vgSetParameterfv(fillPaint, VG_PAINT_COLOR, 4, fill);
	vgSetPaint(fillPaint, VG_FILL_PATH);
	//vgSetParameteri(strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
	vgSetParameterfv(strokePaint, VG_PAINT_COLOR, 4, stroke);
	vgSetPaint(strokePaint, VG_STROKE_PATH);

	p = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
	vguRect(p, x, y, w, h);
	vgDrawPath(p, VG_FILL_PATH | VG_STROKE_PATH);

	vgDestroyPaint(fillPaint);
	vgDestroyPaint(strokePaint);
	vgDestroyPath(p);

}
