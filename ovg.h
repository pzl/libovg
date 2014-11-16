#ifndef _LOVG_H
#define _LOVG_H

typedef enum {
	QUALITY_NON_ANTIALIASED					= 0x1200,
	QUALITY_FAST							= 0x1201,
	QUALITY_BEST							= 0x1202
} DrawQuality;

typedef enum {
	MATRIX_PATH								= 0x1400,
	MATRIX_IMAGE							= 0x1401,
	MATRIX_FILL								= 0x1402,
	MATRIX_STROKE							= 0x1403
} MatrixMode;

typedef enum {
	CAP_BUTT								= 0x1700,
	CAP_ROUND								= 0x1701,
	CAP_SQUARE								= 0x1702
} CapStyle;

typedef enum {
	JOIN_MITER								= 0x1800,
	JOIN_ROUND								= 0x1801,
	JOIN_BEVEL								= 0x1802
} JoinStyle;

typedef enum {
	FILL_RULE_ALTERNATE						= 0x1900,
	FILL_RULE_ALL							= 0x1901
} FillRule;

typedef enum {
	GRADIENT_PAD							= 0x1C00,
	GRADIENT_REPEAT							= 0x1C01,
	GRADIENT_REFLECT						= 0x1C02
} GradRule;

#ifndef Path
typedef void * Path;
#endif

#define BG_COLOR { 0.94f, 1.0f, 0.92f, 1.0f }

/*****************************
* Management Commands
******************************/
void ovg_init(void);
void ovg_open(int x, int y, int w, int h);
void ovg_wininfo(int *x, int *y, int *w, int *h);
void ovg_dispinfo(unsigned int *w, unsigned int *h);
void ovg_quality(DrawQuality);
void ovg_free(Path);
void ovg_cleanup(void);
void ovg_draw(void);
void ovg_clear(void);
void ovg_clear_rect(int x, int y, int w, int h);


/*****************************
* Fills n Strokes
*****************************/
void ovg_fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void ovg_fill_current(unsigned char *r,
                      unsigned char *g,
                      unsigned char *b,
                      unsigned char *a);
void ovg_stroke(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void ovg_stroke_current(unsigned char *r,
                      unsigned char *g,
                      unsigned char *b,
                      unsigned char *a);

void ovg_stroke_width(float w);
void ovg_stroke_cap(CapStyle);
void ovg_stroke_join(JoinStyle);
void ovg_stroke_miter(float limit);
void ovg_dash(int *pattern, int n);
void ovg_dash_phase(int phase);

//float locations should be 0..1, with 4 uchars per point
void ovg_gradient_linear(int nstops, GradRule,
                         int startx, int starty,
                         int endx, int endy,
                         float *points, unsigned char *colors);
void ovg_gradient_radial(int nstops, GradRule,
                         int centerx, int centery,
                         int focalx, int focaly, int radius,
                         float *points, unsigned char *colors);
void ovg_fill_rule(FillRule);


/***************************
* Shapes and Objects
****************************/
Path ovg_line(int x0,int y0,int x1,int y1);
Path ovg_polyline(int *x, int *y, int n);
Path ovg_polygon(int *x, int *y, int n);
Path ovg_rect(int x, int y, int w, int h);
Path ovg_round_rect(int x, int y, int w, int h, int r);
Path ovg_circle(int cx, int cy, int r);
Path ovg_ellipse(int cx, int cy, int w, int h);
Path ovg_arc(int x, int y, int w, int h, int start_angle, int angle_travel); //pie slice and chord?
Path ovg_bezier_quad(int sx, int sy, int cx, int cy, int ex, int ey); //start, control, end
Path ovg_bezier_cube(int sx, int sy,
                     int c1x, int c1y, int c2x, int c2y,
                     int ex, int ey); //start, control1,2, end
void ovg_text(int x, int y, char *s, int pointsize);


/*********************
* Utility Commands
*********************/
Path ovg_interpolate(Path start, Path end, float amount);
void ovg_bounds(Path, float *x, float *y, float *w, float *h);
void ovg_bounds_transformed(Path, float *x, float *y, float *w, float *h);


/************************
* Transformations
*************************/
void ovg_translate(int x, int y);
void ovg_rotate(int r);
void ovg_scale(float x, float y);
void ovg_shear(int x, int y);
//matrices
void ovg_reset(void);
void ovg_mat_mode(MatrixMode);
void ovg_mat_set(float *matrix);
void ovg_mat_current(float *matrix);
void ovg_mat_multiply(float *matrix); //shortcut for _set(current * new)

#endif
