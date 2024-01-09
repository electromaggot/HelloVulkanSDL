![snapshots of all demos](https://github.com/electromaggot/HelloVulkanSDL/blob/master/screenshots.png)
# HelloVulkanSDL
Starting-point for platform-independent graphics projects based on [Vulkan](https://www.khronos.org/vulkan/) and [SDL](https://www.libsdl.org/).

General Features:
- A "Hello Triangle"-style (3D "Hello World") main program, but including:
  - unified reworked examples from [Vulkan Tutorial](https://vulkan-tutorial.com/)
  - a bonus shader-based ray-marching example (thanks [ShaderToy](https://www.shadertoy.com/))!
- **Object-oriented** encapsulation of Vulkan's extensive setup
- [RAII](https://en.cppreference.com/w/cpp/language/raii) and injected dependencies (making their heirarchy obvious)
- Design goals: code that's [reusable, modular, minimalist](https://github.com/electromaggot/HelloVulkanSDL/wiki/Not%E2%80%90so%E2%80%90frequently-Asked-Questions#design-goals)

Specific Features:
- Ranking mechanism to automatically choose the best graphics device
- Abstracts some basic 3D objects and vertex types

Supports:
- iOS and macOS (Xcode 11 to 14) &nbsp; &rarr; &nbsp; [Mac Build Instructions](https://github.com/electromaggot/HelloVulkanSDL/tree/master/Xcode#mac-build-instructions)
- Windows (Visual Studio 2019+) &nbsp; &rarr; &nbsp; [Windows Build Instructions](https://github.com/electromaggot/HelloVulkanSDL/tree/master/VisualStudio#windows-build-instructions)
- Linux (CMake) &nbsp; &rarr; &nbsp; [Ubuntu Build Instructions](https://github.com/electromaggot/HelloVulkanSDL/tree/master/CMakeLinux#ubuntu--build-instructions) &nbsp; &nbsp; [Raspberry Pi 5 Build Instructions](https://github.com/electromaggot/HelloVulkanSDL/tree/master/CMakeRasPi#raspberry-pi-5--build-instructions)
- (Android and VR platform support coming soon!__[*](https://github.com/electromaggot/HelloVulkanSDL#contributing)__)

**Windows Requisites** and **Language Notes:** Visual Studio 2019 or later is required for this project.  [Why?](https://github.com/electromaggot/HelloVulkanSDL/wiki/Not%E2%80%90so%E2%80%90frequently-Asked-Questions#why-visual-studio-2019-only)\
Clang/LLVM is the targeted compiler, C++20/libc++ the intended language variant.

Why another "Hello Vulkan" project?  Hopefully this one is different.  [Some reasoning and justification here](https://github.com/electromaggot/HelloVulkanSDL/wiki/Not%E2%80%90so%E2%80%90frequently-Asked-Questions#why-another-hello-vulkan-or-hello-triangle-etc-when-there-are-many-others-on-github).

Find a note about my [choice of Unlicense here](https://github.com/electromaggot/HelloVulkanSDL/wiki/Not%E2%80%90so%E2%80%90frequently-Asked-Questions#license-related-personal-comment).

## General Build Dependencies

This project requires the following:
 1. Vulkan SDK
 2. Simple DirectMedia Layer (SDL)
 3. OpenGL Mathematics (glm) (*note that this is included in the Windows version of #1*)

More-specific instructions/links are in each platform-centric subdirectory. &nbsp; (again: [Windows here](https://github.com/electromaggot/HelloVulkanSDL/tree/master/VisualStudio#windows-build-instructions), [Mac here](https://github.com/electromaggot/HelloVulkanSDL/tree/master/Xcode#mac-build-instructions))

Whatever location you install these dependencies to, the Project will have to know where to find them, so it can include their header files and link with their library binaries.\
In the root directory of the HelloVulkanSDL Project, there will be a subdirectory called **`External`** where the Project's build process looks to find these dependencies.  So while you can modify the build settings, it's easiest to do one of these two options:\
&nbsp; &nbsp; A. Either install the packages to this **`External`** directory. &nbsp; ...or...\
&nbsp; &nbsp; B. Set up *symbolic links* from within this directory to the locations where you actually have them installed.\
&nbsp; &nbsp; &nbsp; &nbsp; To assist you, each platform-specific directory contains a `setup1st` script you can run to set up the links.\
&nbsp; &nbsp; &nbsp; &nbsp; However, it requires minor editing first.\
&nbsp; &nbsp; &nbsp; &nbsp; The script generally assumes you have already installed these packages, where you want them,\
&nbsp; &nbsp; &nbsp; &nbsp; so won't overzealously try to install them for you.

## Contributing

__&#42;__ &ndash; I could use your help!  This project isn't perfect, but I seek to perfect it.  If you see problems with how I'm doing something, please submit an issue and tell me.  If you'd like to contribute with a pull-request, please do!  If you don't like SDL, I tried to generalize it to an interface (`iPlatform`) and I added GLFW/STB/WTF files too, but I didn't test these.  Feel free to fix and PR.  The same goes for the aforementioned Android/VR additions.  If you help, of course I'll give you credit as a contributor here.

## Acknowledgements

This project owes much inspiration and appreciation to:
- [Alexander Overvoorde](https://github.com/Overv)'s excellent [Vulkan Tutorial](https://vulkan-tutorial.com/), which influenced much of my code and from which I blatantly stole sample shaders and assets, in order to rework and re-present them here.  His unrestrictive license is generous and I strive to do the same.
- [Sascha Willems](https://www.saschawillems.de) for his outstanding [Examples](https://github.com/SaschaWillems/Vulkan), contributions to the community, sharing knowledge and helping devs on online forums.
- LunarG for MoltenVK, for free, especially given Apple's limited support for it and their announced deprecation of OpenGL (a back-stab to the open 3D graphics standard to which Apple owes their early iPhone/AppStore success).
- Khronos and partner companies for Vulkan.
- [Inigo Quilez](https://www.iquilezles.org) for his awe-inspiring work (and generously sharing his secrets) too.
