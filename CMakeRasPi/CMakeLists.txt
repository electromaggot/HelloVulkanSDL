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

set(Graphics__Shaders
    "../graphics/shaders/09_shader_base.frag"
    "../graphics/shaders/09_shader_base.vert"
    "../graphics/shaders/17_shader_vertexbuffer.frag"
    "../graphics/shaders/17_shader_vertexbuffer.vert"
    "../graphics/shaders/21_shader_ubo.frag"
    "../graphics/shaders/21_shader_ubo.vert"
    "../graphics/shaders/25_shader_textures.frag"
    "../graphics/shaders/25_shader_textures.vert"
    "../graphics/shaders/basicPlaid.frag"
    "../graphics/shaders/FullScreenTriangle.vert"
    "../graphics/shaders/TexturedQuad.frag"
    "../graphics/shaders/TexturedQuad.vert"
    "../graphics/shaders/tjVolcanic.frag"
)
source_group("Graphics\\Shaders" FILES ${Graphics__Shaders})

set(Graphics__MeshObjects
    "../graphics/meshObjects/Quad2D.cpp"
    "../graphics/meshObjects/Quad2DColored.cpp"
    "../graphics/meshObjects/Quad2DTextured.cpp"
    "../graphics/meshObjects/Quad2DTextureTinted.cpp"
    "../graphics/meshObjects/Triangle2DColored.cpp"
)
source_group("Graphics\\MeshObjects" FILES ${Graphics__MeshObjects})

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

set(Vulkan__Adjunct
    "../src/VulkanModule/Adjunct/BufferBase.h"
    "../src/VulkanModule/Adjunct/CommandBufferBase.h"
    "../src/VulkanModule/Adjunct/Descriptors.cpp"
    "../src/VulkanModule/Adjunct/Descriptors.h"
    "../src/VulkanModule/Adjunct/Mipmaps.h"
    "../src/VulkanModule/Adjunct/PrimitiveBuffer.cpp"
    "../src/VulkanModule/Adjunct/PrimitiveBuffer.h"
    "../src/VulkanModule/Adjunct/TextureImage.cpp"
    "../src/VulkanModule/Adjunct/TextureImage.h"
    "../src/VulkanModule/Adjunct/UniformBuffer.cpp"
    "../src/VulkanModule/Adjunct/UniformBuffer.h"
    "../src/VulkanModule/Adjunct/UniformBufferLiterals.h"
)
source_group("Vulkan\\Adjunct" FILES ${Vulkan__Adjunct})

set(Vulkan__Adjunct__Renderables
    "../src/VulkanModule/Adjunct/Renderables/AddOns.cpp"
    "../src/VulkanModule/Adjunct/Renderables/AddOns.h"
    "../src/VulkanModule/Adjunct/Renderables/FixedRenderable.cpp"
    "../src/VulkanModule/Adjunct/Renderables/FixedRenderable.h"
    "../src/VulkanModule/Adjunct/Renderables/iRenderable.h"
    "../src/VulkanModule/Adjunct/Renderables/MeshObject.h"
)
source_group("Vulkan\\Adjunct\\Renderables" FILES ${Vulkan__Adjunct__Renderables})

set(Vulkan__Adjunct__VertexTypes
    "../src/VulkanModule/Adjunct/VertexTypes/Vertex3DTypes.h"
    "../src/VulkanModule/Adjunct/VertexTypes/Vertex2DTypes.h"
    "../src/VulkanModule/Adjunct/VertexTypes/VertexAttribute.h"
    "../src/VulkanModule/Adjunct/VertexTypes/VertexDescription.h"
    "../src/VulkanModule/Adjunct/VertexTypes/VertexAbstract.h"
    "../src/VulkanModule/Adjunct/VertexTypes/VertexNull.h"
    "../src/VulkanModule/Adjunct/VertexTypes/VertexNull.cpp"
)
source_group("Vulkan\\Adjunct\\VertexTypes" FILES ${Vulkan__Adjunct__VertexTypes})

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
    ${Graphics__Shaders}
    ${Graphics__MeshObjects}
    ${Platform}
    ${Platform__FileSystem}
    ${Platform__GUISystem}
    ${Platform__ImageHandling}
    ${Platform__Logger}
    ${Platform__Linux}
    ${Platform__OSAbstraction}
    ${Platform__ControlScheme}
    ${Vulkan__Adjunct}
    ${Vulkan__Adjunct__Renderables}
    ${Vulkan__Adjunct__VertexTypes}
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
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Adjunct;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Adjunct/Renderables;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Adjunct/VertexTypes;"
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
        COMMAND if [ ! -d "build" ]\; then mkdir build\; fi
        COMMAND if [ ! -L "build/compiledShaders" ]\; then cd build\; ln -s "../../CMakeLinux/build/compiledShaders" \; cd "../../graphics/shaders" \; ./compileLinux.sh\; fi
        COMMAND if [ ! -L "build/textures" ]\; then cd build\; ln -s "../../graphics/assets/textures" \; fi
        COMMENT "Post-build events complete."
    )
endif()


#if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Custom build events
#    add_custom_command(
#        OUTPUT "compiledShaders"
#        COMMAND cd ../graphics/shaders
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

