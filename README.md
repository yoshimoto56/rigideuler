Euler method: Rigid Body Simulation
====

Overview

This porject is for learning how to Rigid Body Simulation by Euler method in C language. Let's fill in the code at TODO.

## Text
[See here](https://github.com/yoshimoto56/rigideuler/blob/master/text/EULER_text.pdf)

## Demo
[![Rigid Euler](https://img.youtube.com/vi/Ay6StTZ5N_I/0.jpg)](https://www.youtube.com/watch?v=Ay6StTZ5N_I "Rigid Euler")

## Requirement

- LIB: math, glut, glfw3
- Body Data PATH: ../data/param.ini
- Rigid Object Data PATH: ../data/armadillo.stl
- Container Data PATH: ../data/container.stl

## Compile command

    $ gcc main.c Solver.c Mesh.c GLTool.c MathTool.c -o euler -lglfw3 -lGL -lGLU -lXrandr -lXinerama -lXcursor -lXi -lXxf86vm -lX11 -lpthread -lm -ldl

    $ ./euler ../data/armadillo.stl

## Licence

[MIT](https://github.com/tcnksm/tool/blob/master/LICENCE)

## Author

[yoshimoto56](https://github.com/yoshimoto56)
