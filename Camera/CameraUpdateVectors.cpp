//
// Created by ian on 7/2/19.
//

#include <Camera.h>

void CameraUpdateVectors(Camera* camera)
{
    // Calculate the new Front vector
    vec3 front;
    front.x = cos(ToRadians(camera->Yaw)) * cos(ToRadians(camera->Pitch));
    front.y = sin(ToRadians(camera->Pitch));
    front.z = sin(ToRadians(camera->Yaw)) * cos(ToRadians(camera->Pitch));
    camera->Front = glm::normalize(front);

    // Also re-calculate the Right and Up vector
    // Normalize the vectors, because their length gets closer to 0
    // the more you look up or down which results in slower movement.
    camera->Right = glm::normalize(glm::cross(camera->Front, camera->WorldUp));
    camera->Up    = glm::normalize(glm::cross(camera->Right, camera->Front));
}