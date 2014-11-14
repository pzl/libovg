#include "common.h"
#include "utility.h"


static void _zip(int *, int *, int n, VGfloat *result);

VGPath ovg_rect(int x, int y, int w, int h){
	VGPath p = getpath();
	vguRect(p, x, y, w, h);
	vgDrawPath(p, VG_FILL_PATH | VG_STROKE_PATH);
	return p;
}

VGPath ovg_line(int x0, int y0, int x1, int y1){
	VGPath p;
	p = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
	vguLine(p, x0,y0,x1,y1);
	vgDrawPath(p, VG_STROKE_PATH);
	return p;
}

VGPath ovg_round_rect(int x, int y, int w, int h, int r){
	VGPath p = getpath();
	vguRoundRect(p,x,y,w,h,r,r);
	vgDrawPath(p, VG_FILL_PATH | VG_STROKE_PATH);
	return p;
}

VGPath ovg_circle(int cx, int cy, int r){
	return ovg_ellipse(cx,cy,r*2,r*2);
}

VGPath ovg_ellipse(int cx, int cy, int w, int h){
	VGPath p = getpath();
	vguEllipse(p,cx,cy,w,h);
	vgDrawPath(p, VG_FILL_PATH | VG_STROKE_PATH);
	return p;
}

VGPath ovg_polyline(int *x, int *y, int n) {
	VGPath p = getpath();
	VGfloat points[n*2];
	_zip(x,y,n,points);
	vguPolygon(p,points,n, VG_FALSE);
	vgDrawPath(p, VG_STROKE_PATH);
	return p;
}

VGPath ovg_polygon(int *x, int *y, int n) {
	VGPath p = getpath();
	VGfloat points[n*2];
	_zip(x,y,n,points);
	vguPolygon(p,points,n, VG_TRUE);
	vgDrawPath(p, VG_FILL_PATH | VG_STROKE_PATH);
	return p;
}

VGPath ovg_arc(int x, int y, int w, int h, int sa, int ea){
	VGPath p = getpath();
	vguArc(p, x, y, w, h, sa, ea, VGU_ARC_OPEN); //@todo: other arc types
	vgDrawPath(p, VG_STROKE_PATH | VG_FILL_PATH);
	return p;
}

VGPath ovg_bezier_quad(int sx, int sy, int cx, int cy, int ex, int ey) {
	VGPath p = getpath();
	VGubyte commands[2] = {VG_MOVE_TO_ABS,VG_QUAD_TO};
	VGfloat points[6] = {sx,sy,cx,cy,ex,ey};

	vgAppendPathData(p,2,commands,points);
	vgDrawPath(p, VG_STROKE_PATH);
	return p;
}

VGPath ovg_bezier_cube(int sx, int sy,
                     int c1x, int c1y, int c2x, int c2y,
                     int ex, int ey) {
	VGPath p = getpath();
	VGubyte commands[2] = {VG_MOVE_TO_ABS,VG_CUBIC_TO};
	VGfloat points[8] = {sx,sy,c1x,c1y,c2x,c2y,ex,ey};

	vgAppendPathData(p,2,commands,points);
	vgDrawPath(p, VG_STROKE_PATH);
	return p;
}


static void _zip(int *x, int *y, int n, VGfloat *result){
	while (n--){
		*result++ = *x++;
		*result++ = *y++;
	}
}
