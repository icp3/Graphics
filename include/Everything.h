#pragma once

#define GLEW_STATIC
#define GLM_FORCE_RADIAN
#define STB_IMAGE_IMPLEMENTATION

#define DEBUG_MODE 1 //for debug options
#define SLOW_MODE  1 //for things that slow down the code

#ifdef WIN32
#include <Windows.h> //For Timing & Memory
#endif
#ifdef __linux__
#include <sys/mman.h>
#endif

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp> //For Math
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <stb_image.h> //For Image Loading

using glm::vec2; //TODO remove this
using glm::vec3; //TODO remove this
using glm::vec4; //TODO remove this
using glm::mat4; //TODO remove this
using glm::ivec2;

typedef signed   char  int8;
typedef signed   short int16;
typedef signed   int   int32;
typedef signed   long  int64;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef unsigned long  uint64;

typedef signed   int sint;
typedef unsigned int uint;
typedef unsigned int bool32;

typedef uint8 byte;

#define NULL 0

#define PI	  3.14159265359f
#define TWOPI 6.28318530718f

#define ToRadians(value) ( ((value) * PI) / 180.0f )
#define ToDegrees(value) ( ((value) * 180.0f) / PI )

#define vecmagnitude(vec) sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z))

#define global static

#if SLOW_MODE

#define Assert(expression) if( !(expression) ) *(int*)0 = 1;
#define vecout(vec) std::cout << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z << '\n'
#define out(val) std::cout << ' ' << val << '\n'
#else
#define Assert(expression)
#define vecout(vec)
#define out(val)
#endif

#if DEBUG_MODE
#define DebugShader(shader) GLint logSize = 0; glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize); char *errorLog = new char[logSize]; glGetShaderInfoLog(shader, logSize, NULL, errorLog); std::cout << errorLog;
#else
#define DebugShader(vert,frag)
#endif

/*------------------ Block Definitions ------------------*/
#define AIR_ID   0
#define STONE_ID 1
#define DIRT_ID  2
#define GRASS_ID 3
#define SAND_ID  4
#define WATER_ID 5
#define WOOD_ID  6
#define CLAY_BRICK_ID  7
#define STONE_BRICK_ID 8
#define COAL_ORE_ID 9
#define BEDROCK_ID 255