//
// Created by ian on 7/2/19.
//

#include <Camera.h>

void CameraProcessMouseMovement(Camera* camera, float xoffset, float yoffset, GLboolean constrainPitch = true)
{
    xoffset *= camera->MouseSensitivity;
    yoffset *= camera->MouseSensitivity;

    camera->Yaw += xoffset;
    camera->Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (camera->Pitch >  89.0f) camera->Pitch = 89.0f;
        if (camera->Pitch < -89.0f) camera->Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    CameraUpdateVectors(camera);
}

// Processes input received from a mouse scroll-wheel event.
// Only requires input on the vertical wheel-axis
void CameraProcessMouseScroll(Camera* camera, float yoffset)
{
    if (camera->Zoom >= 1.0f && camera->Zoom <= 45.0f) camera->Zoom -= yoffset;
    if (camera->Zoom <= 1.0f ) camera->Zoom = 1.0f;
    if (camera->Zoom >= 45.0f) camera->Zoom = 45.0f;
}