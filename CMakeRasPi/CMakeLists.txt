cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(HelloVulkanSDL CXX)


if(NOT CMAKE_PLATFORM_NAME)				# Set target architecture type if empty.
    set(CMAKE_PLATFORM_NAME "x64")
endif()
message("${CMAKE_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_PLATFORM_NAME}" STREQUAL "x64"
     OR "${CMAKE_PLATFORM_NAME}" STREQUAL "x86"))
    message(FATAL_ERROR "${CMAKE_PLATFORM_NAME} arch is not supported!")
endif()

set(CMAKE_CONFIGURATION_TYPES				# Global configuration types
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

include(CMake/Utils.cmake)				# Common utils

include(CMake/GlobalSettingsInclude.cmake OPTIONAL)	# Additional Global Settings(add specific info there)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)		# Use solution folders feature

set(PROJECT_NAME HelloTriangle)				# Sub-projects

set(EXECUTABLE_OUTPUT_PATH build)			# sub-directory destination for built binary


#################### Source groups ######################

set(HelloTriangle
    "../src/AppConstants.h"
    "../src/HelloTriangle.cpp"
    "../src/HelloTriangle.h"
    "../src/main.cpp"
)
source_group("HelloTriangle" FILES ${HelloTriangle})

set(HelloTriangle__Settings
    "../src/Settings/AppSettings.cpp"
    "../src/Settings/AppSettings.h"
    "../src/Settings/json.hpp"
)
source_group("HelloTriangle\\Settings" FILES ${HelloTriangle__Settings})

set(HelloTriangle__Shaders
    "../src/shaders/09_shader_base.frag"
    "../src/shaders/09_shader_base.vert"
    "../src/shaders/17_shader_vertexbuffer.frag"
    "../src/shaders/17_shader_vertexbuffer.vert"
    "../src/shaders/21_shader_ubo.frag"
    "../src/shaders/21_shader_ubo.vert"
    "../src/shaders/25_shader_textures.frag"
    "../src/shaders/25_shader_textures.vert"
    "../src/shaders/basicPlaid.frag"
    "../src/shaders/compile.bat"
    "../src/shaders/FullScreenTriangle.vert"
    "../src/shaders/TexturedQuad.frag"
    "../src/shaders/TexturedQuad.vert"
    "../src/shaders/tjVolcanic.frag"
)
source_group("HelloTriangle\\Shaders" FILES ${HelloTriangle__Shaders})

set(Platform
    "../src/VulkanModule/Platform/PlatformCommon.cpp"
    "../src/VulkanModule/Platform/PlatformExtension.h"
)
source_group("Platform" FILES ${Platform})

set(Platform__FileSystem
    "../src/VulkanModule/Platform/FileSystem/FileSystem.cpp"
    "../src/VulkanModule/Platform/FileSystem/FileSystem.h"
    "../src/VulkanModule/Platform/FileSystem/FileSystemSDL.h"
)
source_group("Platform\\FileSystem" FILES ${Platform__FileSystem})

set(Platform__GUISystem
    "../src/VulkanModule/Platform/GUISystem/stubs/imgui.h"
    "../src/VulkanModule/Platform/GUISystem/stubs/imgui_impl_sdl.cpp"
    "../src/VulkanModule/Platform/GUISystem/stubs/imgui_impl_sdl.h"
)
source_group("Platform\\GUISystem" FILES ${Platform__GUISystem})

set(Platform__ImageHandling
    "../src/VulkanModule/Platform/ImageHandling/iImageSource.h"
    "../src/VulkanModule/Platform/ImageHandling/ImageSDL.h"
    "../src/VulkanModule/Platform/ImageHandling/ImageSTB.h"
    "../src/VulkanModule/Platform/ImageHandling/sdlEquivalents.h"
)
source_group("Platform\\ImageHandling" FILES ${Platform__ImageHandling})

set(Platform__Logger
    "../src/VulkanModule/Platform/Logger/Logging.cpp"
    "../src/VulkanModule/Platform/Logger/Logging.h"
)
source_group("Platform\\Logger" FILES ${Platform__Logger})

set(Platform__Linux
    "src/LocalFileSystem.h"
    "src/optional.h"
    "src/PlatformConstants.h"
    "src/PlatformSpecifics.h"
)
source_group("Platform\\Linux" FILES ${Platform__Linux})

set(Platform__OSAbstraction
    "../src/VulkanModule/Platform/OSAbstraction/iPlatform.h"
    "../src/VulkanModule/Platform/OSAbstraction/PlatformGLFW.h"
    "../src/VulkanModule/Platform/OSAbstraction/PlatformSDL.cpp"
    "../src/VulkanModule/Platform/OSAbstraction/PlatformSDL.h"
)
source_group("Platform\\OSAbstraction" FILES ${Platform__OSAbstraction})

set(Platform__ControlScheme
    "../src/VulkanModule/Platform/ControlScheme/iControlScheme.h"
)
source_group("Platform\\ControlScheme" FILES ${Platform__ControlScheme})

set(Vulkan__AddOns
    "../src/VulkanModule/AddOns/BufferBase.h"
    "../src/VulkanModule/AddOns/CommandBufferBase.h"
    "../src/VulkanModule/AddOns/Descriptors.cpp"
    "../src/VulkanModule/AddOns/Descriptors.h"
    "../src/VulkanModule/AddOns/Mipmaps.h"
    "../src/VulkanModule/AddOns/PrimitiveBuffer.cpp"
    "../src/VulkanModule/AddOns/PrimitiveBuffer.h"
    "../src/VulkanModule/AddOns/TextureImage.cpp"
    "../src/VulkanModule/AddOns/TextureImage.h"
    "../src/VulkanModule/AddOns/UniformBuffer.cpp"
    "../src/VulkanModule/AddOns/UniformBuffer.h"
    "../src/VulkanModule/AddOns/UniformBufferLiterals.h"
)
source_group("Vulkan\\AddOns" FILES ${Vulkan__AddOns})

set(Vulkan__AddOns__3DObjects
    "../src/VulkanModule/AddOns/3DObjects/MeshObject.h"
    "../src/VulkanModule/AddOns/3DObjects/Quad2D.cpp"
    "../src/VulkanModule/AddOns/3DObjects/Quad2DColored.cpp"
    "../src/VulkanModule/AddOns/3DObjects/Quad2DTextured.cpp"
    "../src/VulkanModule/AddOns/3DObjects/Quad2DTextureTinted.cpp"
    "../src/VulkanModule/AddOns/3DObjects/Triangle2DColored.cpp"
)
source_group("Vulkan\\AddOns\\3DObjects" FILES ${Vulkan__AddOns__3DObjects})

set(Vulkan__AddOns__Renderables
    "../src/VulkanModule/AddOns/Renderables/AddOns.cpp"
    "../src/VulkanModule/AddOns/Renderables/AddOns.h"
    "../src/VulkanModule/AddOns/Renderables/FixedRenderable.cpp"
    "../src/VulkanModule/AddOns/Renderables/FixedRenderable.h"
    "../src/VulkanModule/AddOns/Renderables/iRenderable.h"
)
source_group("Vulkan\\AddOns\\Renderables" FILES ${Vulkan__AddOns__Renderables})

set(Vulkan__AddOns__VertexTypes
    "../src/VulkanModule/AddOns/VertexTypes/Vertex2D.h"
    "../src/VulkanModule/AddOns/VertexTypes/Vertex2DColored.h"
    "../src/VulkanModule/AddOns/VertexTypes/Vertex2DTextured.h"
    "../src/VulkanModule/AddOns/VertexTypes/Vertex2DTextureTinted.h"
    "../src/VulkanModule/AddOns/VertexTypes/VertexNull.h"
    "../src/VulkanModule/AddOns/VertexTypes/VertexAbstract.h"
    "../src/VulkanModule/AddOns/VertexTypes/VertexTypes.cpp"
)
source_group("Vulkan\\AddOns\\VertexTypes" FILES ${Vulkan__AddOns__VertexTypes})

set(Vulkan__Assist
    "../src/VulkanModule/Assist/DebugReport.cpp"
    "../src/VulkanModule/Assist/DebugReport.h"
    "../src/VulkanModule/Assist/DeviceAssessment.cpp"
    "../src/VulkanModule/Assist/DeviceAssessment.h"
    "../src/VulkanModule/Assist/DeviceProfile.h"
    "../src/VulkanModule/Assist/DeviceRanking.cpp"
    "../src/VulkanModule/Assist/DeviceRanking.h"
    "../src/VulkanModule/Assist/Helpers.h"
    "../src/VulkanModule/Assist/Utility.cpp"
    "../src/VulkanModule/Assist/Utility.h"
    "../src/VulkanModule/Assist/ValidationLayers.cpp"
    "../src/VulkanModule/Assist/ValidationLayers.h"
    "../src/VulkanModule/Assist/vkEnumStrings.cpp"
    "../src/VulkanModule/Assist/VulkanPlatform.h"
)
source_group("Vulkan\\Assist" FILES ${Vulkan__Assist})

set(Vulkan__Objects
    "../src/VulkanModule/Objects/CommandObjects.cpp"
    "../src/VulkanModule/Objects/CommandObjects.h"
    "../src/VulkanModule/Objects/DepthBuffer.cpp"
    "../src/VulkanModule/Objects/DepthBuffer.h"
    "../src/VulkanModule/Objects/DeviceQueues.cpp"
    "../src/VulkanModule/Objects/DeviceQueues.h"
    "../src/VulkanModule/Objects/EventObjects.cpp"
    "../src/VulkanModule/Objects/EventObjects.h"
    "../src/VulkanModule/Objects/Framebuffers.cpp"
    "../src/VulkanModule/Objects/Framebuffers.h"
    "../src/VulkanModule/Objects/GraphicsDevice.cpp"
    "../src/VulkanModule/Objects/GraphicsDevice.h"
    "../src/VulkanModule/Objects/GraphicsPipeline.cpp"
    "../src/VulkanModule/Objects/GraphicsPipeline.h"
    "../src/VulkanModule/Objects/ImageResource.cpp"
    "../src/VulkanModule/Objects/ImageResource.h"
    "../src/VulkanModule/Objects/RenderPass.cpp"
    "../src/VulkanModule/Objects/RenderPass.h"
    "../src/VulkanModule/Objects/ShaderModules.cpp"
    "../src/VulkanModule/Objects/ShaderModules.h"
    "../src/VulkanModule/Objects/Swapchain.cpp"
    "../src/VulkanModule/Objects/Swapchain.h"
    "../src/VulkanModule/Objects/SyncObjects.cpp"
    "../src/VulkanModule/Objects/SyncObjects.h"
    "../src/VulkanModule/Objects/VulkanInstance.cpp"
    "../src/VulkanModule/Objects/VulkanInstance.h"
    "../src/VulkanModule/Objects/WindowSurface.cpp"
    "../src/VulkanModule/Objects/WindowSurface.h"
)
source_group("Vulkan\\Objects" FILES ${Vulkan__Objects})

set(Vulkan__Setup
    "../src/VulkanModule/Setup/RenderSettings.h"
    "../src/VulkanModule/Setup/Shader.h"
    "../src/VulkanModule/Setup/VulkanConfigure.h"
    "../src/VulkanModule/Setup/VulkanSetup.cpp"
    "../src/VulkanModule/Setup/VulkanSetup.h"
    "../src/VulkanModule/Setup/VulkanSingleton.h"
)
source_group("Vulkan\\Setup" FILES ${Vulkan__Setup})

set(ALL_FILES
    ${HelloTriangle}
    ${HelloTriangle__Settings}
    ${HelloTriangle__Shaders}
    ${Platform}
    ${Platform__FileSystem}
    ${Platform__GUISystem}
    ${Platform__ImageHandling}
    ${Platform__Logger}
    ${Platform__Linux}
    ${Platform__OSAbstraction}
    ${Platform__ControlScheme}
    ${Vulkan__AddOns}
    ${Vulkan__AddOns__3DObjects}
    ${Vulkan__AddOns__Renderables}
    ${Vulkan__AddOns__VertexTypes}
    ${Vulkan__Assist}
    ${Vulkan__Objects}
    ${Vulkan__Setup}
)

#########################################################


add_executable(${PROJECT_NAME} ${ALL_FILES})		# Target

set(ROOT_NAMESPACE HelloTriangle)


if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Output directory
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build/${CMAKE_PLATFORM_NAME}/$<CONFIG>/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/${CMAKE_PLATFORM_NAME}/$<CONFIG>/"
    )
endif()
if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/$<CONFIG>/"
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()


if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Include directories
    target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/VulkanSDK/Include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/VulkanSDK/Third-Party/Include;"
	"/usr/include/SDL2;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/SDL2/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/SDL2_image/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/src;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/Settings;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Setup;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Assist;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/AddOns;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/AddOns/3DObjects;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/AddOns/Renderables;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/AddOns/VertexTypes;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Objects;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/OSAbstraction;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/ImageHandling;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/Logger;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/FileSystem;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/GUISystem;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/GUISystem/stubs;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/ControlScheme;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/imgui;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/imgui/examples;"
    )
endif()


if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Compile definitions
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "SDL_MAIN_HANDLED;"
        "_MBCS"
    )
elseif("${CMAKE_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "_MBCS"
    )
endif()


if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Post build events
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
	COMMAND if [ ! -e "build" ]\; then mkdir build\; fi
	COMMAND if [ ! -e "build/compiledShaders" ]\; then cd build\; ln -s "../../src/shaders" compiledShaders \; cd compiledShaders \; ./compileLinux.sh \; fi
        COMMAND if [ ! -L "build/textures" ]\; then cd build\; ln -s "../../assets/textures" \; fi
        COMMENT "Post-build events complete."
    )
endif()


#if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Custom build events
#    add_custom_command(
#        OUTPUT "compiledShaders"
#        COMMAND cd ../src/shaders
#        COMMAND compileLinux.sh
#        COMMENT "Shaders compiled."
#    )
#endif()


if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Dependencies
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "vulkan;"
        "SDL2;"
        "SDL2_image;"
	"m;"
	"stdc++"
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/VulkanSDK/Lib;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/VulkanSDK/Third-Party/Bin;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/SDL2/lib/${CMAKE_PLATFORM_NAME};"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/SDL2_image/lib/${CMAKE_PLATFORM_NAME}"
    )
endif()


target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gy
            >
            -ggdb3
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            -Wno-vla;
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )

target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
            >
            -fPIE;
        )

# END OF FILE

