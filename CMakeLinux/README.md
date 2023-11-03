## Ubuntu &nbsp; Build Instructions

The following was tested on Ubuntu 22.04 running on a modern Intel-based PC with integrated graphics.

You will need:

1. **Vulkan SDK** Linux version

   LunarG develops this SDK's Linux version on Ubuntu and provides their builds, making it really easy to download/install via `apt`.

   Simply following the instructions at  https://vulkan.lunarg.com/sdk/home#linux  ...

   The **Ubuntu Packages** Tab says to do this: 
   ```ruby
   wget -qO- https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo tee /etc/apt/trusted.gpg.d/lunarg.asc
   sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.243-jammy.list https://packages.lunarg.com/vulkan/1.3.243/lunarg-vulkan-1.3.243-jammy.list
   sudo apt update
   sudo apt install vulkan-sdk
   ``` 

2. **SDL** Linux version

   Grab the Simple Directmedia Layer libraries:
   ```ruby
   sudo apt install libsdl2-2.0-0
   sudo apt install libsdl2-image-2.0-0
   ``` 
   And also install their header files via:
   ```ruby 
   sudo apt install libsdl2-dev
   sudo apt install libsdl2-image-dev
   ```
   SDL's source code has helpful documentation, available here:  https://github.com/libsdl-org/SDL/releases
  
3. **GLM** on Linux

   OpenGL's Math library is excellent, still totally relevant, and invaluable even for Vulkan:
   ```ruby
   sudo apt install libglm-dev
   ``` 

\
Installing this Project
-----------------------
```bash
cd <your project directory>
git clone https://github.com/electromaggot/HelloVulkanSDL.git
git clone https://github.com/electromaggot/VulkanModule.git
cd HelloVulkanSDL/CMakeLinux
```
To build the project:
```ruby
cmake .
make
```
To run:
```ruby
cd build
./HelloTriangle
```

