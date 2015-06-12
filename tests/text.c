#include <stdio.h>
#include <ovg.h>

int main(int argc, char **argv){

	ovg_init();

	ovg_open(0,0,500,750);
	ovg_clear_color(255,255,255,255);
	ovg_clear();

	OVGFont f = ovg_create_font();
	int l = ovg_text_width(f,"Hello World",12);

	printf("Hello World should be %d wide\n", l);


	ovg_text(20,20,f,"Hello World",12);
	ovg_free(ovg_draw_path(ovg_circle(20,20,3),FILL_PATH));
	ovg_free(ovg_draw_path(ovg_rect(20,20,l,12),STROKE_PATH));


	ovg_text( (80 - l/2) ,60,f,"Hello World",12);
	ovg_free(ovg_draw_path(ovg_circle(80,60,3),FILL_PATH));
	ovg_free(ovg_draw_path(ovg_rect((80 - l/2),60,l,12),STROKE_PATH));





	int *i, sizes[] = {6,7,8,9,10,11,12,14,16,18,21,24,36,48,60,72,96,0},
		height=0, width;

	for (i=sizes; *i; i++){
		height += *i + 5;
		width = ovg_text_width(f,"Lx",*i);

		ovg_text(200,height,f,"Lx", *i);
		ovg_free(ovg_draw_path(ovg_circle(200,height,3),FILL_PATH));
		ovg_free(ovg_draw_path(ovg_rect(200,height,width,*i),STROKE_PATH));
	}



	ovg_draw();

	ovg_destroy_font(f);

	getchar();

	ovg_cleanup();
	return 0;

}
