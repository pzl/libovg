LibOVG
======

This is a wrapper over OpenVG library calls to make it portable between X11 and Raspberry Pi's graphics stack (meaning it can run on a Pi without X).


Filename Conventions
--------------------

any filename ending in `_pi.c` or `_x11.c` will only get compiled on their target platforms. all other filenames get compiled for all targets.