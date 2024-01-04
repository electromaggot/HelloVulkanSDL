cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

set(CMAKE_C_COMPILER /usr/bin/clang-15)
set(CMAKE_CPP_COMPILER /usr/bin/clang-cpp-15)
set(CMAKE_CXX_COMPILER /usr/bin/clang++-15)
set(CMAKE_LINKER /usr/bin/ld.lld-15)

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

AUX_SOURCE_DIRECTORY("src/" Platform_Linux)

AUX_SOURCE_DIRECTORY("../src/" HelloTriangle)
AUX_SOURCE_DIRECTORY("../src/Settings/" HelloTriangle_Settings)
AUX_SOURCE_DIRECTORY("../src/Extras/" HelloTriangle_Extras)

AUX_SOURCE_DIRECTORY("../graphics/shaders/" Graphics_Shaders)
AUX_SOURCE_DIRECTORY("../graphics/meshObjects/" Graphics_MeshObjects)

AUX_SOURCE_DIRECTORY("../src/VulkanModule/Platform/" Platform)
AUX_SOURCE_DIRECTORY("../src/VulkanModule/Platform/FileSystem/" Platform_FileSystem)
AUX_SOURCE_DIRECTORY("../src/VulkanModule/Platform/GUISystem/stubs/" Platform_GUISystem)
AUX_SOURCE_DIRECTORY("../src/VulkanModule/Platform/ImageHandling/" Platform_ImageHandling)
AUX_SOURCE_DIRECTORY("../src/VulkanModule/Platform/Logger/" Platform_Logger)

set(Platform_OSAbstraction
    "../src/VulkanModule/Platform/OSAbstraction/PlatformSDL.cpp"
)
source_group("Platform\\OSAbstraction" FILES ${Platform_OSAbstraction})

AUX_SOURCE_DIRECTORY("../src/VulkanModule/Adjunct/" Vulkan_Adjunct)

set(Vulkan_Adjunct_Renderables
    "../src/VulkanModule/Adjunct/Renderables/AddOns.cpp"
    "../src/VulkanModule/Adjunct/Renderables/FixedRenderable.cpp"
)
source_group("Vulkan\\Adjunct\\Renderables" FILES ${Vulkan_Adjunct_Renderables})

AUX_SOURCE_DIRECTORY("../src/VulkanModule/Adjunct/VertexTypes/" Vulkan_Adjunct_VertexTypes)

AUX_SOURCE_DIRECTORY("../src/VulkanModule/Assist/" Vulkan_Assist)

AUX_SOURCE_DIRECTORY("../src/VulkanModule/Objects/" Vulkan_Objects)

AUX_SOURCE_DIRECTORY("../src/VulkanModule/Setup/" Vulkan_Setup)

set(ALL_FILES
    ${HelloTriangle}
    ${HelloTriangle_Settings}
    ${HelloTriangle_Extras}
    ${Graphics_Shaders}
    ${Graphics_MeshObjects}
    ${Platform}
    ${Platform_FileSystem}
    ${Platform_GUISystem}
    ${Platform_ImageHandling}
    ${Platform_Logger}
    ${Platform_Linux}
    ${Platform_OSAbstraction}
    ${Platform_ControlScheme}
    ${Vulkan_Adjunct}
    ${Vulkan_Adjunct_Renderables}
    ${Vulkan_Adjunct_VertexTypes}
    ${Vulkan_Assist}
    ${Vulkan_Objects}
    ${Vulkan_Setup}
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
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/Extras;"
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
            -std=c++20
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            -Wno-vla;
            -Wno-reorder-init-list;
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

