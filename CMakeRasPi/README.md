## Raspberry Pi 5 &nbsp; Build Instructions

The following was tested on a Pi 5. It might work on Pi 4 (but Pi 3 and prior/lesser may waste your time).

### What is already installed

Pi5's Bookworm Linux comes with the following packages pre-installed (so you don't need to!)\
in their "stable" release versions for arm64:&emsp;...as of 10/31/2023\
&ensp; VULKAN: &ensp;                   `  libvulkan1 (1.3.239.0-1)                                                 `\
&ensp; SDL: &emsp; &nbsp; &nbsp; &nbsp; `  libsdl2-2.0-0 (2.26.5+dfsg-1)        libsdl2-image-2.0-0 (2.6.3+dfsg-1)  `\
&emsp;&emsp; &emsp; &emsp; &emsp;       `  libsdl2-mixer-2.0-0 (2.6.2+dfsg-2)   libsdl2-ttf-2.0-0 (2.20.1+dfsg-2)   `\
&emsp; &emsp; &emsp; &emsp; &emsp; &emsp;    and even these obsolete SDL versions:\
&emsp;&emsp; &emsp; &emsp; &emsp;       `  libsdl1.2debian (1.2.15+dfsg2-8)     libsdl-image1.2 (1.2.12-13+b2)      `\
The SDL part is encouraging, as it is specifically embraced by this project.

### What you will need to install

This project additionally requires Linux versions of the following:\
&emsp;&emsp; 1\. Vulkan **SDK**\
&emsp;&emsp;&emsp; Unlike the Ubuntu version, there's no "simple download" of this, so you'll have to build it.\
&emsp;&emsp; 2\. SDL "**-dev**" version\
&emsp;&emsp;&emsp; As mentioned in the previous section, SDL is mostly already present, but you'll need its header files as well.\
&emsp;&emsp; 3\. GLM

**1\. Vulkan SDK**\
&emsp; This is slightly more complicated than installing the Ubuntu version of VulkanSDK.\
&emsp; Requires two big steps:\
&emsp; &emsp; 1.1. Install pre-requisites for Raspberry Pi.\
&emsp; &emsp; 1.2. Download, extract, and build the SDK.
 
&emsp; **1.1.** installing **PREREQUISITES**\
&emsp;&emsp;&emsp; Following the instructions from:\
&emsp;&emsp;&emsp;&emsp;&emsp; https://vulkan.lunarg.com/doc/view/1.3.268.0/linux/getting_started.html#raspbian-raspberry-pi-os
```ruby
  $ sudo apt update
  $ sudo apt upgrade
```
&emsp; LunarG's instructions say to also do this, although I found it was already present &nbsp; &rarr; &nbsp; `$ sudo apt install xz-utils`\
&emsp; The following line installs the ICD or Installable Client Driver:
```ruby
  $ sudo apt install mesa-vulkan-drivers vulkan-tools

  $ apt list --installed | grep -i vulkan
```
&emsp; The above command should now show these additional Vulkan components as present:
```brainfuck
	mesa-vulkan-drivers/stable,now 23.2.1-0+rpt2 arm64 [installed]
	vulkan-tools/stable,now 1.3.239.0+dfsg1-1 arm64 [installed]
```
&emsp; Enter the following shell command; it does differ from LunarG's instructions (which fail)...
```ruby
  $ sudo apt install libglm-dev cmake libxcb-dri3-0 libxcb-present0 libpciaccess0 \
	libpng-dev libxcb-keysyms1-dev libxcb-dri3-dev libx11-dev g++ gcc \
	libwayland-dev libxrandr-dev libxcb-randr0-dev libxcb-ewmh-dev \
	python-is-python3 2to3 python3 bison libx11-xcb-dev liblz4-dev libzstd-dev python3-distutils \
	ocaml-core ninja-build pkg-config libxml2-dev wayland-protocols qtcreator \
	qtbase5-dev qt5-qmake qtbase5-dev-tools
```
> Side note - here were LunarG's failure points, modules that errored-out when trying to install them:\
> &emsp;&emsp;&emsp;&emsp;&emsp;`python` is perhaps deprecated, so replaced with `python-is-python3` `2to3`.\
> &emsp;&emsp;&emsp;&emsp;&emsp;`libmirclient-dev` also appears to be no longer available, so was removed.\
> &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;Remainder of build succeeds without it.  Note: `$ sudo apt list *libmir*`\
> &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;does not show "libmirclient"... (although is `libmirplatform` a replacement?)\
> &emsp;&emsp;&emsp;&emsp;&emsp;`git` is already present on Pi5, so its installation is redundant.

&emsp; Finally, during build, I saw a python error related to "jsonschema," so additionally do this:
```ruby
  $ sudo apt install python3-jsonschema
```

\
&emsp; **1.2.** building **VULKAN SDK**

\_\_\_\_**1.2.1.** DOWNLOAD________________________________________________________________________\
Go here, to the Linux section, and click "Latest SDK Tarball" to download it:\
&emsp; &emsp; https://vulkan.lunarg.com/sdk/home#linux \
(or on the "SDK Tarball" tab, click an "SDK" link to download an earlier version if you have trouble)\
The version number was 1.3.268.0 at the time of this writing, but the newest version should be fine.\
Don't worry that the tarball says "x86"... it will still build for arm64 a la Raspberry Pi.

\_\_\_\_**1.2.2.** EXTRACT__________________________________________________________________________\
Double-click the file - "xarchiver" should open and allow you to extract - or from command line `cd ~/Downloads`
and `tar xf` like the instructions say.  The result is a directory named the same as the version number.

LunarG's page instructs creating a `~/vulkan` directory to house the SDK, then sets environment variables
to this directory.  So consider relocating it somewhere more permanent, examples being `/opt/VulkanSDK` or
`/usr/local/src/VulkanSDK` or similar, at your discretion.

Let's assume the latter, **`/usr/local/src/`**, which we'll use for the rest of this document.  So move/rename the directory you
just extracted and make a symbolic link for convenience:
```powershell
  $ sudo mv 1.3.268.0 /usr/local/src/VulkanSDK_1.3.268.0
  $ cd /usr/local/src
  $ ln -s VulkanSDK_1.3.268.0 VulkanSDK
```
\_\_\_\_**1.2.3.** SET ENVIRONMENT VARIABLES______________________________________________________\
Now navigate to the new Vulkan directory and run the script located there:
```shell
  $ cd VulkanSDK
  $ source setup-env.sh
```
\_\_\_\_**1.2.4.** BUILD/INSTALL_____________________________________________________________________\
As per the LunarG instructions:  (beware! the following can take around 4.5 hours to
				  complete on a Pi5*, so you may want to run it overnight)
```ruby
  $ ./vulkansdk -j 1
```
(* - although CPU occupancy appeared to hover at 25%, so only one of the Cortex-A76's
     cores seems to have been used here by the terminal/shell-script.)
     
\_\_\_\_**1.2.5.** TEST IT OUT_______________________________________________________________________\
Since Pi Bookworm defaults to Wayland, run this and you should see LunarG's 3D spinning cube:
```ruby
  $ vkcube-wayland
```
(otherwise running `vkcube` or `vkcubepp` error-out with "Cannot connect to XCB" as they are intended
 for X-Windows, unless you have alternately set it as your windowing environment instead of Wayland)

\
**3. SDL-dev**

As said above, SDL is mostly already installed in the official Bookworm distribution for the Pi5.
However we'd like the version with header files too, so do this:
```ruby
  $ sudo apt install libsdl2-dev libsdl2-image-dev
```

**4. GLM**

Note that the Vulkan SDK installation steps (#1 above) already installed this!  So now we don't have to.


Installing this Project
-----------------------
```bash
  $ cd <your project directory>
  $ git clone https://github.com/electromaggot/HelloVulkanSDL.git
  $ git clone https://github.com/electromaggot/VulkanModule.git
  $ cd HelloVulkanSDL
```
To build the project:
```ruby
  $ cmake .
  $ make
```
***\[TODO:*** should make successfully!... however `compiledShaders/compileLinux.sh` could not find `glslangValidator`\
&emsp; which is in `VulkanSDK/1.3.268.0/aarch64/bin` -- instructions to be added here on how to copy or link or\
&emsp; include those into the shell's environment variables... ***FORTHCOMING!]***

Finally
-------
When running this program, you may see the following error messages print to the console:
```
Authorization required, but no authorization protocol specified

xkbcommon: ERROR: /usr/share/X11/locale/iso8859-1/Compose:39:34: string literal is not a valid UTF-8 string
                                                          . . .
xkbcommon: ERROR: /usr/share/X11/locale/iso8859-1/Compose:49:29: string literal is not a valid UTF-8 string
xkbcommon: ERROR: /usr/share/X11/locale/iso8859-1/Compose:49:29: too many errors
xkbcommon: ERROR: /usr/share/X11/locale/iso8859-1/Compose:49:29: failed to parse file
```
This can be "fixed" (although not such a major error) by changing your locale settings:
- go to: **Preferences** &rarr; **Raspberry Pi Configuration**
- tab: **Localisation**
- click: **Set Locale...**
- change **Character Set:** to **UTF-8**  (assuming it was ISO-8859-1 before)
