cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

# Top-level build for HelloVulkanSDL, for every desktop platform.
#
#	git clone --recurse-submodules <this repo>
#	mkdir -p build && cd build && cmake .. && cmake --build . -j
#	./HelloTriangle
#
# The per-IDE directories alongside this file (Xcode/, VisualStudio/, CMakeLinux/, CMakeRasPi/)
#	remain for hand-maintained IDE projects; each also supplies the one app-side header the
#	module still asks of its consumer -- PlatformSpecifics.h -- which is why the include path
#	below picks the directory matching the host.  THIS file is what a plain `cmake ..` at the
#	repository root finds, which nothing previously did.

# Platform detection
if(WIN32)
    set(PLATFORM_NAME "Windows")
elseif(APPLE)
    set(PLATFORM_NAME "macOS")
elseif(UNIX AND NOT APPLE)
    if(EXISTS "/proc/device-tree/model")
        file(READ "/proc/device-tree/model" DEVICE_MODEL)
        if(DEVICE_MODEL MATCHES "Raspberry Pi")
            set(PLATFORM_NAME "RaspberryPi")
        else()
            set(PLATFORM_NAME "Linux")
        endif()
    else()
        set(PLATFORM_NAME "Linux")
    endif()
else()
    set(PLATFORM_NAME "Unknown")
endif()

message(STATUS "Building for platform: ${PLATFORM_NAME}")

project(HelloVulkanSDL CXX)

if(APPLE)					# Obj-C++ (.mm) support, needed by MacOSFullScreen.mm
    enable_language(OBJCXX)
endif()


# Set target architecture
if(NOT CMAKE_PLATFORM_NAME)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CMAKE_PLATFORM_NAME "x64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(CMAKE_PLATFORM_NAME "x86")
    else()
        set(CMAKE_PLATFORM_NAME "unknown")
    endif()
endif()

# ARM64 detection (Raspberry Pi 5, Apple silicon)
if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(CMAKE_PLATFORM_NAME "arm64")
endif()

message(STATUS "${CMAKE_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_PLATFORM_NAME}" STREQUAL "x64"
     OR "${CMAKE_PLATFORM_NAME}" STREQUAL "x86"
     OR "${CMAKE_PLATFORM_NAME}" STREQUAL "arm64"))
    message(FATAL_ERROR "${CMAKE_PLATFORM_NAME} architecture is not supported!")
endif()

set(CMAKE_CONFIGURATION_TYPES				# Global configuration types
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)		# Use solution folders feature

set(PROJECT_NAME HelloTriangle)


# VulkanModule arrives as src/VulkanModule -- a symlink to a peer checkout, or the submodule.
set(VULKAN_MODULE "${CMAKE_CURRENT_SOURCE_DIR}/src/VulkanModule")

if(NOT EXISTS "${VULKAN_MODULE}/Setup/VulkanSetup.h")
    message(FATAL_ERROR
        "VulkanModule not found at ${VULKAN_MODULE}.\n"
        "Either clone it as a peer and symlink it:\n"
        "    git clone https://github.com/electromaggot/VulkanModule ../VulkanModule\n"
        "    ln -s ../../VulkanModule src/VulkanModule\n"
        "or fetch the submodule:  git submodule update --init --recursive")
endif()


#################### Source groups ######################

AUX_SOURCE_DIRECTORY("src" HelloTriangle_src)
AUX_SOURCE_DIRECTORY("src/Settings" HelloTriangle_Settings)
AUX_SOURCE_DIRECTORY("src/Extras" HelloTriangle_Extras)

AUX_SOURCE_DIRECTORY("graphics/meshObjects" Graphics_MeshObjects)

AUX_SOURCE_DIRECTORY("${VULKAN_MODULE}/Platform/" Platform)
AUX_SOURCE_DIRECTORY("${VULKAN_MODULE}/Platform/FileSystem/" Platform_FileSystem)
AUX_SOURCE_DIRECTORY("${VULKAN_MODULE}/Platform/GUISystem/stubs/" Platform_GUISystem)
set(Platform_ImageHandling			# ImageSDLoptional.cpp is a mutually-exclusive
    "${VULKAN_MODULE}/Platform/ImageHandling/ImageSDL.cpp"	#  stand-in for ImageSDL.cpp (SDL
)						#  without SDL_image) -- never build both, they
						#  define the same class.  SDL_image is REQUIRED
						#  below, so use the full implementation.
AUX_SOURCE_DIRECTORY("${VULKAN_MODULE}/Platform/Logger/" Platform_Logger)

set(Platform_OSAbstraction
    "${VULKAN_MODULE}/Platform/OSAbstraction/PlatformSDL.cpp"
)
if(APPLE)					# PlatformSDL.cpp calls into these Obj-C++ routines
    list(APPEND Platform_OSAbstraction
        "${VULKAN_MODULE}/Platform/OSAbstraction/MacOSFullScreen.mm"
    )
    set_source_files_properties(		# see "Objective-C memory management (ARC)" in
        "${VULKAN_MODULE}/Platform/OSAbstraction/MacOSFullScreen.mm"	# VulkanModule's CLAUDE.md
        PROPERTIES COMPILE_OPTIONS "-fobjc-arc"
    )
endif()
source_group("Platform\\OSAbstraction" FILES ${Platform_OSAbstraction})

AUX_SOURCE_DIRECTORY("${VULKAN_MODULE}/Adjunct/" Vulkan_Adjunct)

AUX_SOURCE_DIRECTORY("${VULKAN_MODULE}/Adjunct/Renderables/" Vulkan_Adjunct_Renderables)
source_group("Vulkan\\Adjunct\\Renderables" FILES ${Vulkan_Adjunct_Renderables})

AUX_SOURCE_DIRECTORY("${VULKAN_MODULE}/Adjunct/VertexTypes/" Vulkan_Adjunct_VertexTypes)

AUX_SOURCE_DIRECTORY("${VULKAN_MODULE}/Assist/" Vulkan_Assist)	# note: not recursive, hence...
AUX_SOURCE_DIRECTORY("${VULKAN_MODULE}/Assist/ResourceTracker/" Vulkan_Assist_ResourceTracker)

AUX_SOURCE_DIRECTORY("${VULKAN_MODULE}/Objects/" Vulkan_Objects)

AUX_SOURCE_DIRECTORY("${VULKAN_MODULE}/Setup/" Vulkan_Setup)

set(ALL_FILES
    ${HelloTriangle_src}
    ${HelloTriangle_Settings}
    ${HelloTriangle_Extras}
    ${Graphics_MeshObjects}
    ${Platform}
    ${Platform_FileSystem}
    ${Platform_GUISystem}
    ${Platform_ImageHandling}
    ${Platform_Logger}
    ${Platform_OSAbstraction}
    ${Vulkan_Adjunct}
    ${Vulkan_Adjunct_Renderables}
    ${Vulkan_Adjunct_VertexTypes}
    ${Vulkan_Assist}
    ${Vulkan_Assist_ResourceTracker}
    ${Vulkan_Objects}
    ${Vulkan_Setup}
)

#########################################################


add_executable(${PROJECT_NAME} ${ALL_FILES})		# Target

set(ROOT_NAMESPACE HelloTriangle)


# Output directory: the binary lands directly in the build directory it was configured in, so
#	`cmake .. && cmake --build .` from build/ yields build/HelloTriangle, with its compiled
#	shaders and textures deployed beside it -- exactly where FileSystem.cpp looks for them.
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})	# multi-config generators (Visual Studio,
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG_UC)	#  Xcode) otherwise append a per-config
    set_target_properties(${PROJECT_NAME} PROPERTIES	#  subdirectory; keep them uniform so the
        RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UC} "${CMAKE_CURRENT_BINARY_DIR}"
    )							#  asset deployment paths stay predictable
endforeach()

if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64" OR "${CMAKE_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()


# Find required packages
find_package(Vulkan REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

# Find GLM
find_path(GLM_INCLUDE_DIR glm/glm.hpp
    HINTS
        "/opt/homebrew/include"  # macOS Homebrew
        "/usr/include"           # Linux
        "/usr/local/include"     # Generic Unix
        "C:/vcpkg/installed/x64-windows/include"  # Windows vcpkg
        "$ENV{VULKAN_SDK}/include"
)

# Platform-specific include directories
set(PLATFORM_INCLUDES)
if(APPLE)
    list(APPEND PLATFORM_INCLUDES
        "/opt/homebrew/include"
        "/opt/homebrew/include/SDL2"
    )
elseif(WIN32)
    list(APPEND PLATFORM_INCLUDES
        "$ENV{VULKAN_SDK}/include"
        "C:/vcpkg/installed/x64-windows/include"
        "C:/vcpkg/installed/x64-windows/include/SDL2"
    )
else()
    list(APPEND PLATFORM_INCLUDES
        "/usr/include"
        "/usr/include/SDL2"
        "/usr/local/include"
        "/usr/local/include/SDL2"
    )
endif()

# The app owes VulkanModule a PlatformSpecifics.h; each IDE directory keeps the variant suited
#	to its platform (the Apple one carries the iOS emplace_back workaround).  Reuse them rather
#	than duplicating a fourth copy here.
if(APPLE)
    set(APP_PLATFORM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Xcode/src")
elseif(WIN32)
    set(APP_PLATFORM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/VisualStudio/src")
else()
    set(APP_PLATFORM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLinux/src")
endif()

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${Vulkan_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIR}
    ${PLATFORM_INCLUDES}
    "${APP_PLATFORM_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Settings"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Extras"
    "${VULKAN_MODULE}/Setup"
    "${VULKAN_MODULE}/Assist"
    "${VULKAN_MODULE}/Assist/ResourceTracker"
    "${VULKAN_MODULE}/Adjunct"
    "${VULKAN_MODULE}/Adjunct/Renderables"
    "${VULKAN_MODULE}/Adjunct/VertexTypes"
    "${VULKAN_MODULE}/Objects"
    "${VULKAN_MODULE}/Platform"
    "${VULKAN_MODULE}/Platform/OSAbstraction"
    "${VULKAN_MODULE}/Platform/ImageHandling"
    "${VULKAN_MODULE}/Platform/Logger"
    "${VULKAN_MODULE}/Platform/FileSystem"
    "${VULKAN_MODULE}/Platform/GUISystem"
    "${VULKAN_MODULE}/Platform/GUISystem/stubs"
    "${VULKAN_MODULE}/Platform/ControlScheme"
)


# Compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "SDL_MAIN_HANDLED"
    "IMGUI_DISABLE"				# this demo builds GUISystem/stubs (above), not
)						#  External/imgui, so exclude ImGui globally.

# NOTE on handedness: no INVERT_Z_SETTING here, deliberately.  VulkanModule defaults to
#	standard right-handed Vulkan (+Z out of the screen, CCW front faces) and this demo's
#	geometry and camera are written to that convention.  Only an app that builds its own
#	left-handed projection/view matrices should define INVERT_Z_SETTING=true.

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "_MBCS"
        "WIN32_LEAN_AND_MEAN"
        "NOMINMAX"
        "VK_USE_PLATFORM_WIN32_KHR"
    )
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "VK_USE_PLATFORM_METAL_EXT"
    )
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "VK_USE_PLATFORM_XCB_KHR"
    )
endif()


#################### Shaders ############################
#
# Compile GLSL -> SPIR-V as part of the build, then deploy alongside the binary.  Previously
#	this was a post-build shell script (compileApple.sh / compileLinux.sh / compile.bat), one
#	per platform, each hardcoding a glslangValidator path.  find_package(Vulkan) locates the
#	compiler for us as Vulkan_GLSLC_EXECUTABLE, on every platform, with no script to maintain.
#
#	NOTE: the deployed destination must match SHADER_SUBDIRECTORY in
#	 VulkanModule/Platform/FileSystem/FileSystem.cpp ("shaders/compiled/"), which is where the
#	 runtime actually looks.  Keep the two in sync.
#
#	Naming: <base>.<stage> compiles to <base>-<stage>.spv (21_shader_ubo.vert ->
#	 21_shader_ubo-vert.spv), matching how OtherDrawables.cpp names the shaders it requests.

set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/graphics/shaders")
set(SHADER_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders/compiled")

file(GLOB SHADER_SOURCES "${SHADER_SOURCE_DIR}/*.vert" "${SHADER_SOURCE_DIR}/*.frag"
                         "${SHADER_SOURCE_DIR}/*.comp" "${SHADER_SOURCE_DIR}/*.geom"
                         "${SHADER_SOURCE_DIR}/*.tesc" "${SHADER_SOURCE_DIR}/*.tese")

if(Vulkan_GLSLC_EXECUTABLE AND SHADER_SOURCES)

    set(SPIRV_BINARIES)
    foreach(SHADER_SOURCE ${SHADER_SOURCES})
        get_filename_component(SHADER_BASE "${SHADER_SOURCE}" NAME_WE)
        get_filename_component(SHADER_TYPE "${SHADER_SOURCE}" LAST_EXT)
        string(SUBSTRING "${SHADER_TYPE}" 1 -1 SHADER_STAGE)		# ".vert" -> "vert"
        set(SPIRV "${SHADER_BINARY_DIR}/${SHADER_BASE}-${SHADER_STAGE}.spv")

        add_custom_command(
            OUTPUT "${SPIRV}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${SHADER_BINARY_DIR}"
            COMMAND ${Vulkan_GLSLC_EXECUTABLE} "${SHADER_SOURCE}" -o "${SPIRV}"
            DEPENDS "${SHADER_SOURCE}"
            COMMENT "Compiling shader ${SHADER_BASE}${SHADER_TYPE}"
            VERBATIM
        )
        list(APPEND SPIRV_BINARIES "${SPIRV}")
    endforeach()

    add_custom_target(CompileShaders DEPENDS ${SPIRV_BINARIES})
    add_dependencies(${PROJECT_NAME} CompileShaders)

    add_custom_command(					# deploy where the runtime looks for them
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/compiled"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SPIRV_BINARIES}
                                                     "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/compiled/"
        COMMENT "Deploying compiled shaders"
    )

else()

    message(FATAL_ERROR "glslc not found (or no GLSL sources in ${SHADER_SOURCE_DIR}).  "
                        "Install the Vulkan SDK, which supplies it -- every demo here needs "
                        "shaders, so there is nothing to run without it.")

endif()

#################### Textures ###########################
#
# Several demos load textures.  TEXTURE_SUBDIRECTORY in FileSystem.cpp is "assets/texture/"
#	(singular), while the repository stores them under graphics/assets/textures/ (plural) --
#	hence the rename in the destination path rather than a straight copy.

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/graphics/assets/textures"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/texture"
    COMMENT "Deploying textures"
)


# Platform-specific library dependencies
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    ${Vulkan_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

if(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa)		# for MacOSFullScreen.mm
    list(APPEND ADDITIONAL_LIBRARY_DEPENDENCIES
        "m"					# (in libSystem here, so a no-op -- kept for symmetry)
        ${COCOA_FRAMEWORK}
    )						# Note: no "stdc++".  Clang maps -lstdc++ to -lc++ on macOS,
						#  and clang++ as the link driver already links libc++
						#  implicitly, so naming it here asks for it a second time.
elseif(WIN32)
    list(APPEND ADDITIONAL_LIBRARY_DEPENDENCIES
        "user32"
        "gdi32"
        "shell32"
    )
else()
    list(APPEND ADDITIONAL_LIBRARY_DEPENDENCIES
        "m"
        "stdc++"
        "pthread"
        "dl"
    )
endif()

# pkg-config reports SDL2 as a dependency of SDL2_image, so SDL2 arrives twice: once from
#	SDL2_LIBRARIES and again inside SDL2_IMAGE_LIBRARIES.  CMake preserves repeated entries
#	(ordering can matter for static archives) by re-emitting everything that follows them, so
#	that one duplicate also drags m/stdc++/Cocoa onto the link line twice -- which the linker
#	then warns about.  These are all shared libraries, whose order is irrelevant, so simply
#	drop the repeats.  (REMOVE_DUPLICATES keeps each item's FIRST occurrence.)
list(REMOVE_DUPLICATES ADDITIONAL_LIBRARY_DEPENDENCIES)

target_link_libraries(${PROJECT_NAME} PRIVATE ${ADDITIONAL_LIBRARY_DEPENDENCIES})

# Platform-specific library directories
if(APPLE)
    target_link_directories(${PROJECT_NAME} PRIVATE
        "/opt/homebrew/lib"
    )
elseif(WIN32)
    target_link_directories(${PROJECT_NAME} PRIVATE
        "$ENV{VULKAN_SDK}/lib"
        "C:/vcpkg/installed/x64-windows/lib"
    )
else()
    target_link_directories(${PROJECT_NAME} PRIVATE
        "/usr/lib"
        "/usr/local/lib"
        "/usr/lib/aarch64-linux-gnu"  # Raspberry Pi 5 ARM64
    )
endif()

# Platform-specific compiler options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:Release>:/O2 /Oi /Gy>
        /std:c++20
        /W3
        /EHsc
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O2 -DNDEBUG>
        -std=c++20
        -Wall
        -Wno-vla
        -Wno-reorder-init-list	# designated initializers, deliberately out of declaration order
    )

    if(NOT APPLE)
        target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
    endif()

    if(PLATFORM_NAME STREQUAL "RaspberryPi")
        target_compile_options(${PROJECT_NAME} PRIVATE
            -march=armv8-a
            -mtune=cortex-a76
        )
    elseif("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE -m64)
    endif()
endif()

# Platform-specific linker options
if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/OPT:REF /OPT:ICF>
        $<$<CONFIG:Debug>:/DEBUG:FULL>
    )
else()
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:-s>  # Strip symbols in release
    )

    if(NOT APPLE)
        target_link_options(${PROJECT_NAME} PRIVATE -fPIE)
    endif()

    if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE -m64)
    endif()
endif()

# END OF FILE
