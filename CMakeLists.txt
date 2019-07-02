# Name: Ian Pirie
# Date: July 2,2019
# File: CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(Surface_Graphics)

include_directories( /usr/include/GLFW/)

find_package(OpenGL REQUIRED COMPONENTS OpenGL )
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

#Hud Libraries

add_library(Hud include/HUD.h Hud/TextStringInit.cpp Hud/TextToRenderable.cpp Hud/Quad.cpp Hud/Crosshair.cpp Hud/Button.cpp Hud/PlayerHUDInit.cpp)
add_library(Game_World Game_World/Chunk_Init.cpp Game_World/ChunkToRenderable.cpp Game_World/ChunkUpdate.cpp Game_World/GameWorldInit.cpp)
add_library(Camera Camera/CameraUpdateVectors.cpp Camera/CameraInit.cpp Camera/CameraProcessKeyboard.cpp Camera/CameraProcessMouse.cpp)
add_library(Memory include/Memory.h Memory/Memory.cpp)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES main.cpp include/Everything.h include/Enemy.h Hud Game_World Camera Memory Physics/Physics.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if("${CMAKE_SYSTEM}" MATCHES "Linux")
    include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ./include /usr/include/glm  /usr/include/GLFW /usr/include/GL /usr/include/GLFW /usr/include/stb )
elseif (WIN32)
    include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ./include)
endif()

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})