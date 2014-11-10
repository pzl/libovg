LibOVG
======

This is a wrapper over OpenVG library calls to make it portable between X11 and Raspberry Pi's graphics stack (meaning it can run on a Pi without X).


Requirements
-------------
- OpenVG
    + This should come pre-installed on the Raspberry Pi
    + On X11, the [ShivaVG](https://github.com/ileben/ShivaVG) project implements the OpenVG API through OpenGL

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

    ovg_init(); //initialize OpenGL contexts, etc

    ovg_setFill(255,200,128,255); //R,G,B,A 
    ovg_rect(202,50,200,90);      //X,Y,W,H
    ovg_draw(); //perform the drawing commands


    getchar(); //quick way to pause and wait for user input. press enter to close

    ovg_cleanup(); //properly destroy windows and release memory

    return 0;
}
```


Filename Conventions
--------------------

any filename ending in `_pi.c` or `_x11.c` will only get compiled on their target platforms. all other filenames get compiled for all targets.