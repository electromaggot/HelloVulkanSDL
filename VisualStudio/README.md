## Windows Build Instructions

0. After cloning this project (or downloading/unzipping/dragging it, then opening **Command Prompt**),\
   `cd` to the project's directory, which is assumed to appear as `.\` as below.

1. Download the latest **Vulkan SDK**: &nbsp; https://vulkan.lunarg.com/sdk/home#windows \
   Run the `.exe` and when requested, either: a) change the Install directory as follows,\
   **The HelloVulkanSDL Visual Studio project will be looking for: &nbsp; `.\External\VulkanSDK`**\
   -OR- b) Install the SDK to wherever you want, requiring you to run the script in #3 below.
2. Download the DEVELOPMENT LIBRARIES for both of these:\
  a) **SDL:** &nbsp; https://www.libsdl.org/download-2.0.php \
	 &nbsp; &nbsp; You're probably running 64-bit Windows, so download: &nbsp; `SDL2-devel-2.0.<xx>-VC.zip`\
	 &nbsp; &nbsp; &nbsp; &nbsp; (where "\<xx>" is the latest version number)\
	 &nbsp; &nbsp; &nbsp; &nbsp; (DON'T bother with the "Runtime Binaries" -- they're just DLLs, lacking the .H files we need)\
	 &nbsp; &nbsp; Unzip and drag to either `.\External` or your own location like #1b) above.\
  b) **SDL_image:** &nbsp; https://www.libsdl.org/projects/SDL_image/ \
	 &nbsp; &nbsp; Similar to above, you'll probably want to download: &nbsp; `SDL2_image-devel-2.0.<x>-VC.zip`\
	 &nbsp; &nbsp; &nbsp; &nbsp; ("\<x>" is latest)\
	 &nbsp; &nbsp; and unzip it the same way.
3. Lastly, grab **glm**: &nbsp; https://glm.g-truc.net \
	 Download using the upper-left link, e.g.: &nbsp; `glm-0.9.9.<x>.zip`\
	 unzip/open/move the directory as you did previously.

4. To set up symbolic links (skip this if you installed everything to the `.\External` directory),\
   edit and run the `setup1st.bat` script* as follows. &nbsp; From **Command Prompt**:
    1. `cd` to where you downloaded/cloned, then into the **`VisualStudio`** directory.
    2. Make sure you have the relative path correct: `dir ..\..\<to directory where you downloaded #1 & #2 above>` 
    3. `notepad setup1st.bat` and replace the `EXTERNAL_LIBRARY_PATH`'s value with the one you just saw.
    4. Run the batch file:  `.\setup1st.bat`
    5. Verify `External` directory created in project root and that its junction links list actual files/dirs.

5. Open `VisualStudio\HelloVulkanSDL.sln`, build, and run.\
	To try different demos, change the `iMode` variable atop the `HelloApplication::Init()` method in the `HelloTriangle.cpp` file.

<br/>

\* - If, for whatever reason, the setup script fails to work, here's how to manually replicate what it does.\
FOR BUILD PROCESS TO FIND:
1. VulkanSDK:\
	`> cd .\External`<br/>
	`> mklink /J VulkanSDK <path\to\where\you\installed\sdk>`<br/>
	Example from my machine:  `mklink /J VulkanSDK ..\..\Modules\3rdParty\VulkanSDK`
2. SDL:\
	Link this unzipped library like #1 above:\
	`> mklink /J SDL2-2.0.<xx> <path\to\where\you\installed\sdl2>`\
	Example from my machine:  `mklink /J SDL2-2.0.10 ..\..\Modules\3rdParty\SDL2-2.0.10`
3. SDL_image:\
	Link this the same way as #2 above.\
	Example:  `mklink /J SDL2_image-2.0.5 ..\..\Modules\3rdParty\SDL2_image-2.0.5`
4. glm:\
	`> mklink /J glm <path\to\glm>`

