#!/usr/bin/env python3

import ovg
import sys



AXIS_PADDING=7
AXIS_MAJOR_TICK=100
AXIS_MINOR_SPACING=2


TEST_CASE_SIZE=100
TEST_CASE_PADDING=20
CLOSE_ENOUGH=0.005


"""
helper functions
"""
class current(object):
	x = TEST_CASE_PADDING
	y = TEST_CASE_PADDING
	fill = ovg.Color(0,0,0,0)
	stroke = ovg.Color(0,0,0,0)


def stroke(x):
	ovg.free(ovg.draw_path(x,ovg.PaintMode.Stroke))

def fill(x):
	ovg.free(ovg.draw_path(x,ovg.PaintMode.Fill))

def strokefill(x):
	ovg.free(ovg.draw_path(x,ovg.PaintMode.Fill|ovg.PaintMode.Stroke))

def near(a,b):
	return (abs(a-b) < CLOSE_ENOUGH)


def save_fills():
	current.fill = ovg.fill_current()
	current.stroke = ovg.stroke_current()

def restore_fills():
	ovg.fill(current.fill)
	ovg.stroke(current.stroke)

def reset_styles():
	ovg.fill(0,0,0,0)
	ovg.stroke(0,0,0,255)
	ovg.stroke_width(1)
	ovg.stroke_cap(ovg.Cap.Butt)
	ovg.stroke_join(ovg.Join.Miter)
	ovg.dash_phase(0)
	ovg.dash(None)

def setUp():
	save_fills()
	reset_styles()
	ovg.translate(current.x, current.y)
	stroke(ovg.rect(0,0,TEST_CASE_SIZE,TEST_CASE_SIZE))
	restore_fills()

def tearDown():
	ovg.reset()
	current.x += TEST_CASE_SIZE + TEST_CASE_PADDING

	x,y,w,h = ovg.wininfo()

	if (current.x + TEST_CASE_SIZE > w):
		current.x = TEST_CASE_PADDING
		current.y += TEST_CASE_SIZE + TEST_CASE_PADDING




"""
test cases
"""


def path_len():
	rect = [
		[-1.0,-1.0,-1.0,0.0,100.0,200.0,300.0,400.0,-1.0],
		[-1.0,-1.0,-1.0,100.0,200.0,300.0,400.0,-1.0,-1.0],
		[-1.0,-1.0,-1.0,100.0,200.0,300.0,-1.0,-1.0,-1.0],
		[-1.0,-1.0,-1.0,100.0,200.0,-1.0,-1.0,-1.0,-1.0],
		[-1.0,-1.0,-1.0,100.0,-1.0,-1.0,-1.0,-1.0,-1.0],
		[-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0],
	]
	line = [
		[-1.0,-1.0,-1.0,0.0,70.710678,-1.0],
		[-1.0,-1.0,-1.0,70.710678,-1.0,-1.0],
		[-1.0,-1.0,-1.0,-1.0,-1.0,-1.0]
	]
	bez = [
		[-1.0,-1.0,-1.0,0.0,22.956015,-1.0],
		[-1.0,-1.0,-1.0,22.956015,-1.0,-1.0]
	]


	p = ovg.rect(10,13,100,100)
	print("rect 10,13,100,100")
	for segment in range(0,6):
		for i in range(-2,7):
			val = ovg.length(p, segment, i)
			if val != rect[segment][i+2]:
				print("Fail: Rect segment %d, i=%d. Should be %f but got %f" % (segment, i, rect[segment][i+2],val))
	ovg.free(p)

	p = ovg.line([0,0],[50,50])
	print("\nline 0,0,50,50")
	for segment in range(0,3):
		for i in range(-2,4):
			val = ovg.length(p, segment, i)
			if not near(val,line[segment][i+2]): #despite using near, these should be exact matches
				print("Fail: Line segment %d, i=%d. Should be %f but got %f" % (segment,i,line[segment][i+2],val))
	ovg.free(p)

	p = ovg.bezier_quad(0,0,10,10,20,0)
	print("\nbezier quad 0,0,10,10,20,0")
	for segment in range(0,2):
		for i in range(-2,4):
			val = ovg.length(p, segment, i)
			if not near(val,bez[segment][i+2]):
				print("Fail: Bezier segment %d, i=%d. Should be %f but got %f" % (segment,i,bez[segment][i+2],val))
	ovg.free(p)

	p = ovg.bezier_cube(0,5,[5,10],[10,0],15,5)
	print("\nbezier cubic 0,5,5,10,10,0,15,5")
	val = ovg.length(p,1,1)
	print("got length: %f" % val)
	ovg.free(p)

	p = ovg.arc(10,0,20,20,0,180)
	print("\narc 10,0,20,20,0,180")
	val = ovg.length(p,0,2)
	print("got length: %f" % val)

	ovg.free(p)


def basic_shapes():
	reset_styles()
	ovg.fill(255,200,128,255)

	setUp()
	fill(ovg.rect(5,5,30,30))
	strokefill(ovg.rect(50,5,47,30))
	fill(ovg.rect(5,40,90,50))
	tearDown()

	setUp()
	strokefill(ovg.round_rect(5,5,50,60,15))
	tearDown()

	setUp()
	strokefill(ovg.circle(50,50,20))
	tearDown()

	setUp()
	strokefill(ovg.ellipse(50,50,80,27))
	tearDown()

	setUp()
	stroke(ovg.line([10,90],[85,10]))
	tearDown()

	setUp()
	strokefill(ovg.arc(50,10,90,90,0,180))
	tearDown()



def fills():
	"""[5,0],[25,35],[45,0],[0,25],[50,25],[5,0]"""
	star = [ 5,0 , 25,35 , 45,0 , 0,25 , 50,25 , 5,0 ]

	ovg.fill(122,200,174,255)

	#test fill rules
	setUp()
	ovg.fill_rule(ovg.FillRule.All)
	strokefill(ovg.polygon(star))

	ovg.translate(0,50)
	ovg.fill_rule(ovg.FillRule.Alternate)
	strokefill(ovg.polygon(star))
	tearDown()

	#test alpha
	setUp()
	ovg.fill(255,0,0,120)
	fill(ovg.rect(2,10,60,60))

	ovg.fill(0,0,255,100)
	fill(ovg.rect(30,40,60,50))
	tearDown()

	ovg.draw()


def polys():
	#crappy star
	star = [
		[50,100],[30,65],[0,65],[25,40],[20,0],[50,25],[80,0],[75,40],[100,65],[70,65]
	]

	ovg.fill(255,200,128,255)
	setUp()
	strokefill(ovg.polygon(star))
	tearDown()

	setUp()
	stroke(ovg.polyline(star))
	tearDown()

def trans():
	#transformation tests
	#should be a small diamond above the other shapes

	ovg.fill(50,128,255,255)
	setUp()
	ovg.translate(40,40)
	ovg.rotate(45)
	ovg.scale(0.5,0.5)
	diamond = ovg.draw_path(ovg.rect(-50,-50,100,100), ovg.PaintMode.Fill | ovg.PaintMode.Stroke)

	#bounds checking of transformed box
	x,y,w,h = ovg.bounds(diamond)
	ovg.free(diamond)
	tearDown()

	#draw bounds
	ovg.stroke(255,0,0,255)
	stroke(ovg.rect(int(x),int(y),int(w),int(h)))
	ovg.draw()

	ovg.stroke(0,0,0,255) #undo red

	#test path interpolation
	dec = [(50, 100), (21, 89), (2, 65), (2, 34), (21, 10), (50, 0), (79, 10), (98, 34), (98, 65), (79, 89)]
	star = [(50, 100), (30, 65), (0, 65), (25, 40), (20, 0), (50, 25), (80, 0), (75, 40), (100, 65), (70, 65)]
	star_p = ovg.polygon(star)
	dec_p = ovg.polygon(dec)

	for i in range(0,6):
		setUp()
		strokefill(ovg.interpolate(star_p,dec_p,i*0.2))
		tearDown()

	ovg.free(star_p)
	ovg.free(dec_p)


def text():
	ovg.fill(0,0,0,255)
	setUp()
	f = ovg.create_font()
	w = ovg.text_width(f,"Shapes",16)
	ovg.text(4,70,f,"Demo",24)
	ovg.text(4,40,f,"Shapes",16)

	ovg.fill(0,0,255,255)
	ovg.stroke(255,0,0,255)

	stroke(ovg.rect(4,40,w,16))
	fill(ovg.circle(4,40,2))

	ovg.destroy_font(f)
	tearDown()
	ovg.stroke(0,0,0,255)

	ovg.draw()


def line_styles():

	#line width
	setUp()
	ovg.stroke_width(1)
	stroke(ovg.line([2,10],[98,10]))
	ovg.stroke_width(2)
	stroke(ovg.line([2,18],[98,18]))
	ovg.stroke_width(3)
	stroke(ovg.line([2,28],[98,28]))
	ovg.stroke_width(4)
	stroke(ovg.line([2,38],[98,38]))
	ovg.stroke_width(5)
	stroke(ovg.line([2,50],[98,50]))
	ovg.stroke_width(6)
	stroke(ovg.line([2,62],[98,62]))
	ovg.stroke_width(7)
	stroke(ovg.line([2,76],[98,76]))
	ovg.stroke_width(8)
	stroke(ovg.line([2,90],[98,90]))
	tearDown()

	#terminations
	setUp()
	ovg.stroke_width(10)
	ovg.stroke_cap(ovg.Cap.Round)
	stroke(ovg.line([10,20],[90,20]))
	ovg.stroke_cap(ovg.Cap.Butt)
	stroke(ovg.line([10,50],[90,50]))
	ovg.stroke_cap(ovg.Cap.Square)
	stroke(ovg.line([10,80],[90,80]))
	tearDown()

	#joins
	p = [ ovg.Point(40,10), {'x':50,'y':30}, 60,10  ]
	setUp()
	ovg.stroke_width(10)
	ovg.stroke_join(ovg.Join.Miter)
	stroke(ovg.polyline(p))
	p[0].y    += 30
	p[1]['y'] += 30
	p[3]      += 30
	ovg.stroke_join(ovg.Join.Round)
	stroke(ovg.polyline(p))
	p[0].y    += 30
	p[1]['y'] += 30
	p[3]      += 30
	ovg.stroke_join(ovg.Join.Bevel)
	stroke(ovg.polyline(p))
	tearDown()

	#bezier tests, arch and squiggle
	setUp()
	stroke(ovg.bezier_quad(10,10,45,60,80,10))
	stroke(ovg.bezier_cube(10,60,[25,35],[100,20],50,60))
	tearDown()

	#dashed lines
	dash = [5,10,15,20]
	setUp()
	ovg.stroke_width(3)
	#normal dash, repeated twice
	ovg.dash(dash)
	stroke(ovg.line([10,10],[90,10]))
	#above that, offset the position
	ovg.dash_phase(10)
	stroke(ovg.line([10,25],[90,25]))

	#reset phase
	ovg.dash_phase(0)
	stroke(ovg.line([10,40],[90,40]))
	tearDown()

	#turn off dashing
	ovg.dash(None)
	ovg.draw()


def gradients():
	grad_pts = [0,1]
	colors = [
		255,100,100,255,
		20, 80, 255,255
	]

	#linear
	setUp()
	ovg.linear_gradient(ovg.GradientRule.Pad,0,0,200,200,grad_pts,colors)
	fill(ovg.rect(0,0,100,100))
	tearDown()
	#linear, tighter
	setUp()
	ovg.linear_gradient(ovg.GradientRule.Pad,50,50,100,100,grad_pts,colors)
	fill(ovg.rect(0,0,100,100))
	tearDown()
	#normal radial
	setUp()
	ovg.radial_gradient(ovg.GradientRule.Pad,50,50,50,50,50,grad_pts,colors)
	fill(ovg.rect(0,0,100,100))
	tearDown()
	#off-center focal point
	setUp()
	ovg.radial_gradient(ovg.GradientRule.Pad,50,50,50,7,50,grad_pts,colors)
	fill(ovg.rect(0,0,100,100))
	tearDown()


	#test gradient repeat types
	complex_grad_pts = [
		0,0.33,0.66,1
	]
	complex_colors = [
		255,0,0,255,
		0,255,0,255,
		0,0,255,255,
		255,255,255,255
	]
	#linear, pad
	setUp()
	ovg.linear_gradient(ovg.GradientRule.Pad,20,20,60,60,complex_grad_pts,complex_colors)
	fill(ovg.rect(0,0,100,100))
	tearDown()
	#linear, repeat
	setUp()
	ovg.linear_gradient(ovg.GradientRule.Repeat,20,20,60,60,complex_grad_pts,complex_colors)
	fill(ovg.rect(0,0,100,100))
	tearDown()
	#linear, reflect
	setUp()
	ovg.linear_gradient(ovg.GradientRule.Reflect,20,20,60,60,complex_grad_pts,complex_colors)
	fill(ovg.rect(0,0,100,100))
	tearDown()
	#radial, pad
	setUp()
	ovg.radial_gradient(ovg.GradientRule.Pad,50,50,50,50,50,complex_grad_pts,complex_colors)
	fill(ovg.rect(0,0,100,100))
	tearDown()
	#radial, repeat
	setUp()
	ovg.radial_gradient(ovg.GradientRule.Repeat,50,50,50,50,50,complex_grad_pts,complex_colors)
	fill(ovg.rect(0,0,100,100))
	tearDown()
	#radial, reflect
	setUp()
	ovg.radial_gradient(ovg.GradientRule.Reflect,50,50,50,50,50,complex_grad_pts,complex_colors)
	fill(ovg.rect(0,0,100,100))
	tearDown()

	ovg.draw()

def advanced_modes():
	ovg.fill(255,0,0,255)

	setUp()
	p = ovg.rect(20,40,100,20)
	ovg.mask(p,ovg.Alpha.Set)
	#ovg.free(p)
	fill(ovg.rect(2,2,96,96))
	tearDown()

	ovg.mask_off()

	#test blending
	#blends = [b for b in dir(ovg.Blend) if isinstance(getattr(ovg.Blend,b),int)]
	blends = ['Src','Dst_Over','Src_In','Dst_In','Multiply','Screen','Darken','Lighten','Add']
	for i in blends:
		setUp()
		ovg.fill(255,0,0,120)
		fill(ovg.rect(2,10,60,60))
		ovg.blend(getattr(ovg.Blend,i))
		ovg.fill(0,0,255,100)
		fill(ovg.rect(30,40,60,50))
		ovg.blend(ovg.Blend.Src_Over)
		tearDown()

	ovg.draw()


def custom_path():
	commands = [
		ovg.abs(ovg.Commands.Move_To),
		ovg.rel(ovg.Commands.VLine_To),
		ovg.rel(ovg.Commands.Line_To),
		ovg.rel(ovg.Commands.Line_To),
		ovg.rel(ovg.Commands.VLine_To),
		ovg.rel(ovg.Commands.Line_To),
		ovg.Commands.Close_Path
	]
	points = [
		14,32,
		36,
		36, -15,
		36, 15,
		-36,
		-36, 15
	]

	setUp()
	strokefill(ovg.path(commands,points))
	tearDown()

def axes():
	x,y,w,h = ovg.wininfo()
	print("window is at %d,%d and is %dx%d" % (x,y,w,h))

	stroke(ovg.line([AXIS_PADDING,AXIS_PADDING],[w - AXIS_PADDING, AXIS_PADDING]))
	stroke(ovg.line([AXIS_PADDING,h - AXIS_PADDING],[AXIS_PADDING, AXIS_PADDING]))

	for i in range(0,w, int(AXIS_MAJOR_TICK/AXIS_MINOR_SPACING)):
		if i % AXIS_MAJOR_TICK == 0:
			stroke(ovg.line([i,AXIS_PADDING],[i,1]))
		else:
			stroke(ovg.line([i,AXIS_PADDING],[i,4]))


	for i in range(0, h, int(AXIS_MAJOR_TICK/AXIS_MINOR_SPACING)):
		if i % AXIS_MAJOR_TICK == 0:
			stroke(ovg.line([AXIS_PADDING,i],[1,i]))
		else:
			stroke(ovg.line([AXIS_PADDING,i],[4,i]))

	ovg.draw()

def clear():
	ovg.fill(220,80,80,255)

	setUp()
	strokefill(ovg.rect(10,10,80,80))
	ovg.clear_rect(60,20,20,20,ovg.Coord.REL)
	tearDown()

	ovg.draw()

	sys.stdin.read(1)

	ovg.clear_color(255,0,0,255)
	ovg.clear()
	ovg.draw()



if __name__ == '__main__':
	w,h = ovg.dispinfo()
	print("Display size is %dx%d" % (w, h))


	ovg.open(0,0,1366,768)
	ovg.clear_color(200,200,200,255) # grey bg
	ovg.clear()

	path_len()
	basic_shapes()
	fills()
	polys()
	trans()
	text()
	line_styles()
	gradients()
	advanced_modes()
	custom_path()

	axes()
	clear()

	sys.stdin.read(1)
	ovg.cleanup()
