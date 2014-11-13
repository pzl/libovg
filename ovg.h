#ifndef _LOVG_H
#define _LOVG_H

typedef enum {
  CAP_BUTT                                 = 0x1700,
  CAP_ROUND                                = 0x1701,
  CAP_SQUARE                               = 0x1702
} CapStyle;

typedef enum {
  JOIN_MITER                               = 0x1800,
  JOIN_ROUND                               = 0x1801,
  JOIN_BEVEL                               = 0x1802
} JoinStyle;

#define BG_COLOR { 0.94f, 1.0f, 0.92f, 1.0f }

/*****************************
* Management Commands
******************************/
void ovg_init(void);
void ovg_wininfo(int *x, int *y, int *w, int *h);
void ovg_cleanup(void);
void ovg_draw(void);
void ovg_clear(void);
void ovg_clear_rect(int x, int y, int w, int h);


/*****************************
* Fills n Strokes
*****************************/
void ovg_fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void ovg_stroke(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

void ovg_stroke_width(float w);
void ovg_stroke_cap(CapStyle);
void ovg_stroke_join(JoinStyle);
void ovg_stroke_miter(float limit);
void ovg_dash(int *pattern, int n);
void ovg_dash_phase(int phase);


/***************************
* Shapes and Objects
****************************/
void ovg_line(int x0,int y0,int x1,int y1);
void ovg_polyline(int *x, int *y, int n);
void ovg_polygon(int *x, int *y, int n);
void ovg_rect(int x, int y, int w, int h);
void ovg_round_rect(int x, int y, int w, int h, int r);
void ovg_circle(int cx, int cy, int r);
void ovg_ellipse(int cx, int cy, int w, int h);
void ovg_arc(int x, int y, int w, int h, int start_angle, int angle_travel); //pie slice and chord?
void ovg_text(int x, int y, char *s, int pointsize);



/************************
* Transformations
*************************/
void ovg_translate(int x, int y);
void ovg_rotate(int r);
void ovg_scale(float x, float y);
void ovg_shear(int x, int y);


#endif
