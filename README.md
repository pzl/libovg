LibOVG
======

LibOVG is a thin drawing library on top of OpenVG. It is designed to allow unified drawing commands between platforms (Raspberry Pi, BeagleBone, pcDuino, ...) and allowing for development on a desktop or laptop of choice. It can be run on these embedded devices without an X11 environment, greatly improving speed of your application. On your development machine, you can debug and preview your graphics with X running!


Requirements
-------------
**OpenVG**

On embedded devices like the Raspberry Pi, this should come pre-installed. For development environments (e.g. your laptop), support is provided through the [ShivaVG](https://github.com/ileben/ShivaVG) project, which implements OpenVG API on top of OpenGL.


Installation
------------

```sh
make
sudo make install
```


Usage
-----

Simple example usage is provided below. You can compile with `gcc -o ovg_demo temp.c -l ovg` and run with `./ovg_demo`. You should get a yellowish rectangle at the bottom of the screen that appears.

```c
#include <stdio.h>
#include <ovg.h>

int main(int argc, char **argv) {
    Path rect;

    ovg_init(); //initialize OpenGL contexts, etc

    ovg_open(0,0,400,400); //open a 400x400 window at 0,0 on your monitor

    ovg_fill(255,200,128,255);     //R,G,B,A
    rect = ovg_rect(76,50,200,90); //X,Y,W,H

    //perform the drawing commands
    ovg_draw_path(rect, FILL_PATH);
    ovg_draw();


    getchar(); //quick way to pause and wait for user input. press enter to close

    ovg_cleanup(); //properly destroy windows and release memory

    return 0;
}
```


Filename Conventions
--------------------

any filename ending in `_pi.c` or `_x11.c` will only get compiled on their target platforms. all other filenames get compiled for all targets.