#ifndef DRAWLIB_FONT_H
#define DRAWLIB_FONT_H

//from ajstarks/openvg
typedef struct {
	const short *CharacterMap;
	const int *GlyphAdvances;
	int Count;
	VGPath Glyphs[256];
} Fontinfo;

#define MAXFONTPATH 256

#endif
