# HelloVulkanSDL

Starting-point for platform-independent graphics projects based on [Vulkan](https://www.khronos.org/vulkan/) and [SDL](https://www.libsdl.org/).

A "Hello Triangle"-style (3D "Hello World") program, hides away much of Vulkan's extensive setup in an **object-oriented** manner.  Includes unified, reworked examples from [Vulkan Tutorial](https://vulkan-tutorial.com/) plus a bonus shader-based ray-marching example (thanks [ShaderToy](https://www.shadertoy.com/))!

Currently supports:
- iOS and macOS (Xcode 11)
- Windows (Visual Studio 2019)

**Windows Requisites** and **Language Notes:** Visual Studio 2019 is required for this project, which is replete with modern C/C++ syntax, so won't build on older VS versions.\
One example is the use of "designated initializers," aligning with a main design goal here: "The less code, the better."\
That even means the most minimal code appearing on the visible page (although without crowding).\
Vulkan setup requires ubiquitous pre-initialized data structures, so streamlining their definition is important (designated initializers have existed for years in Clang/GCC for C++ and since C99).\
Clang/LLVM is the targetted compiler, C++14/libc++ the intended language variant.

## Build Instructions

You'll need to download the following:
 1. Vulkan SDK
 2. Simple DirectMedia Layer (SDL)

More-specific instructions/links are in the next section.

Whatever location you install these to, the Project will have to know where to find them, so it can include their header files and link with their library binaries.\
In the root directory of the Project, there will be a subdirectory called `External` where the Project's build process will look to find these dependencies.  So while you can modify the build settings, it's easiest to do one of these two options:\
&nbsp;&nbsp;&nbsp;&nbsp;A. Either install the packages to this `External` directory.\
&nbsp;&nbsp;&nbsp;&nbsp;B. Or set up *symbolic links* from within this directory to the locations where you actually have them installed. To assist you,\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;each platform-specific directory contains a script you can run to set up the links.  However, it requires a minor edit first.

**STEP-BY-STEP INSTRUCTIONS**

0. After cloning this project (or downloading/unzipping it, then opening a Terminal (Mac) or Command Prompt (Windows)),\
   `cd` to the project's directory and assume that is `./` as referred to below (of course reversing the slash for Windows).

1. Download the latest **Vulkan SDK**: \
   **Windows**: &nbsp; https://vulkan.lunarg.com/sdk/home#windows \
   &nbsp;&nbsp;&nbsp;&nbsp;Run the `.exe` and when requested, either:\
   &nbsp;&nbsp;&nbsp;&nbsp;a) Change the Install directory to: &nbsp; `./External/VulkanSDK`\
   &nbsp;&nbsp;&nbsp;&nbsp;b) -OR- Install the SDK to wherever you want, requiring you to run the script in #3 below.

   **Mac**: &nbsp; https://vulkan.lunarg.com/sdk/home#mac \
   &nbsp;&nbsp;&nbsp;&nbsp;Double-click the `.tar.gz` and drag/`mv` the extracted directory to either:\
   &nbsp;&nbsp;&nbsp;&nbsp;a) `./External/VulkanSDK` (renaming it)\
   &nbsp;&nbsp;&nbsp;&nbsp;b) -OR- wherever you want, while still renaming it as just `VulkanSDK` and running the script in #3.
2. Download the DEVELOPMENT LIBRARIES for both of these:\
  a) **SDL:** &nbsp; https://www.libsdl.org/download-2.0.php \
	 &nbsp;&nbsp;&nbsp;&nbsp;**Windows**: you're probably running 64-bit, so download: &nbsp; `SDL2-devel-2.0.<xx>-VC.zip`\
	 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(where "\<xx>" is the latest version number)\
	 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(DON'T bother with the "Runtime Binaries" -- they're just DLLs, lacking the .H files we need)\
	 &nbsp;&nbsp;&nbsp;&nbsp;**Mac**: download: &nbsp; `SDL2-2.0.<xx>.dmg`\
	 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Double-click and drag `SDL2.framework` into either `./External` or\
	 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;into your own location like 1b) above.\
  b) **SDL_image:** &nbsp; https://www.libsdl.org/projects/SDL_image/ \
	 &nbsp;&nbsp;&nbsp;&nbsp;Similar to above, you'll probably want to download: &nbsp; `SDL2_image-devel-2.0.<x>-VC.zip`\
	 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;("\<x>" is latest)\
	 &nbsp;&nbsp;&nbsp;&nbsp;and unzip it the same way.\

3. To set up symbolic links (skip this if you installed everything to the `./External` directory).
   edit and run the "setup1st" script for your specific platform as follows:<br/>\
    MAC - from Terminal:
    1. `cd` to where you downloaded/cloned and then into the `Xcode` directory.
    2. To make sure you have the relative path correct: `ls ../../<to directory where you downloaded #1 & #2 above>`
    3. Edit `setup1st.sh` and replace the `EXTERNAL_LIBRARY_PATH` with the one you just saw.
    4. Run the script:  `./setup1st.sh`
    5. Verify `External` directory created in project root and that its symbollic links list actual files/dirs.\

    WINDOWS - from Command Prompt:
    1. `cd` to where you downloaded/cloned and then into the `VisualStudio` directory.
    2. To make sure you have the relative path correct: `dir ..\..\<to directory where you downloaded #1 & #2 above>` 
    3. Edit `setup1st.bat` and replace the `EXTERNAL_LIBRARY_PATH` with the one you just saw.
    4. Run the batch file:  `./setup1st.bat`
    5. Verify `External` directory created in project root and that its junction links list actual files/dirs.


If the scripts don't work, here's how to manually replicate what they do.
FOR BUILD PROCESS TO FIND:
1. VulkanSDK:\
	 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`> cd ./External`<br/>
	 &nbsp;&nbsp;&nbsp;&nbsp;ON WINDOWS, for example:<br/>
	 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`> mklink /J VulkanSDK <path/to/where/you/installed/sdk>`<br/>
	 &nbsp;&nbsp;&nbsp;&nbsp;ON MAC:<br/>
	 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`> ln -s <path/to/where/you/installed/sdk> VulkanSDK`<br/>
	 &nbsp;&nbsp;&nbsp;&nbsp;Example from my machine:  `mklink /J VulkanSDK ..\..\Modules\3rdParty\VulkanSDK`
2. SDL:\
	 &nbsp;&nbsp;&nbsp;&nbsp;Link this unzipped library like #1 above:\
	 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`> mklink /J SDL2-2.0.<xx> <path/to/where/you/installed/sdl2>`\
	 &nbsp;&nbsp;&nbsp;&nbsp;Example from my machine:  `mklink /J SDL2-2.0.10 ..\..\Modules\3rdParty\SDL2-2.0.10`\
3. SDL_image:\
	 &nbsp;&nbsp;&nbsp;&nbsp;Link this the same way as #2 above.\
	 &nbsp;&nbsp;&nbsp;&nbsp;Example:  `mklink /J SDL2_image-2.0.5 \..\..\Modules\3rdParty\SDL2_image-2.0.5`

