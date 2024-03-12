# INF443

Assignments from the course 3D computer graphics at École polytechnique. The projects in this repository were made in C++ using [OpenGL](https://learnopengl.com/Introduction). The repository also contains screenshots showcasing the scenes developed.

## Running locally

The ***inf443_lab_code*** folder contains all the exercises developed during lab sessions. Makefiles are provided to compile each exercise. In order to run them locally, navigate to an exercise's root folder and run

> make

> ./exercice_name

## Lab Exercises

### Lab 1: Basic 3D scene and shader effects

The objectives of this lab were to insert objects into a 3D scene and play with translations, rotations and shader effects controlled by the simulation's time parameter.

![intro](screenshots/Shader_effects.png)

### Lab 2: Phong Shading

In order to visualize depth on the projected image, shading is essencial. In this lab, we developed a [Phong shader](https://en.wikipedia.org/wiki/Phong_shading#:~:text=Phong%20shading%20interpolates%20surface%20normals,and%20the%20Phong%20reflection%20model.) from scratch.

![shader1](screenshots/Shader_Phong.png)

![shader2](screenshots/Shader_Phong2.png)

Additionally, a fog effect was added to make objects gradually fade away as distance increases.

![shader3](screenshots/Shader_Phong_Fog.png)

## Course Project

To come...