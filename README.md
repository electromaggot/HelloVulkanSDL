# HelloVulkanSDL

Starting-point for platform-independent graphics projects based on [Vulkan](https://www.khronos.org/vulkan/) and [SDL](https://www.libsdl.org/).

General Features:
- A "Hello Triangle"-style (3D "Hello World") main program, but including:
  - unified reworked examples from [Vulkan Tutorial](https://vulkan-tutorial.com/)
  - a bonus shader-based ray-marching example (thanks [ShaderToy](https://www.shadertoy.com/))!
- **Object-oriented** encapsulation of Vulkan's extensive setup
- [RAII](https://en.cppreference.com/w/cpp/language/raii) and injected dependencies (making their heirarchy obvious)
- Design goals: modular, minimalistic

Specific Features:
- Ranking mechanism to automatically choose the best graphics device
- Abstracts some basic 3D objects and vertex types

Supports:
- iOS and macOS (Xcode 11) &nbsp; &rarr; &nbsp; [Mac Build Instructions](https://github.com/electromaggot/HelloVulkanSDL/tree/master/Xcode#mac-build-instructions)
- Windows (Visual Studio 2019) &nbsp; &rarr; &nbsp; [Windows Build Instructions](https://github.com/electromaggot/HelloVulkanSDL/tree/master/VisualStudio#windows-build-instructions)

**Windows Requisites** and **Language Notes:** Visual Studio 2019 is required for this project, which is replete with modern C/C++ syntax, so won't build on older VS versions.\
One example is the use of "designated initializers," aligning with a main design goal here: "The less code, the better."\
That even means the most minimal code appearing on the visible page (although without crowding).\
Vulkan setup requires ubiquitous pre-initialized data structures, so streamlining their definition is important (designated initializers have existed for years in Clang/GCC for C++ and since C99).\
Clang/LLVM is the targetted compiler, C++14/libc++ the intended language variant.

## General Build Dependencies

This project requires the following:
 1. Vulkan SDK
 2. Simple DirectMedia Layer (SDL)
 3. OpenGL Mathematics (glm) (*note that this is included in the Windows version of #1*)

More-specific instructions/links are in each platform-centric subdirectory.

Whatever location you install these to, the Project will have to know where to find them, so it can include their header files and link with their library binaries.\
In the root directory of the HelloVulkanSDL Project, there will be a subdirectory called **`External`** where the Project's build process will look to find these dependencies.  So while you can modify the build settings, it's easiest to do one of these two options:\
&nbsp; &nbsp; A. Either install the packages to this `External` directory. &nbsp; ...or...\
&nbsp; &nbsp; B. Set up *symbolic links* from within this directory to the locations where you actually have them installed.\
&nbsp; &nbsp; &nbsp; &nbsp; To assist you, each platform-specific directory contains a `setup1st` script you can run to set up the links.\
&nbsp; &nbsp; &nbsp; &nbsp; However, it requires minor editing first.\
&nbsp; &nbsp; &nbsp; &nbsp; The script generally assumes you have already installed these packages, where you want them,\
&nbsp; &nbsp; &nbsp; &nbsp; so won't overzealously try to install them for you.

## Attributions

This project owes much inspiration and appreciation to:
- [Alexander Overvoorde](https://github.com/Overv)'s excellent [Vulkan Tutorial](https://vulkan-tutorial.com/), which influenced much of my code and from which I blatantly stole sample shaders and assets, in order to rework and re-present them here.  His unrestrictive license is generous and I strive to do the same.
- [Sascha Willems](https://www.saschawillems.de) for his outstanding [Examples](https://github.com/SaschaWillems/Vulkan), contributions to the community, sharing knowledge and helping devs on online forums.
- LunarG for MoltenVK, for free, especially given Apple's limited support for it and their announced deprecation of OpenGL (a back-stab to the open 3D graphics standard to which Apple owes their early iPhone/AppStore success).
- Khronos and partner companies for Vulkan.
- [Inigo Quilez](https://www.iquilezles.org) for his awe-inspiring work (and sharing his secrets) too.
