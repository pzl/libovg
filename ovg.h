#ifndef _LOVG_H
#define _LOVG_H

#define BG_COLOR { 0.94f, 1.0f, 0.92f, 1.0f }

void dl_init(void);
void dl_wininfo(int *x, int *y, int *w, int *h);
void dl_cleanup(void);
void dl_draw(void);
void dl_clear(void);


void dl_setFill(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void dl_setStroke(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

void dl_line(int x0,int y0,int x1,int y1);
void dl_rect(int x, int y, int w, int h);
void dl_round_rect(int x, int y, int w, int h, int r);
void dl_circle(int cx, int cy, int r);
void dl_ellipse(int cx, int cy, int w, int h);
//void dl_arc();
void dl_text(int x, int y, char *s, int pointsize);

#endif
