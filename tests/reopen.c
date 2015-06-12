#include <stdio.h>
#include <ovg.h>

int main(int argc, char **argv){

	ovg_init();

	ovg_open(0,0,500,500);
	ovg_draw();

	getchar();

	ovg_close();

	getchar();

	ovg_open(0,0,600,600);

	getchar();

	ovg_draw();

	getchar();

	ovg_close();
	ovg_cleanup();

	return 0;

}
