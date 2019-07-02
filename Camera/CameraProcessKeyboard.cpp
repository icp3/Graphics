//
// Created by ian on 7/2/19.
//

#include <Camera.h>

void CameraProcessKeyboard(Camera* camera, int direction, float DeltaTime)
{
    float velocity = camera->MovementSpeed * DeltaTime;
    if (direction == CAM_FORWARD)  camera->Position += vec3(camera->Front.x, 0, camera->Front.z) * velocity;
    if (direction == CAM_BACKWARD) camera->Position -= vec3(camera->Front.x, 0, camera->Front.z) * velocity;
    if (direction == CAM_LEFT)     camera->Position -= camera->Right * velocity;
    if (direction == CAM_RIGHT)    camera->Position += camera->Right * velocity;
}

// DEBUG camera
// Processes input received from any keyboard-like input system.
void CameraProcessKeyboardDEBUG(Camera* camera, int direction, float DeltaTime)
{
    float velocity = camera->MovementSpeed * DeltaTime;
    if (direction == CAM_FORWARD)  camera->Position += camera->Front * velocity;
    if (direction == CAM_BACKWARD) camera->Position -= camera->Front * velocity;
    if (direction == CAM_LEFT)	   camera->Position -= camera->Right * velocity;
    if (direction == CAM_RIGHT)	   camera->Position += camera->Right * velocity;
}