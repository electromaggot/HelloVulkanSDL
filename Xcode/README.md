## Mac Build Instructions

0. A. Clone or unzip not only this project, but also https://github.com/electromaggot/VulkanModule into\
   &nbsp;&nbsp; &nbsp; the same parent project directory of your choosing.\
   B. Open the Mac **Terminal**.app, then\
   &nbsp;&nbsp; &nbsp; `cd HelloVulkanSDL` to this project's directory, which is assumed to be `./` when appearing below.\
   C. Set up **../VulkanModule** as a submodule (that's non-recursive or non-nested) outside this repository:\
   &nbsp;&nbsp; &nbsp; `.submodules/RunMe*.sh`

1. Download the latest **Vulkan SDK**: &nbsp; https://vulkan.lunarg.com/sdk/home#mac \
   In the Mac section under "SDK - SDK Installer," double-click the `.dmg` and `InstallVulkan` upon opening it.\
   For newer Macs with Apple Silicon, the SDK requires Rosetta, so click Install and run again.\
   When the installer asks "where the Vulkan SDK will be installed" you have a choice,\
   depending on whether you wish to use the SDK elsewhere, making it more accessible\
   for other projects (which is probably what you want).\
   **The HelloVulkanSDL Xcode project will be looking for: `./External/VulkanSDK`**\
   So either: a) install to a common directory like `/usr/local` or `/opt` or similar,\
   &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
                 then edit/run the script described in #4 below. &nbsp; &nbsp; ...OR:\
   &nbsp; &nbsp; &nbsp; b) `mkdir External` and install into that directory, if you think\
   &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; you'll only ever need the Vulkan SDK in this project.

3. Get the latest SDL files, either as **Source Code** or **`.dmg`** package, depending on your desired platform.\
   Each `.dmg` file only contains a macOS framework.  If you also want to target iOS, download the Source\
   Code instead, from which we'll build both iOS frameworks (instructions below) and the macOS frameworks.\
  a) **SDL:** &nbsp; https://www.libsdl.org/download-2.0.php \
	 &nbsp; &nbsp; Download: &nbsp; `SDL2-2.0.<xx>.zip`\
	 &nbsp; &nbsp; &nbsp; &nbsp; or alternately: &nbsp; `SDL2-2.0.<xx>.dmg`\
	 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; (where `<xx>` is the latest version number)\
	 &nbsp; &nbsp; Unzip and move the source directory (or double-click and drag `SDL2.framework`) into either `./External`\
	 &nbsp; &nbsp; or into your own location like 1b) above.\
  b) **SDL_image:** &nbsp; https://www.libsdl.org/projects/SDL_image/ \
	 &nbsp; &nbsp; Similar to above, you'll probably want to download: &nbsp; `SDL2_image-2.0.<x>.zip`\
	 &nbsp; &nbsp; &nbsp; &nbsp; or alternately: &nbsp; `SDL2_image-2.0.<x>.dmg`\
	 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; (`<x>` being latest)\
	 &nbsp; &nbsp; and unzip/double-click and relocate the contents the same way.

4. Lastly, grab **glm**: &nbsp; https://glm.g-truc.net \
	 Download using the upper-left link, e.g.: &nbsp; `glm-0.9.9.<x>.zip`\
	 unzip/double-click and move the directory as you did previously.

5. To set up symbolic links (skip this if you installed everything to the `./External` directory),\
   modify and run the `setup1st.sh` script* as follows. &nbsp; From **Terminal**:
	1. `cd` to where you downloaded/cloned, then into the **`Xcode`** directory.
	2. Make sure you have the relative path correct: `ls ../../<to directory where you downloaded #1 & #2 above>`
	3. Edit `setup1st.sh` and replace the `EXTERNAL_LIBRARY_PATH`'s value with the one you just saw.\
	   Also examine the SDK/SDL version numbers &mdash; which are part of the filenames/directory-names you downloaded
	   and extracted &mdash; to make sure those in the script are up-to-date and match exactly.  Save, exit.
	4. Run the script:  `./setup1st.sh`
	5. Verify `External` directory created in project root and that its symbolic links list actual files/dirs.

6. Building for **iOS** (which you downloaded the SDL source code for above) requires an iOS framework.
   SDL's own Xcode projects only build a framework for macOS (you'll need to do this) but not for iOS.  However this
   HelloVulkanSDL repo includes additional Xcode projects to build the iOS frameworks for SDL:
	1. `> cd ./Xcode/iOSFrameworks`
	2. `> open SDL-iOS.xcodeproj`
	3. Build the **BuildFatFramework** target &rarr; creates a `build` directory containing `SDL_iOS.framework` files.\
	   (If you see "BUILD SUCCEEDED" but then "disk I/O error" you can ignore that; the framework should now be built.)
	   Later, the HelloVulkanSDL project will know to find the iOS frameworks there.  As for the macOS ones, #3 above
	   already created links in the `./External` directory (or you dragged/copied the macOS frameworks yourself).
	4. `> open SDL_image-iOS.xcodeproj` and do the same.

7. Open `Xcode/HelloVulkanSDL.xcodeproj`, select a target, build, and run.\
	To try different demos, click in the window to cycle through them, or experiment with the\
	`HelloTriangle.cpp` file's `iMode` variable atop the `HelloApplication::Init()` method.

<br/>

\* - If, for whatever reason, the setup script fails to work, here's how to manually replicate what it does.\
FOR BUILD PROCESS TO FIND:
1. VulkanSDK:\
	`> cd ./External`\
	`> ln -s <path/to/where/you/installed/sdk> VulkanSDK`\
	Example from my machine:  `ln -s ..\..\Modules\3rdParty\vulkansdk-macos-1.1.126.0 VulkanSDK`
2. SDL:\
	Link the source directory like #1 above:\
	`> ln -s <path/to/where/you/installed/sdl2-2.0.<xx>> SDL`\
	Example from my machine:  `ln -s ..\..\Modules\3rdParty\SDL2-2.0.10 SDL`
3. SDL_image:\
	Link this the same way as #2 above.\
	Example:  `ln -s ..\..\Modules\3rdParty\SDL2_image-2.0.5 SDL_image`
4. glm:\
	`> ln -s <path/to/glm> glm`

