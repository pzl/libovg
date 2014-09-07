#ifndef PI
#include <vg/openvg.h>
#include <vg/vgu.h>
#else
#include <VG/openvg.h>
#include <VG/vgu.h>
#endif
#include "drawlib.h"

void dl_setFill(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	VGfloat fill[4] = {r/255.0f, g/255.0f, b/255.0f, a/255.0f};
	VGPaint fillPaint = vgCreatePaint();

	vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
	vgSetParameterfv(fillPaint, VG_PAINT_COLOR, 4, fill);
	vgSetPaint(fillPaint, VG_FILL_PATH);
}

void dl_setStroke(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	VGfloat stroke[4] = {r/255.0f, g/255.0f, b/255.0f, a/255.0f};
	VGPaint strokePaint = vgCreatePaint();

	vgSetParameteri(strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
	vgSetParameterfv(strokePaint, VG_PAINT_COLOR, 4, stroke);
	vgSetPaint(strokePaint, VG_STROKE_PATH);
}
