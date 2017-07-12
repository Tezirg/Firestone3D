# Firestone3D

F3D is an Open Source desktop graphics & compute library. 
It is designed so that it is easy to use for beginners, while allowing advanced users to fully configure and write their own pipelines.

# Features
	* Window management
	* Keyboard, mouse and joystick inputs
	* Scene graph interface
	* Import from common 3D geometry formats, [Full list](http://www.assimp.org/main_features_formats.html)
	* Supports common texture formats, [Full list](https://www.imagemagick.org/script/formats.php#supported)

# Using F3D
See the [Getting started](dead) for a basic introduction, or the [advanced guide](dead) for a complete introduction.
Some [Examples](https://github.com/Tezirg/Firestone3D/tree/master/examples) are also available to showcase the main features.
In addition, you might want to consult the [API reference](dead) (Generated with doxyen).

# Dependencies
## Exposed dependencies
For convenience, F3D is using and exposing a mathematics library for graphics programming.
	* [GLM](https://github.com/g-truc/glm), [License](http://glm.g-truc.net/copying.txt)

## Hidden dependencies
F3D is also internally using multiple libraries:
	* [ImageMagick](http://imagemagick.org/script/index.php), [License](http://imagemagick.org/script/license.php)
	* [Assimp](https://github.com/assimp/assimp), [License](http://assimp.sourceforge.net/main_license.html), 
	* [GLFW] (https://github.com/glfw/glfw), [License](http://www.glfw.org/license.html)

## Drivers
F3D is written is such a way that the underlying driver is completely hidden to the user.
Currently based on [Vulkan](https://www.khronos.org/vulkan), following versions may include support for others APIs (OpenGL, DX12)

The Vulkan SDK is therefore required to be installed on your system before using or compiling F3D.
 