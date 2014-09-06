#ifndef DRAWLIB_H
#define DRAWLIB_H

#define BG_COLOR { 0.94f, 1.0f, 0.92f, 1.0f }

void dl_init(void);
void dl_wininfo(int *x, int *y, int *w, int *h);
void dl_cleanup(void);
void dl_rect(int x, int y, int w, int h);
void dl_text(int x, int y, char *s, int pointsize);
void dl_draw(void);
void dl_clear(void);
void dl_resize(void (*callback)(void));

#endif
