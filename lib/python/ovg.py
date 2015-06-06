#!/usr/bin/env python3

from ctypes.util import find_library
from ctypes import *


lib = CDLL(find_library("ovg"))

lib.ovg_init()

"""
Python-wrapper-specific helpers
"""
class Color(Structure):
	_fields_ = [ ("r",c_ubyte),
				("g",c_ubyte),
				("b",c_ubyte),
				("a",c_ubyte)]




"""
Mirrors of C Enums
"""


		


class PaintMode(object):
	Stroke = (1<<0)
	Fill   = (1<<1)

class Quality(object):
	Non_AA = 0x1200
	Fast   = 0x1201
	Best   = 0x1202

class MatrixMode(object):
	Path   = 0x1400
	Image  = 0x1401
	Fill   = 0x1402
	Stroke = 0x1403

class Alpha(object):
	Clear     = 0x1500
	Fill      = 0x1501
	Set       = 0x1502
	Union     = 0x1503
	Intersect = 0x1504
	Subtract  = 0x1505

class Cap(object):
	Butt   = 0x1700
	Round  = 0x1701
	Square = 0x1702

class Join(object):
	Miter = 0x1800
	Round = 0x1801
	Bevel = 0x1802

class FillRule(object):
	Alternate = 0x1900
	All       = 0x1901

class GradientRule(object):
	Pad     = 0x1C00
	Repeat  = 0x1C01
	Reflect = 0x1C02


class Blend(object):
	Src      = 0x2000
	Src_Over = 0x2001
	Dst_Over = 0x2002
	Src_In   = 0x2003
	Dst_In   = 0x2004
	Multiply = 0x2005
	Screen   = 0x2006
	Darken   = 0x2007
	Lighten  = 0x2008
	Add      = 0x2009


class Coord(object):
	ABS = 0
	REL = 1


class PathSegment(object):
	Close_Path = ( 0 << 1),
	Move_To    = ( 1 << 1),
	Line_To    = ( 2 << 1),
	HLine_To   = ( 3 << 1),
	VLine_To   = ( 4 << 1),
	Quad_To    = ( 5 << 1),
	Cubic_To   = ( 6 << 1),
	SQuad_To   = ( 7 << 1),
	SCubic_To  = ( 8 << 1),
	SCCWArc_To = ( 9 << 1),
	SCWArc_To  = (10 << 1),
	LCCWArc_To = (11 << 1),
	LCWArc_To  = (12 << 1)


"""
Shortens up MOVE_TO_REL vs MOVE_TO_ABS, CUBIC_TO_ABS vs CUBIC_TO_REL, etc
to abs(Move_To), rel(Move_To)
"""

def abs(command):
	return command | Coord.ABS

def rel(command):
	return command | Coord.REL






"""
Management Commands
"""

"""
lib.ovg_init.argtypes = None
lib.ovg_init.restype = None
def init():
	return lib.ovg_init()
"""


lib.ovg_open.argtypes = [c_int, c_int, c_int, c_int]
lib.ovg_open.restype = None
def open(x ,y, w, h):
	return lib.ovg_open(x,y,w,h)


lib.ovg_wininfo.argtypes = [POINTER(c_int),POINTER(c_int),POINTER(c_int),POINTER(c_int)]
lib.ovg_wininfo.restype = None
def wininfo():
	x = c_int()
	y = c_int()
	w = c_int()
	h = c_int()
	lib.ovg_wininfo(byref(x), byref(y), byref(w), byref(h))
	return (x.value,y.value,w.value,h.value)


lib.ovg_dispinfo.argtypes = [POINTER(c_uint), POINTER(c_uint)]
lib.ovg_dispinfo.restype = None
def dispinfo():
	w = c_uint()
	h = c_uint()
	lib.ovg_dispinfo(byref(w),byref(h))
	return (w.value,h.value)


lib.ovg_quality.argtypes = [c_int]
lib.ovg_quality.restype = None
#uses Quality
def quality(q):
	return lib.ovg_quality(q)


lib.ovg_free.argtypes = [c_void_p]
lib.ovg_free.restype = None
def free(path):
	return lib.ovg_free(path)


lib.ovg_cleanup.argtypes = None
lib.ovg_cleanup.restype = None
def cleanup():
	return lib.ovg_cleanup()


lib.ovg_draw.argtypes = None
lib.ovg_draw.restype = None
def draw():
	return lib.ovg_draw()

lib.ovg_draw_path.argtypes = [c_void_p, c_int]
lib.ovg_draw_path.restype = c_void_p
def draw_path(path, mode):
	return lib.ovg_draw_path(path, mode)


lib.ovg_clear.argtypes = None
lib.ovg_clear.restype = None
def clear():
	return lib.ovg_clear()


lib.ovg_clear_rect.argtypes = [c_int, c_int, c_int, c_int, c_int]
lib.ovg_clear_rect.restype = None
#uses Coord
def clear_rect(x,y,w,h,c):
	return lib.ovg_clear_rect(x,y,w,h,c)





"""
	------------------------------------
	Fills n Strokes
	------------------------------------
"""


lib.ovg_fill.argtypes = [c_ubyte, c_ubyte, c_ubyte, c_ubyte]
lib.ovg_fill.restype = None
def fill(*args, **kwargs):
	"""
	Set Fill Color, as values between 0-255

	Can accept:
		- a color object (anything with properties r,g,b,a)
		- 4 ints as red,green,blue,alpha in order
		- named arguments (red=255,green=177...)

	Named arguments may also be presented in addition to a Color object
	or ordered parameters, and take precedence. Can be used as overrides.
	"""
	if len(args) == 1:
		red = c_ubyte(args[0].r)
		green = c_ubyte(args[0].g)
		blue = c_ubyte(args[0].b)
		alpha = c_ubyte(args[0].a)
	elif len(kwargs.keys()) < 4:
		red = c_ubyte(args[0])
		green = c_ubyte(args[1])
		blue = c_ubyte(args[2])
		alpha = c_ubyte(args[3])	

	if 'red' in kwargs:
		red = c_ubyte(kwargs['red'])
	if 'green' in kwargs:
		green = c_ubyte(kwargs['green'])
	if 'blue' in kwargs:
		blue = c_ubyte(kwargs['blue'])
	if 'alpha' in kwargs:
		alpha = c_ubyte(kwargs['alpha'])

	return lib.ovg_fill(red, green, blue, alpha)




lib.ovg_fill_current.argtypes = [POINTER(c_ubyte), POINTER(c_ubyte), POINTER(c_ubyte), POINTER(c_ubyte)]
lib.ovg_fill_current.restype = None
def fill_current():
	r = c_ubyte()
	g = c_ubyte()
	b = c_ubyte()
	a = c_ubyte()
	lib.ovg_fill_current(byref(r),byref(g),byref(b),byref(a))
	return Color(r,g,b,a)


lib.ovg_stroke.argtypes = [c_ubyte, c_ubyte, c_ubyte, c_ubyte]
lib.ovg_stroke.restype = None
#expects Color
def stroke(*args, **kwargs):
	"""
	Set Stroke Color, as values between 0-255

	Can accept:
		- a color object (anything with properties r,g,b,a)
		- 4 ints as red,green,blue,alpha in order
		- named arguments (red=255,green=177...)

	Named arguments may also be presented in addition to a Color object
	or ordered parameters, and take precedence. Can be used as overrides.
	"""
	if len(args) == 1:
		red = c_ubyte(args[0].r)
		green = c_ubyte(args[0].g)
		blue = c_ubyte(args[0].b)
		alpha = c_ubyte(args[0].a)
	elif len(kwargs.keys()) < 4:
		red = c_ubyte(args[0])
		green = c_ubyte(args[1])
		blue = c_ubyte(args[2])
		alpha = c_ubyte(args[3])

	if 'red' in kwargs:
		red = c_ubyte(kwargs['red'])
	if 'green' in kwargs:
		green = c_ubyte(kwargs['green'])
	if 'blue' in kwargs:
		blue = c_ubyte(kwargs['blue'])
	if 'alpha' in kwargs:
		alpha = c_ubyte(kwargs['alpha'])
	return lib.ovg_stroke(red, green, blue, alpha)


lib.ovg_stroke_current.argtypes = [POINTER(c_ubyte), POINTER(c_ubyte), POINTER(c_ubyte), POINTER(c_ubyte)]
lib.ovg_stroke_current.restype = None
def stroke_current():
	r = c_ubyte()
	g = c_ubyte()
	b = c_ubyte()
	a = c_ubyte()
	lib.ovg_stroke_current(byref(r),byref(g),byref(b),byref(a))
	return Color(r,g,b,a)


lib.ovg_stroke_width.argtypes = [c_float]
lib.ovg_stroke_width.restype = None
def stroke_width(f):
	return lib.ovg_stroke_width(f)


lib.ovg_stroke_cap.argtypes = [c_int]
lib.ovg_stroke_cap.restype = None
def stroke_cap(c):
	return lib.ovg_stroke_cap(c)


lib.ovg_stroke_join.argtypes = [c_int]
lib.ovg_stroke_join.restype = None
def stroke_join(j):
	return lib.ovg_stroke_join(j)

lib.ovg_stroke_miter.argtypes = [c_float]
lib.ovg_stroke_miter.restype = None
def stroke_miter(limit):
	return lib.ovg_stroke_miter(limit)


lib.ovg_dash.argtypes = [POINTER(c_int), c_int]
lib.ovg_dash.restype = None
#takes list, tuple, etc
def dash(d):
	if d and len(d):
		pattern = (c_int * len(d))(*d)
		lp = len(d)
	else:
		pattern = None
		lp = 0

	return lib.ovg_dash(pattern, lp)

lib.ovg_dash_phase.argtypes = [c_int]
lib.ovg_dash_phase.restype = None
def dash_phase(p):
	return lib.ovg_dash_phase(p)




lib.ovg_gradient_linear.argtypes = [c_int, c_int, c_int, c_int, c_int, c_int, POINTER(c_float), POINTER(c_ubyte)]
lib.ovg_gradient_linear.restype = None
def linear_gradient(stops, rule, startx, starty, endx, endy, points, colors):

	if len(points) < 1:
		print("There definitely can't be a 0-point gradient.")

	if len(points) < 2:
		print("Can there be a 1-point gradient?")

	if len(colors) != len(points)*4:
		print("There must be 4 colors per gradient point!")


	p = (c_float * len(points))(*points)
	c = (c_ubyte * len(points)*4)(*colors)


	return lib.ovg_gradient_linear(stops, rule, startx, starty, endx, endy, p, c)


lib.ovg_gradient_radial.argtypes = [c_int, c_int, c_int, c_int, c_int, c_int, c_int, POINTER(c_float), POINTER(c_ubyte)]
lib.ovg_gradient_radial.restype = None
def radial_gradient(stops, rule, cx, cy, fx, fy, rad, points, colors):

	if len(points) < 1:
		print("There definitely can't be a 0-point gradient.")

	if len(points) < 2:
		print("Can there be a 1-point gradient?")

	if len(colors) != len(points)*4:
		print("There must be 4 colors per gradient point!")


	p = (c_float * len(points))(*points)
	c = (c_ubyte * len(points)*4)(*colors)


	return lib.ovg_gradient_radial(stops, rule, cx, cy, fx, fy, rad, p, c)


lib.ovg_fill_rule.argtypes = [c_int]
lib.ovg_fill_rule.restype = None
def fill_rule(r):
	return lib.ovg_fill_rule(r)



"""
Shapes and Objects
"""

lib.ovg_line.argtypes = [c_int, c_int, c_int, c_int]
lib.ovg_line.restype = c_void_p
def line(x,y):
	return lib.ovg_line(x[0], y[0], x[1], y[1])


lib.ovg_polyline.argtypes = [POINTER(c_int), POINTER(c_int), c_int]
lib.ovg_polyline.restype = c_void_p
def polyline(x,y):
	lx = len(x)
	ly = len(y)


	if lx != ly:
		print("coordinates must match in length")

	if lx < 1 or ly < 1:
		print("must have at least one coordinate pair")

	px = (c_int * lx)(*x)
	py = (c_int * ly)(*y)

	return lib.ovg_polyline(px, py, lx)


lib.ovg_polygon.argtypes = [POINTER(c_int), POINTER(c_int), c_int]
lib.ovg_polygon.restype = c_void_p
def polygon(x,y):
	lx = len(x)
	ly = len(y)


	if lx != ly:
		print("coordinates must match in length")

	if lx < 1 or ly < 1:
		print("must have at least one coordinate pair")

	px = (c_int * lx)(*x)
	py = (c_int * ly)(*y)

	return lib.ovg_polygon(px, py, lx)


lib.ovg_rect.argtypes = [c_int, c_int, c_int, c_int]
lib.ovg_rect.restype = c_void_p
def rect(x,y,w,h):
	return lib.ovg_rect(x,y,w,h)


lib.ovg_round_rect.argtypes = [c_int, c_int, c_int, c_int, c_int]
lib.ovg_round_rect.restype = c_void_p
def round_rect(x,y,w,h,r):
	return lib.ovg_round_rect(x,y,w,h,r)


lib.ovg_circle.argtypes = [c_int, c_int, c_int]
lib.ovg_circle.restype = c_void_p
def circle(cx,cy,r):
	return lib.ovg_circle(cx,cy,r)


lib.ovg_ellipse.argtypes = [c_int, c_int, c_int, c_int]
lib.ovg_ellipse.restype = c_void_p
def ellipse(cx,cy,w,h):
	return lib.ovg_ellipse(cx,cy,w,h)


lib.ovg_arc.argtypes = [c_int, c_int, c_int, c_int, c_int, c_int]
lib.ovg_arc.restype = c_void_p
def arc(x,y,w,h, sa, travel):
	return lib.ovg_arc(x,y,w,h, sa, travel)


lib.ovg_bezier_quad.argtypes = [c_int, c_int, c_int, c_int, c_int, c_int]
lib.ovg_bezier_quad.restype = c_void_p
def bezier_quad(sx,sy,cx,cy,ex,ey):
	return lib.ovg_bezier_quad(sx,sy,cx,cy,ex,ey)


lib.ovg_bezier_cube.argtypes = [c_int, c_int, c_int, c_int, c_int, c_int, c_int, c_int]
lib.ovg_bezier_cube.restype = c_void_p
def bezier_cube(sx,sy,cx,cy,ex,ey):
	return lib.ovg_bezier_cube(sx,sy,cx[0],cy[0],cx[1],cy[1],ex,ey)


lib.ovg_path.argtypes = [c_int, POINTER(c_ubyte), POINTER(c_float)]
lib.ovg_path.restype = c_void_p
def path(commands, data):
	lc = len(commands)
	ld = len(data)

	c = (c_ubyte * lc)(*commands)
	d = (c_float * ld)(*data)

	return lib.ovg_path(lc, c, d)


lib.ovg_text.argtypes = [c_int, c_int, c_char_p, c_int]
lib.ovg_text.restype = None
def text(x,y,string,size):
	return lib.ovg_text(x,y,string,size)


"""
Utility Commands
"""
lib.ovg_interpolate.argtypes = [c_void_p, c_void_p, c_float]
lib.ovg_interpolate.restype = c_void_p
def interpolate(start, end, amount):
	return lib.ovg_interpolate(start, end, amount)

lib.ovg_bounds.argtypes = [c_void_p, POINTER(c_float), POINTER(c_float), POINTER(c_float), POINTER(c_float)]
lib.ovg_bounds.restype = None
def bounds(path):
	x, y, w, h = c_float(), c_float(), c_float(), c_float()

	lib.ovg_bounds(path,byref(x),byref(y),byref(w),byref(h))
	return (x.value,y.value,w.value,h.value)

lib.ovg_bounds_transformed.argtypes = [c_void_p, POINTER(c_float), POINTER(c_float), POINTER(c_float), POINTER(c_float)]
lib.ovg_bounds_transformed.restype = None
def bounds_transformed(path):
	x, y, w, h = c_float(), c_float(), c_float(), c_float()

	lib.ovg_bounds_transformed(path,byref(x),byref(y),byref(w),byref(h))
	return (x.value,y.value,w.value,h.value)



lib.ovg_length.argtypes = [c_void_p, c_int, c_int]
lib.ovg_length.restype = c_float
def length(path, start_segment, nsegments):
	f = lib.ovg_length(path,start_segment, nsegments)
	return f.value


lib.ovg_mask.argtypes = [c_void_p, c_int]
lib.ovg_mask.restype = None
def mask(path, msk):
	return lib.ovg_mask(path, msk)


lib.ovg_mask_off.argtypes = None
lib.ovg_mask_off.restype = None
def mask_off():
	return lib.ovg_mask_off()



lib.ovg_blend.argtypes = [c_int]
lib.ovg_blend.restype = None
def blend(bld):
	return lib.ovg_blend(bld)


"""
Transformations
"""

lib.ovg_translate.argtypes = [c_int, c_int]
lib.ovg_translate.restype = None
def translate(x, y):
	return lib.ovg_translate(x, y)

lib.ovg_rotate.argtypes = [c_int]
lib.ovg_rotate.restype = None
def rotate(r):
	return lib.ovg_rotate(r)

lib.ovg_scale.argtypes = [c_float, c_float]
lib.ovg_scale.restype = None
def scale(x, y):
	return lib.ovg_scale(x, y)


lib.ovg_shear.argtypes = [c_int, c_int]
lib.ovg_shear.restype = None
def shear(x, y):
	return lib.ovg_shear(x, y)


#matrices

lib.ovg_reset.argtypes = None
lib.ovg_reset.restype = None
def reset():
	return lib.ovg_reset()

lib.ovg_mat_mode.argtypes = [c_int]
lib.ovg_mat_mode.restype = None
def matrix_mode(m):
	return lib.ovg_mat_mode(m)

lib.ovg_mat_set.argtypes = [POINTER(c_float)]
lib.ovg_mat_set.restype = None
def matrix_set(m):
	#@todo: check length. current mode necessary to know

	mat = (c_float * len(m))(*m)
	return lib.ovg_mat_set(mat)


lib.ovg_mat_current.argtypes = [POINTER(c_float)]
lib.ovg_mat_current.restype = None
def matrix_get():
	m = (c_float * 9)

	lib.ovg_mat_current(m)
	return m


lib.ovg_mat_multiply.argtypes = [POINTER(c_float)]
lib.ovg_mat_multiply.restype = None
def matrix_mult(m):
	mat = (c_float * len(m))(*m)
	return lib.ovg_mat_multiply(mat)
