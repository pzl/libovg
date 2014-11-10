#ifndef PI
#include <vg/openvg.h>
#include <vg/vgu.h>
#else
#include <VG/openvg.h>
#include <VG/vgu.h>
#endif
#include "ovg.h"

void dl_rect(int x, int y, int w, int h){
	VGPaint p;

	p = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
	vguRect(p, x, y, w, h);
	vgDrawPath(p, VG_FILL_PATH | VG_STROKE_PATH);

	vgDestroyPath(p);

}

void dl_line(int x0, int y0, int x1, int y1){
	VGPaint p;
	p = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
	vguLine(p, x0,y0,x1,y1);
	vgDrawPath(p, VG_STROKE_PATH);

	vgDestroyPath(p);
}

void dl_round_rect(int x, int y, int w, int h, int r){
	VGPaint p;
	p = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
	vguRoundRect(p,x,y,w,h,r,r);
	vgDrawPath(p, VG_FILL_PATH | VG_STROKE_PATH);
	vgDestroyPath(p);
}

void dl_circle(int cx, int cy, int r){
	dl_ellipse(cx,cy,r*2,r*2);
}

void dl_ellipse(int cx, int cy, int w, int h){
	VGPaint p;
	p = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
	vguEllipse(p,cx,cy,w,h);
	vgDrawPath(p, VG_FILL_PATH | VG_STROKE_PATH);
	vgDestroyPath(p);
}
