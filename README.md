# Project-Aela
A 2D/3D engine written in C++ and GLSL using OpenGL and OpenAL. This engine is used by the video game Neo-Zero (https://www.neozerogame.com).

## Features
* 3D rendering of models, lights, skyboxes, materials, billboards and particles
* 3D maps & entity management systems
* 3D model loading
* 2D rendering of images and text,
* 2D and 3D animation system based on keyframes, including translation, scaling, rotation and point-rotation animations
* Texture and font loading
* Audio playback from .wav and .ogg files using audio streaming
* Loading and writing to compressed files
* Error handling system
* Events system, including pre-built keyboard, mouse and window events
* Menus, including buttons, labels (text), images, rectangles and submenus
* Python scripting
* Resource management system, including resource objects (e.g. a texture) and resource loaders (e.g. PNG loader)
* Scene system for switching between maps and menus
* Time classes, such as Clock, Stopwatch, Timer and FramerateCalculator
* UserEnvironment class for obtaining details such as GPU name or CPU core count
* Window class for creating and manipulating the user window
* XML loading
* Utility classes for colours, shapes, and C++ functions

## Required APIs
* Glew (2.1.0) and GLM (0.9.7.1) (for OpenGL)
* OpenAL
* SDL (2.0.10)
* Libvorbis (1.3.6)
* Libogg (1.3.3)
* Boost (1.70.0) (for Python scripting)
* Python (3.7)
* Freetype (2.9)
* Zlib (1.2.11)
* Libpng (1.6.35)

## Neo-Zero Screenshots
Here are some development screenshots of Neo-Zero, which is powered by Project Aela.

A town in Neo-Zero.
![https://neozerogame.com/img/screenshots/town.png](https://neozerogame.com/img/screenshots/town.png "A town.")

The same town during a cool evening.
![https://neozerogame.com/img/screenshots/demo_out_1.png](https://neozerogame.com/img/screenshots/demo_out_1.png "The same town during a cool evening.")

Neo-Zero's battles, which can see a lot of smaller images displayed at the same time.
![https://neozerogame.com/img/screenshots/demo_out_2.png](https://neozerogame.com/img/screenshots/demo_out_2.png "Neo-Zero's battles, which can see a lot of smaller images displayed at the same time.")

## Historical Screenshots
Here are some historical screenshots of Project Aela.

The first model that was displayed with Project Aela.
![https://neozerogame.com/img/screenshots/project-aela-first-model.png](https://neozerogame.com/img/screenshots/project-aela-first-model.png "The first model that was displayed with Project Aela.")

The first shadows that were rendered in Project Aela.
![https://neozerogame.com/img/screenshots/aela-dev-1.png](https://neozerogame.com/img/screenshots/aela-dev-1.png "The first shadows that were rendered in Project Aela.")

4x anti-aliasing when it was first introduced.
![https://neozerogame.com/img/screenshots/aela-dev-2.png](https://neozerogame.com/img/screenshots/aela-dev-2.png "4x anti-aliasing when it was first introduced.")

The first partially-transparent texture to be rendered in Project Aela, which is now used for glass.
![https://neozerogame.com/img/screenshots/aela-dev-3.png](https://neozerogame.com/img/screenshots/aela-dev-3.png "The first partially-transparent texture to be rendered in Project Aela, which is now used for glass.")

Testing how many entities can be rendered by Project Aela.
![https://neozerogame.com/img/screenshots/aela-dev-4.png](https://neozerogame.com/img/screenshots/aela-dev-4.png "Testing how many entities can be rendered by Project Aela.")
