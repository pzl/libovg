#ifndef _LOVG_H
#define _LOVG_H

typedef enum {
	STROKE_PATH								= (1 << 0),
	FILL_PATH								= (1 << 1)
} PaintMode;

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
	MASK_CLEAR								= 0x1500,
	MASK_FILL								= 0x1501,
	MASK_SET								= 0x1502,
	MASK_UNION								= 0x1503,
	MASK_INTERSECT							= 0x1504,
	MASK_SUBTRACT							= 0x1505
} AlphaMask;

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

typedef enum {
	BLEND_SRC								= 0x2000,
	BLEND_SRC_OVER							= 0x2001,
	BLEND_DST_OVER							= 0x2002,
	BLEND_SRC_IN							= 0x2003,
	BLEND_DST_IN							= 0x2004,
	BLEND_MULTIPLY							= 0x2005,
	BLEND_SCREEN							= 0x2006,
	BLEND_DARKEN							= 0x2007,
	BLEND_LIGHTEN							= 0x2008,
	BLEND_ADDITIVE							= 0x2009
} BlendMode;


typedef enum {
	C_ABSOLUTE = 0,
	C_RELATIVE = 1
} CoordSystem;

typedef enum {
	CLOSE_PATH								= ( 0 << 1),
	MOVE_TO									= ( 1 << 1),
	LINE_TO									= ( 2 << 1),
	HLINE_TO								= ( 3 << 1),
	VLINE_TO								= ( 4 << 1),
	QUAD_TO									= ( 5 << 1),
	CUBIC_TO								= ( 6 << 1),
	SQUAD_TO								= ( 7 << 1),
	SCUBIC_TO								= ( 8 << 1),
	SCCWARC_TO								= ( 9 << 1),
	SCWARC_TO								= (10 << 1),
	LCCWARC_TO								= (11 << 1),
	LCWARC_TO								= (12 << 1)
} PathSegment;

typedef enum {
	MOVE_TO_ABS								= MOVE_TO | C_ABSOLUTE,
	MOVE_TO_REL								= MOVE_TO | C_RELATIVE,
	LINE_TO_ABS								= LINE_TO | C_ABSOLUTE,
	LINE_TO_REL								= LINE_TO | C_RELATIVE,
	HLINE_TO_ABS							= HLINE_TO | C_ABSOLUTE,
	HLINE_TO_REL							= HLINE_TO | C_RELATIVE,
	VLINE_TO_ABS							= VLINE_TO | C_ABSOLUTE,
	VLINE_TO_REL							= VLINE_TO | C_RELATIVE,
	QUAD_TO_ABS								= QUAD_TO | C_ABSOLUTE,
	QUAD_TO_REL								= QUAD_TO | C_RELATIVE,
	CUBIC_TO_ABS							= CUBIC_TO | C_ABSOLUTE,
	CUBIC_TO_REL							= CUBIC_TO | C_RELATIVE,
	SQUAD_TO_ABS							= SQUAD_TO | C_ABSOLUTE,
	SQUAD_TO_REL							= SQUAD_TO | C_RELATIVE,
	SCUBIC_TO_ABS							= SCUBIC_TO | C_ABSOLUTE,
	SCUBIC_TO_REL							= SCUBIC_TO | C_RELATIVE,
	SCCWARC_TO_ABS							= SCCWARC_TO | C_ABSOLUTE,
	SCCWARC_TO_REL							= SCCWARC_TO | C_RELATIVE,
	SCWARC_TO_ABS							= SCWARC_TO | C_ABSOLUTE,
	SCWARC_TO_REL							= SCWARC_TO | C_RELATIVE,
	LCCWARC_TO_ABS							= LCCWARC_TO | C_ABSOLUTE,
	LCCWARC_TO_REL							= LCCWARC_TO | C_RELATIVE,
	LCWARC_TO_ABS							= LCWARC_TO | C_ABSOLUTE,
	LCWARC_TO_REL							= LCWARC_TO | C_RELATIVE
} PathCommand;


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
Path ovg_draw_path(Path p, PaintMode);
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
Path ovg_path(int n_commands, unsigned char *commands, float *data);
void ovg_text(int x, int y, char *s, int pointsize);


/*********************
* Utility Commands
*********************/
Path ovg_interpolate(Path start, Path end, float amount);
void ovg_bounds(Path, float *x, float *y, float *w, float *h);
void ovg_bounds_transformed(Path, float *x, float *y, float *w, float *h);
float ovg_length(Path, int startseg, int nseg);
void ovg_mask(Path, AlphaMask);
void ovg_mask_off(void);
void ovg_blend(BlendMode);


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
