### CSCE 450 - Final Project
##### Octavio Almanza
##### UIN: 628005492

#### Compilation Instructions

To compile the code, you will need the following libraries installed:
- ``CMAKE``, available [here](https://cmake.org/download/)
- ``GLM``, available [here](https://github.com/g-truc/glm).
- ``GLFW``, available [here](https://www.glfw.org).
- ``GLEW``, available [here](https://glew.sourceforge.net).
- ``EIGEN3``, available [here](https://eigen.tuxfamily.org/index.php?title=Main_Page).

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