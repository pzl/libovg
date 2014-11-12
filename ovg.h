#ifndef _LOVG_H
#define _LOVG_H

#define BG_COLOR { 0.94f, 1.0f, 0.92f, 1.0f }

void ovg_init(void);
void ovg_wininfo(int *x, int *y, int *w, int *h);
void ovg_cleanup(void);
void ovg_draw(void);
void ovg_clear(void);

void ovg_fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void ovg_stroke(unsigned char r, unsigned char g, unsigned char b, unsigned char a);


void ovg_line(int x0,int y0,int x1,int y1);
void ovg_polyline(int *x, int *y, int n);
void ovg_polygon(int *x, int *y, int n);
void ovg_rect(int x, int y, int w, int h);
void ovg_round_rect(int x, int y, int w, int h, int r);
void ovg_circle(int cx, int cy, int r);
void ovg_ellipse(int cx, int cy, int w, int h);
void ovg_arc(int x, int y, int w, int h, int star_angle, int angle_travel);
void ovg_text(int x, int y, char *s, int pointsize);

#endif
