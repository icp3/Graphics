//
// Created by ian on 7/2/19.
//


#include <Camera.h>

//Sets camera values to defaults
void CameraInit(Camera* camera)
{
    camera->Front = vec3(0.0f, 0.0f, -1.0f);
    camera->MouseSensitivity = SENSITIVITY;
    camera->MovementSpeed = SPEED;
    camera->Zoom = ZOOM;

    camera->Position = vec3(0.0f, 14.8f, 0.0f);
    camera->WorldUp  = vec3(0.0f, 1.0f, 0.0f);
    camera->Yaw = YAW;
    camera->Pitch = PITCH;

    CameraUpdateVectors(camera);
}

// Constructor with vectors
void CameraInit(Camera* camera, vec3 position, vec3 up, float yaw = YAW, float pitch = PITCH)
{
    camera->Front = vec3(0.0f, 0.0f, -1.0f);
    camera->MovementSpeed = SPEED;
    camera->MouseSensitivity = SENSITIVITY;
    camera->Zoom = ZOOM;

    camera->Position = position;
    camera->WorldUp = up;
    camera->Pitch = pitch;
    camera->Yaw = yaw;

    CameraUpdateVectors(camera);
}

// Constructor with scalar values
void CameraInit(Camera* camera, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
    camera->Front = vec3(0.0f, 0.0f, -1.0f);
    camera->MovementSpeed = SPEED;
    camera->MouseSensitivity = SENSITIVITY;
    camera->Zoom = ZOOM;

    camera->Position = vec3(posX, posY, posZ);
    camera->WorldUp  = vec3(upX, upY, upZ);
    camera->Yaw = yaw;
    camera->Pitch = pitch;
    CameraUpdateVectors(camera);
}