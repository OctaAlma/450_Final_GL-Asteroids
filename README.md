# GL-Asteroids
### CSCE 450 - Final Project
##### Octavio Almanza
##### UIN: 628005492

## Description

This was my final project for CSCE 450: Computer Animation. The requirement for honors students was to implement some topic that was beyond what was covered in class. I decided to extend on my knowledge of positional and rotational keyframing by implementing "dynamic" keyframes, which vary depending on the position and directional momentum of the object.

![Demonstration of dynamic keyframes](./website/dynamic_keyframes.gif)

In this game, the player controls a spaceship and can perform barrel rolls and somersaults. The goal of the game is to destroy all of the asteroids and avoid colliding with them. For more information, please visit the [project's website](https://octaalma.github.io/GL-Asteroids/).

![Top-down camera gameplay showcase](./website/particle-effects.gif)

## Controls

- The ``WASD`` keys can be used to move the ship around the map.
- The ``Q`` and ``E`` keys trigger left and right barrel-rolls, respectively.
- The ``SPACEBAR`` key triggers a somersault.
- The ``J`` key can be used to fire a plasma beam. Use this to destroy the asteroids!
- The ``V`` key can be used to switch between third-person, top-down, and first-person cameras.
- The ``P`` key can be used to pause the game.
- The ``B`` key can be used to toggle bounding spheres.
- The ``F`` key can be used to increase the field of fiew.
- The ``G`` key can be used to decrease the field of view.

## Prerequisites and Compilation Instructions

To compile the code, you will need the following libraries installed:
- ``CMAKE``, available [here](https://cmake.org/download/)
- ``GLM``, available [here](https://github.com/g-truc/glm).
- ``GLFW``, available [here](https://www.glfw.org).
- ``GLEW``, available [here](https://glew.sourceforge.net).
- ``EIGEN3``, available [here](https://eigen.tuxfamily.org/index.php?title=Main_Page).

Please visit Dr. Sueda's compilation instructions [here](https://people.engr.tamu.edu/sueda/courses/CSCE441/2022S/labs/L00/index.html#WIN).


<!--

Once installed, you must define the following environment variables:
- ``GLM_INCLUDE_DIR`` to ``/path/to/GLM``
- ``GLFW_DIR`` to ``/path/to/GLFW``
- ``GLEW_DIR`` to ``/path/to/GLEW``
- ``EIGEN3_INCLUDE_DIR`` to ``/path/to/EIGEN3``

1. Once the prerequisite libraries are downloaded, create a new directory called ``build`` in the same directory containing the ``CMakeLists.txt``, ``src`` directory, and ``resources`` directory.

2. Open the ``build`` directory in your terminal and enter ``cmake ..``

3. After a successful compilation, enter the command ``make -j4``

4. Finally, run the program with the command ``./FINAL ../resources <ARGS>``

#### Arguments

- ``-a X``   - Sets the number of asteroids to ``X``
- ``-l X``   - Sets the number of lives to ``X``
- ``-b``     - Turns on bounding boxes around objects
- ``-f``     - Turns on the axis frame
- ``-g``     - Turns on the grid
- ``-t``     - Defaults to top-down cam
- ``-fp``    - Defaults to first-person cam
-->
