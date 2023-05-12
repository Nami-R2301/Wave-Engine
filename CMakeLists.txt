cmake_minimum_required(VERSION 3.12)
project(Wave-Engine)

set(CMAKE_CXX_STANDARD 20)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RELEASE")
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_BUILD_TYPE "DEBUG")
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_BUILD_TYPE "RELEASE")
endif ()

add_library(ImGUI STATIC ${CMAKE_SOURCE_DIR}/Wave/src/ImGUI/imGUI_build.cpp)

target_compile_definitions(ImGUI PUBLIC $<$<CONFIG:DEBUG>:IMGUI_IMPL_OPENGL_DEBUG>)

target_link_libraries(ImGUI PUBLIC OpenGL::GL)

target_include_directories(ImGUI PUBLIC
        ${CMAKE_SOURCE_DIR}/Wave/include/Dependencies/imGUI/
        ${CMAKE_SOURCE_DIR}/Wave/include/Dependencies/GL/)

add_library(Wave STATIC
        Wave/src/Core/engine.cpp
        Wave/src/Renderer/vertex.cpp
        Wave/src/Renderer/shader.cpp
        Wave/src/Renderer/color.cpp
        Wave/src/Core/engine_time.cpp
        Wave/src/Math/vector_2f.cpp
        Wave/src/Math/vector_3f.cpp
        Wave/src/Math/matrix_4f.cpp
        Wave/src/Math/vector_4f.cpp
        Wave/src/Utilities/logger.cpp
        Wave/src/Math/transform.cpp
        Wave/src/Utilities/res_loader.cpp
        Wave/src/Scene/camera.cpp
        Wave/src/Renderer/texture.cpp
        Wave/src/Renderer/gl_vertex_array_buffer.cpp
        Wave/src/Window/display_settings.cpp
        Wave/src/Renderer/renderer.cpp
        Wave/src/Window/window.cpp
        Wave/src/Events/key_event.cpp
        Wave/src/Events/mouse_event.cpp
        Wave/src/Events/app_event.cpp
        Wave/src/Input/input.cpp
        Wave/src/Objects/text.cpp
        Wave/src/Objects/gl_text.cpp
        Wave/src/Core/text_layer.cpp
        Wave/src/Objects/object.cpp
        Wave/src/Core/layer.cpp
        Wave/src/Core/layer_stack.cpp
        Wave/src/Renderer/buffer.cpp
        Wave/src/Renderer/gl_buffer.cpp
        Wave/src/Renderer/framebuffer.cpp
        Wave/src/Renderer/gl_framebuffer.cpp
        Wave/src/Renderer/vertex_array_buffer.cpp
        Wave/src/Events/renderer_event.cpp
        Wave/src/ImGUI/imGUI_layer.cpp
        Wave/src/Scene/editor_camera.cpp
        Wave/src/Window/glfw/glfw_window.cpp
        Wave/src/Renderer/gl_renderer.cpp
        Wave/src/Scene/scene.cpp
        Wave/src/Scene/entity.cpp
        Wave/src/Scene/scene_camera.cpp
        Wave/src/Renderer/gl_shader.cpp
        Wave/src/Renderer/uniform_buffer.cpp
        Wave/src/Renderer/gl_uniform_buffer.cpp)

find_package(Freetype REQUIRED CONFIG)
message("Freetype LIB DETECTED\n")


if (UNIX)
    set(CMAKE_INSTALL_PREFIX "/opt/Wave-Engine")
    message("Unix-like system detected. Setting up configuration for GNU...\n")

    find_package(GLEW REQUIRED)
    message("GLEW LIB DETECTED")
    find_package(glfw3 REQUIRED)
    message("GLFW LIB DETECTED\n")
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)

    if (${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
        target_compile_options(Wave PUBLIC
                -Wall
                -Wextra
                -Wpedantic
                -O1)
    elseif (${CMAKE_BUILD_TYPE} STREQUAL "RELEASE")
        target_compile_options(Wave PUBLIC -O2)
    endif ()

    target_compile_definitions(Wave PUBLIC
            WAVE_STATIC
            GLFW3_STATIC
            GLEW_STATIC
            ${CMAKE_BUILD_TYPE})

    target_precompile_headers(Wave PUBLIC Wave/include/wave_pch.h)

    target_link_libraries(Wave PUBLIC
            ImGUI
            freetype
            GLEW
            glfw)

    target_include_directories(Wave PUBLIC
            Wave/include
            Wave/include/Dependencies
            Wave/include/Dependencies/GL/glfw/include
            Wave/include/Dependencies/freetype/include)

    add_executable(App
            Example_app/src/example_app.cpp
            Example_app/src/example_layer.cpp)

    target_link_libraries(App PUBLIC Wave)

    target_precompile_headers(App PUBLIC Wave/include/wave_pch.h)

    add_executable(Editor
            Wave-Editor/src/editor.cpp
            Wave-Editor/src/editor_layer.cpp)

    target_include_directories(Editor PUBLIC
            Wave-Editor/src
            Wave-Editor/include
            Wave/include
            Wave/include/Dependencies
            Wave/include/Dependencies/GL/glfw/include
            Wave/include/Dependencies/imGUI/)

    target_compile_definitions(Editor PUBLIC EDITOR)

    target_link_libraries(Editor PUBLIC Wave)

    if (${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
        target_compile_options(Editor PUBLIC
                -Wall
                -Wextra
                -O1)
    elseif (${CMAKE_BUILD_TYPE} STREQUAL "RELEASE")
        target_compile_options(Editor PUBLIC -O2)
    endif ()

    target_precompile_headers(Editor PUBLIC Wave/include/wave_pch.h)

    install(TARGETS Editor
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

elseif (WIN32)
    message("Windows system detected. Setting up configuration for MSVCRT...\n")
    find_library(GLEW_LIB
            NAMES glew32s
            PATHS ${CMAKE_SOURCE_DIR}/Wave/libs/x64/glew
            NO_DEFAULT_PATH
            REQUIRED)
    message("GLEW LIB DETECTED -->  ${GLEW_LIB}")

    find_library(GLFW3_LIB
            NAMES glfw3
            PATHS ${CMAKE_SOURCE_DIR}/Wave/libs/x64/glfw
            NO_DEFAULT_PATH
            REQUIRED)
    message("GLFW LIB DETECTED -->  ${GLFW3_LIB} \n")

    find_package(OpenGL REQUIRED)
    message("OpenGL LIB DETECTED\n")

    find_library(GTEST_MAIN
            NAMES gtest_main
            PATHS ${CMAKE_SOURCE_DIR}/Test/libs/x86
            REQUIRED)
    find_library(GTEST
            NAMES gtest
            PATHS ${CMAKE_SOURCE_DIR}/Test/libs/x86
            REQUIRED)
    message("GTEST LIB DETECTED --> ${GTEST} \n")

    target_compile_definitions(Wave PUBLIC
            WAVE_PLATFORM_WINDOWS
            _WINDLL
            _UNICODE
            UNICODE
            WAVE_STATIC
            GLFW3_STATIC
            GLEW_STATIC
            _CRT_SECURE_NO_WARNINGS
            ${CMAKE_BUILD_TYPE})

    target_precompile_headers(Wave PUBLIC Wave/include/wave_pch.h)

    target_link_libraries(Wave PUBLIC
            imGUI
            user32
            gdi32
            shell32
            freetype
            ${GLEW_LIB}
            ${GLFW3_LIB})

    target_include_directories(Wave PUBLIC
            Wave/include
            Wave/include/Dependencies
            Wave/include/Dependencies/GL/glfw/include
            Wave/include/Dependencies/freetype/include)

    # WinMain (SUBSYSTEM:WINDOWS or SUBSYSTEM:CONSOLE).
    if (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
        add_executable(App WIN32
                Example_app/src/example_app.cpp
                Example_app/src/example_layer.cpp)
    else ()
        add_executable(App
                Example_app/src/example_app.cpp
                Example_app/src/example_layer.cpp)
    endif ()
    target_compile_definitions(App PUBLIC
            WIN32_LEAN_AND_MEAN
            ${CMAKE_BUILD_TYPE})
endif ()

# ------------------------------ COMMON STEPS FOR BOTH WIN32 AND UNIX -------------------------------
target_link_libraries(App PUBLIC Wave)

target_include_directories(App PUBLIC
        Wave/include
        Wave/include/Dependencies
        Wave/include/Dependencies/GL/glfw/include
        Example_app/include)

##        ------------------- TESTING -------------------

find_package(GTest)  # Optional
message("GoogleTest LIB DETECTED\n")

# -------- GOOGLE TEST --------------------------------------------------
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# -------------------------------------------------------------------------
enable_testing()

add_executable(Tests
        Example_app/src/example_app.cpp
        Example_app/src/example_layer.cpp
        Test/src/test_main.cpp
        Test/src/test_objects.cpp
        Test/src/test_camera.cpp)

target_compile_options(Tests PUBLIC)

if (WIN32)
    target_compile_definitions(Tests PUBLIC
            GLEW_STATIC
            GLFW3_STATIC
            TESTING
            _CRT_SECURE_NO_WARNINGS
            ${CMAKE_BUILD_TYPE})

    target_precompile_headers(Tests PUBLIC Wave/include/wave_pch.h)

    target_link_libraries(Tests PUBLIC
            Wave
            ${GTEST}
            ${GTEST_MAIN})

    target_include_directories(Tests PUBLIC
            Wave/include
            Wave/include/Dependencies
            Wave/include/Dependencies/GL/glfw/include
            Wave/include/Dependencies/freetype/include
            Wave/include/Dependencies/googleTest/googletest/include
            Wave/include/Dependencies/googleTest/googlemock/include
            Example_app/include)

elseif (UNIX AND (${CMAKE_BUILD_TYPE} STREQUAL DEBUG))
    target_compile_options(Tests PUBLIC -O1)

    target_compile_definitions(Tests PUBLIC
            GLEW_STATIC
            GLFW3_STATIC
            TESTING
            DEBUG)

    target_precompile_headers(Tests PUBLIC Wave/include/wave_pch.h)

    target_link_libraries(Tests PUBLIC
            Wave
            GTest::gtest_main)

    target_include_directories(Tests PUBLIC
            Wave/include
            Wave/include/Dependencies
            Wave/include/Dependencies/GL/glfw/include
            Wave/include/Dependencies/googleTest/googletest/include
            Wave/include/Dependencies/googleTest/googlemock/include
            Example_app/include)
endif ()
include(GoogleTest)
gtest_discover_tests(Tests)

# ---------------------------------------------------------------------------------
