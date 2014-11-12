#include "common.h"
#include <string.h>
#include "font.h"
#include "fonts/LiberationMono.inc"

/*
Copyright (C) 2012 Anthony Starks

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without 
restriction, including without limitation the rights to use, copy, modify, merge, publish, 
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// loadfont loads font path data
// derived from http://web.archive.org/web/20070808195131/http://developer.hybrid.fi/font2openvg/renderFont.cpp.txt
Fontinfo loadfont(const int *Points,
		  const int *PointIndices,
		  const unsigned char *Instructions,
		  const int *InstructionIndices, const int *InstructionCounts, const int *adv, const short *cmap, int ng) {

	Fontinfo f;
	int i;

	memset(f.Glyphs, 0, MAXFONTPATH * sizeof(VGPath));
	if (ng > MAXFONTPATH) {
		return f;
	}
	for (i = 0; i < ng; i++) {
		const int *p = &Points[PointIndices[i] * 2];
		const unsigned char *instructions = &Instructions[InstructionIndices[i]];
		int ic = InstructionCounts[i];
		VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_S_32,
					   1.0f / 65536.0f, 0.0f, 0, 0,
					   VG_PATH_CAPABILITY_ALL);
		f.Glyphs[i] = path;
		if (ic) {
			vgAppendPathData(path, ic, instructions, p);
		}
	}
	f.CharacterMap = cmap;
	f.GlyphAdvances = adv;
	f.Count = ng;
	return f;
}

// unloadfont frees font path data
void unloadfont(VGPath * glyphs, int n) {
	int i;
	for (i = 0; i < n; i++) {
		vgDestroyPath(glyphs[i]);
	}
}

// Text renders a string of text at a specified location, size, using the specified font glyphs
// derived from http://web.archive.org/web/20070808195131/http://developer.hybrid.fi/font2openvg/renderFont.cpp.txt
/*void Text(VGfloat x, VGfloat y, char *s, Fontinfo f, int pointsize) {*/
void ovg_text(int x, int y, char *s, int pointsize){
	VGfloat size = (VGfloat) pointsize,
			xx = (VGfloat) x,
			yy = (VGfloat) y,
			mm[9];
	int i;

	Fontinfo f = loadfont(LiberationMono_glyphPoints,
				LiberationMono_glyphPointIndices,
				LiberationMono_glyphInstructions,
				LiberationMono_glyphInstructionIndices,
				LiberationMono_glyphInstructionCounts,
				LiberationMono_glyphAdvances, LiberationMono_characterMap, LiberationMono_glyphCount);

	vgGetMatrix(mm);
	for (i = 0; i < (int)strlen(s); i++) {
		unsigned int character = (unsigned int)s[i];
		int glyph = f.CharacterMap[character];
		if (glyph == -1) {
			continue;			   //glyph is undefined
		}
		VGfloat mat[9] = {
			size, 0.0f, 0.0f,
			0.0f, size, 0.0f,
			xx, yy, 1.0f
		};
		vgLoadMatrix(mm);
		vgMultMatrix(mat);
		vgDrawPath(f.Glyphs[glyph], VG_FILL_PATH);
		xx += size * f.GlyphAdvances[glyph] / 65536.0f;
	}
	vgLoadMatrix(mm);

	unloadfont(f.Glyphs, f.Count);
}
