#include <stddef.h>
#include <stdio.h>
#include "common.h"

VGPath getpath(void){
	if (lastPath != NULL){
		vgDestroyPath(lastPath);
	}
	VGPath p = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
	lastPath = p;
	return p;
}
