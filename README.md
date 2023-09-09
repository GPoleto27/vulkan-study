# Vulkan Studying repository

Nothing much here, just trying to learn Vulkan API based on [vulkan-tutorial](https://vulkan-tutorial.com). Strongly recommend reading [Learning Modern 3D Graphics Programming](https://paroj.github.io/gltut/) if you're not familiar with computer graphics.

## Dependencies
You can install the dependencies by running ```sudo make install```
- **vulkan-tools:** Command-line utilities, most importantly vulkaninfo and vkcube. Run these to confirm your machine supports Vulkan.
- **libvulkan-dev:** Installs Vulkan loader. The loader looks up the functions in the driver at runtime, similarly to GLEW for OpenGL - if you're familiar with that.
- **vulkan-validationlayers-dev spirv-tools:** Installs the standard validation layers and required SPIR-V tools. These are crucial when debugging Vulkan applications.
- **libglfw3-dev:** Vulkan does not include tools for creation a window to display the rendered results. The GLFW library to create a window, which supports Windows, Linux and MacOS.
- **libglm-dev:** Vulkan does not include a library for linear algebra operations GLM is a nice library that is designed for use with graphics APIs and is also commonly used with OpenGL.
- **libxxf86vm-dev libxi-dev** Required to build GLFW. The flag -lglfw is for GLFW, -lvulkan links with the Vulkan function loader and the remaining flags are low-level system libraries that GLFW needs. The remaining flags are dependencies of GLFW itself: the threading and window management.
It is possible that the Xxf68vm and Xi libraries are not yet installed on your system.

## Building and running
You can build the project by running ```make```, alternatively you can run ```make run``` to build and run the project.
