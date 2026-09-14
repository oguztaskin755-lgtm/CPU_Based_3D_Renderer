cmake_minimum_required(VERSION 3.16)
project(3D_Renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(Includes)
include_directories(src)

#For generation of sdl static libs
set(SDL_SHARED OFF)
set(SDL_STATIC ON)

add_subdirectory(Dependencies/SDL EXCLUDE_FROM_ALL)

add_executable(3D_Renderer
        src/main.cpp
        src/Window.cpp
        Includes/Window.h
        Includes/ApplicationManager.h
        src/ApplicationManager.cpp
        src/Draw.cpp
        Includes/Draw.h
        Math/Includes/Vector.h
        Math/src/Vector.cpp
        Math/Includes/Transform.h
        Math/src/Transform.cpp
        src/Mesh.cpp
        Includes/Mesh.h
        src/Model.cpp
        Includes/Model.h
        Math/Includes/Matrix.h
        Includes/Light.h
        src/Light.cpp
        Includes/Texture.h
        src/Texture.cpp
        src/Camera.cpp
        Includes/Camera.h
        Includes/Clipping.h
        src/Clipping.cpp
        Includes/DeltaTime.h
        src/DeltaTime.cpp
        Includes/Geometry.h
        src/Geometry.cpp
        Includes/GameObject.h
        src/GameObject.cpp
        Includes/ExecutablePath.h
        src/ExecutablePath.cpp
)

target_link_libraries(3D_Renderer PRIVATE SDL3::SDL3-static)

add_custom_command(TARGET 3D_Renderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:3D_Renderer>/Assets
)