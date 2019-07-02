#pragma once

#include "Memory.h"

#define CAM_FORWARD  0
#define CAM_BACKWARD 1
#define CAM_LEFT     2
#define CAM_RIGHT    3

// Default camera values
#define SENSITIVITY 0.1f
#define YAW  -90.0f
#define PITCH 0.0f
#define SPEED 2.5f
#define ZOOM  45.0f

// A camera object that processes input and calculates
// the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
struct Camera
{
    // Camera Attributes
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;

    // Camera options; Adjust accordingly
    float MouseSensitivity = 0.0f;
    float MovementSpeed = 0.0f;
    float Zoom = 0.0f;
};

// Calculates the front vector from the Camera's (updated) Euler Angles
void CameraUpdateVectors(Camera* camera);

//Sets camera values to defaults
void CameraInit(Camera* camera);

// Constructor with vectors
void CameraInit(Camera* camera, vec3 position, vec3 up, float yaw = YAW, float pitch = PITCH);

// Constructor with scalar values
void CameraInit(Camera* camera, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
inline glm::mat4 CameraGetViewMatrix(Camera* camera)
{
    return glm::lookAt(camera->Position, camera->Position + camera->Front, camera->Up);
}

// Regular player movement on a plane
// Processes input received from any keyboard-like input system.
void CameraProcessKeyboard(Camera* camera, int direction, float DeltaTime);

// DEBUG camera
// Processes input received from any keyboard-like input system.
void CameraProcessKeyboardDEBUG(Camera* camera, int direction, float DeltaTime);

// Processes input received from a mouse input system.
// Expects the offset value in both the x and y direction.
void CameraProcessMouseMovement(Camera* camera, float xoffset, float yoffset, GLboolean constrainPitch = true);

// Processes input received from a mouse scroll-wheel event.
// Only requires input on the vertical wheel-axis
void CameraProcessMouseScroll(Camera* camera, float yoffset);