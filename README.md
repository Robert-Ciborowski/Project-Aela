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
