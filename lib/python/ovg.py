#!/usr/bin/env python3

from ctypes.util import find_library
from ctypes import *


lib = CDLL(find_library("ovg"))

lib.ovg_init()

"""
Python-wrapper-specific helpers
"""
class Color(object):
	_r=c_ubyte(255)
	_g=c_ubyte(255)
	_b=c_ubyte(255)
	_a=c_ubyte(255)

	# accepts r,g,b,a -- four ints 0-255
	def __init__(self,*args):
		super(Color,self).__init__()
		self.r, self.g, self.b, self.a = parse_args(args,['r','g','b','a'])

	@property
	def r(self):
		return self._r.value
	@r.setter
	def r(self, r):
		self._r = c_ubyte(r)
	@property
	def g(self):
		return self._g.value
	@g.setter
	def g(self, g):
		self._g = c_ubyte(g)
	@property
	def b(self):
		return self._b.value
	@b.setter
	def b(self, b):
		self._b = c_ubyte(b)
	@property
	def a(self):
		return self._a.value
	@a.setter
	def a(self, a):
		self._a = c_ubyte(a)
	


class Point(object):
	_x=c_int(0)
	_y=c_int(0)

	def __init__(self,*args):
		super(Point,self).__init__()
		self.x, self.y = parse_args(args, ['x','y'])

	@property
	def x(self):
		return self._x
	@x.setter
	def x(self, x):
		self._x = c_int(x)
	
	@property
	def y(self):
		return self._y
	@y.setter
	def y(self, y):
		self._y = c_int(y)
	


class Rect(object):
	_x=c_int(0)
	_y=c_int(0)
	_w=c_int(0)
	_h=c_int(0)

	
	#accepts points (for each corner) or x,y,w,h
	def __init__(self,*args):
		super(Rect,self).__init__()
		self.x, self.y, self.w, self.h = parse_args(args, ['x','y','w','h'])

	@property
	def x(self):
		return self._x
	@x.setter
	def x(self, x):
		self._x = c_int(x)
	@property
	def y(self):
		return self._y
	@y.setter
	def y(self, y):
		self._y = c_int(y)
	@property
	def w(self):
		return self._w
	@w.setter
	def w(self, w):
		self._w = c_int(w)
	@property
	def h(self):
		return self._h
	@h.setter
	def h(self, h):
		self._h = c_int(h)
	

"""
internal helper function for allowing multiple argument types
@param args list or tuple - the argument list to process
@param attrs list - strings of what attributes or dict keys to check.
					Length should match how many raw items match object
@return list of bare values
"""
def parse_args(args,attrs,count=-1):
	vals=[] #return list to put final values in
	largs = list(args)
	while largs and count:
		count -= 1
		arg = largs.pop(0)
		if all([hasattr(arg,prop) for prop in attrs]):
			#every attribute listed is an attribute of arg
			vals.extend([getattr(arg,prop) for prop in attrs])
		elif type(arg) == dict and all([key in arg for key in attrs]):
			#every attribute listed is a key in arg
			vals.extend([arg[key] for key in attrs])
		elif type(arg) in (tuple,list) and len(arg) == len(attrs):
			#list of same length as desired properties
			vals.extend(arg)
		else:
			#grab that many bare args
			vals.append(arg)
			for i in range(len(attrs)-1):
				vals.append(largs.pop(0))
	if count == 0:
		return vals,largs
	return vals



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


class Commands(object):
	Close_Path = ( 0 << 1)
	Move_To    = ( 1 << 1)
	Line_To    = ( 2 << 1)
	HLine_To   = ( 3 << 1)
	VLine_To   = ( 4 << 1)
	Quad_To    = ( 5 << 1)
	Cubic_To   = ( 6 << 1)
	SQuad_To   = ( 7 << 1)
	SCubic_To  = ( 8 << 1)
	SCCWArc_To = ( 9 << 1)
	SCWArc_To  = (10 << 1)
	LCCWArc_To = (11 << 1)
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
def open(*args):
	return lib.ovg_open( *parse_args(args,['x','y','w','h']) )


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
def clear_rect(*args):
	"""
	@param x,y,w,h - define bounding rectangle to clear
	#param c - Coord type (absolute or relative)
	"""
	(x,y,w,h),(coord,) = parse_args(args,['x','y','w','h'],count=1)
	return lib.ovg_clear_rect( x,y,w,h, coord )





"""
	------------------------------------
	Fills n Strokes
	------------------------------------
"""


lib.ovg_fill.argtypes = [c_ubyte, c_ubyte, c_ubyte, c_ubyte]
lib.ovg_fill.restype = None
def fill(*args):
	"""
	Set Fill Color, as values between 0-255
	"""
	return lib.ovg_fill( *parse_args(args,['r','g','b','a']) )




lib.ovg_fill_current.argtypes = [POINTER(c_ubyte), POINTER(c_ubyte), POINTER(c_ubyte), POINTER(c_ubyte)]
lib.ovg_fill_current.restype = None
def fill_current():
	r = c_ubyte()
	g = c_ubyte()
	b = c_ubyte()
	a = c_ubyte()
	lib.ovg_fill_current(byref(r),byref(g),byref(b),byref(a))
	return Color(r.value,g.value,b.value,a.value)


lib.ovg_stroke.argtypes = [c_ubyte, c_ubyte, c_ubyte, c_ubyte]
lib.ovg_stroke.restype = None
#expects Color
def stroke(*args):
	"""
	Set Stroke Color, as values between 0-255
	"""
	return lib.ovg_stroke( *parse_args(args,['r','g','b','a']) )


lib.ovg_stroke_current.argtypes = [POINTER(c_ubyte), POINTER(c_ubyte), POINTER(c_ubyte), POINTER(c_ubyte)]
lib.ovg_stroke_current.restype = None
def stroke_current():
	r = c_ubyte()
	g = c_ubyte()
	b = c_ubyte()
	a = c_ubyte()
	lib.ovg_stroke_current(byref(r),byref(g),byref(b),byref(a))
	return Color(r.value,g.value,b.value,a.value)


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
def linear_gradient(rule, *args):
	"""
	Create a linear gradient and set as active fill
	@param rule - GradientRule (Pad, Reflect, Repeat)
	@param startx, starty - ints
	@param endx, endy - ints
	@param points - list of floats for color stop locations (0 through 1)
	@param colors - list of colors (Color, or ints or ints in a list, dicts, etc)
	"""
	(startx, starty, endx, endy), (points,colors) = parse_args(args,['x','y'],count=2)
	colors = parse_args(colors,['r','g','b','a'])

	lp = len(points)
	lc = len(colors)
	if lp < 2:
		raise Exception("Gradients must have at least 2 color stops")
	if lc != lp*4:
		raise Exception("Invalid number of colors per point")

	#type conversion, and put into lists acceptable for C
	c = (c_ubyte * lc)(*colors)
	p = (c_float * lp)(*points)
	return lib.ovg_gradient_linear(lp, rule, startx, starty, endx, endy, p, c)


lib.ovg_gradient_radial.argtypes = [c_int, c_int, c_int, c_int, c_int, c_int, c_int, POINTER(c_float), POINTER(c_ubyte)]
lib.ovg_gradient_radial.restype = None
def radial_gradient(rule, *args):
	"""
	Create a radial gradient and set as active fill
	@param rule - GradientRule (Pad, Reflect, Repeat)
	@param cenx, ceny - ints, center
	@param focx, focy - ints, focal point
	@param rad - int, radius
	@param points - list of floats for color stop locations (0 through 1)
	@param colors - list of colors (Color, or ints or ints in a list, dicts, etc)
	"""
	(cenx,ceny,focx,focy),(rad,points,colors) = parse_args(args,['x','y'],count=2)
	colors = parse_args(colors,['r','g','b','a'])

	lp = len(points)
	lc = len(colors)
	if lp < 2:
		raise Exception("Gradients must have at least 2 color stops")
	if lc != lp*4:
		raise Exception("Invalid number of colors per point")

	#type conversion, and put into lists acceptable for C
	c = (c_ubyte * lc)(*colors)
	p = (c_float * lp)(*points)
	return lib.ovg_gradient_radial(lp, rule, cenx, ceny, focx, focy, rad, p, c)


lib.ovg_fill_rule.argtypes = [c_int]
lib.ovg_fill_rule.restype = None
def fill_rule(r):
	return lib.ovg_fill_rule(r)



"""
Shapes and Objects
"""

lib.ovg_line.argtypes = [c_int, c_int, c_int, c_int]
lib.ovg_line.restype = c_void_p
def line(*args):
	"""
		@param x0,y0 - ints, point at the start of the line
		@param x1,y1 - ints, point at the end of the line
	"""
	return lib.ovg_line( *parse_args(args,['x','y']) )


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
def rect(*args):
	"""
	@param x,y,w,h - ints, bounds of rectangle
	"""
	return lib.ovg_rect( *parse_args(args,['x','y','w','h']) )


lib.ovg_round_rect.argtypes = [c_int, c_int, c_int, c_int, c_int]
lib.ovg_round_rect.restype = c_void_p
def round_rect(*args):
	"""
	@param x,y,w,h - ints, bounds of rectangle
	@param r - int, radius of corner
	"""
	(x,y,w,h),(r,) = parse_args(args,['x','y','w','h'],count=1)
	return lib.ovg_round_rect(x,y,w,h,r)


lib.ovg_circle.argtypes = [c_int, c_int, c_int]
lib.ovg_circle.restype = c_void_p
def circle(*args):
	"""
	@param cx, cy - ints, center of circle
	@param r - int, radius
	"""
	(cx,cy),(r,) = parse_args(args,['x','y'],count=1)
	return lib.ovg_circle(cx,cy,r)


lib.ovg_ellipse.argtypes = [c_int, c_int, c_int, c_int]
lib.ovg_ellipse.restype = c_void_p
def ellipse(*args):
	"""
	@param cx,cy,w,h - bounding box of ellipse
	"""
	if len(args) == 1: #passing something like a rect
		return lib.ovg_ellipse( *parse_args(args,['x','y','w','h']) )
	else:
		(cx,cy),(w,h) = parse_args(args,['x','y'],count=1)
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
def text(*args):
	"""
	@param x,y - ints, placement of text anchor
	@param txt - string, text to display
	@param size - int, font pointsize
	"""
	(x,y),(txt,size) = parse_args(args,['x','y'],count=1)
	c_s = c_char_p(txt.encode("ascii"))
	return lib.ovg_text(x,y,c_s,size)


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
	return f


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
def translate(*args):
	return lib.ovg_translate( *parse_args(args,['x','y']) )

lib.ovg_rotate.argtypes = [c_int]
lib.ovg_rotate.restype = None
def rotate(*args):
	return lib.ovg_rotate( *parse_args(args,['r']) )

lib.ovg_scale.argtypes = [c_float, c_float]
lib.ovg_scale.restype = None
def scale(*args):
	return lib.ovg_scale( *parse_args(args,['x','y']) )


lib.ovg_shear.argtypes = [c_int, c_int]
lib.ovg_shear.restype = None
def shear(*args):
	return lib.ovg_shear( *parse_args(args,['x','y']) )


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
