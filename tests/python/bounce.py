import ovg
import time
import sys


def axes():
	x,y,w,h = ovg.wininfo()
	#print("window is at %d,%d and is %dx%d" % (x,y,w,h))
	axis_padding = 5
	axis_major_tick = 25
	axis_minor_spacing = 5

	ovg.free(ovg.draw_path(ovg.line([axis_padding,axis_padding],[w - axis_padding, axis_padding]),ovg.PaintMode.Stroke))
	ovg.free(ovg.draw_path(ovg.line([axis_padding,h - axis_padding],[axis_padding, axis_padding]),ovg.PaintMode.Stroke))

	for i in range(0,w, int(axis_major_tick/axis_minor_spacing)):
		if i % axis_major_tick == 0:
			ovg.free(ovg.draw_path(ovg.line([i,axis_padding],[i,1]),ovg.PaintMode.Stroke))
		else:
			ovg.free(ovg.draw_path(ovg.line([i,axis_padding],[i,4]),ovg.PaintMode.Stroke))


	for i in range(0, h, int(axis_major_tick/axis_minor_spacing)):
		if i % axis_major_tick == 0:
			ovg.free(ovg.draw_path(ovg.line([axis_padding,i],[1,i]),ovg.PaintMode.Stroke))
		else:
			ovg.free(ovg.draw_path(ovg.line([axis_padding,i],[4,i]),ovg.PaintMode.Stroke))




if __name__ == '__main__':
	w,h = ovg.dispinfo()
	print("Display size is %dx%d" % (w, h))

	ovg.open(30,30,400,400)
	ovg.clear_color(200,200,200,255) # grey bg
	ovg.fill(255,0,120,255)
	
	ovg.clear()
	axes()
	ovg.draw()

	x,y = 10,10
	direction=1
	start = time.time()
	while True:
		ovg.clear()
		axes()
		ovg.free(ovg.draw_path(ovg.rect(x,y,50,50),ovg.PaintMode.Fill))
		x+=3*direction
		y+=3*direction

		if x > 400 - 50:
			direction=-1
		elif x < 5:
			direction=1

		ovg.draw()
		end=time.time()
		print("%.1f fps" % ( 1/(end-start) ,),end="\r")
		start=end

	#sys.stdin.read(1)
	#ovg.cleanup()
