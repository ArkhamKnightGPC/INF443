# INF443

Assignments from the course 3D computer graphics at École polytechnique. The projects in this repository were made in C++ using [OpenGL](https://learnopengl.com/Introduction). The repository also contains screenshots showcasing the scenes developed.

**Table of content:**
 - [Running locally](#running-locally)
 - [Lab Exercises](#lab-exercises)
    - [Lab 1: Basic 3D scene and shader effects](#lab1)
    - [Lab 2: Phong Shading](#lab2)
    - [Lab 3: 3D modelisation](#lab3)
 - [Course project](#course-project)

<a id="running-locally"></a>

## Running locally

The ***inf443_lab_code*** folder contains all the exercises developed during lab sessions. Makefiles are provided to compile each exercise. In order to run them locally, navigate to an exercise's root folder and run

> make

> ./exercice_name

<a id="lab-exercises"></a>

## Lab Exercises

<a id="lab1"></a>

### Lab 1: Basic 3D scene and shader effects

The objectives of this lab were to insert objects into a 3D scene and play with translations, rotations and shader effects controlled by the simulation's time parameter.

![intro](screenshots/Shader_effects.png)

<a id="lab2"></a>

### Lab 2: Phong Shading

In order to visualize depth on the projected image, shading is essencial. In this lab, we developed a [Phong shader](https://en.wikipedia.org/wiki/Phong_shading#:~:text=Phong%20shading%20interpolates%20surface%20normals,and%20the%20Phong%20reflection%20model.) from scratch.

![shader1](screenshots/Shader_Phong.png)

![shader2](screenshots/Shader_Phong2.png)

Additionally, a fog effect was added to make objects gradually fade away as distance increases.

![shader3](screenshots/Shader_Phong_Fog.png)

<a id="lab3"></a>

### Lab 3: 3D modelisation

In this lab, we explored modeling 3D surfaces using their parametric description. We coded meshes for a 3D terrain and a tree that was then procedurally added at random positions of the terrain in the final scene.

![modelisation1](screenshots/Modelisation_trees.png)

<a id="course-project"></a>

## Course Project

To come...