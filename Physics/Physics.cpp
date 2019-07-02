//
// Created by ian on 7/2/19.
//

#include <Physics.h>

bool InCube(vec3 TestPoint, vec3 Offset, vec3 Scale)
{
    if (TestPoint.x < Offset.x || (TestPoint.x > (Offset.x + Scale.x))) // X
    {
        return false;
    }
    if (TestPoint.z < Offset.z || (TestPoint.z > (Offset.z + Scale.z))) // Z
    {
        return false;
    }
    if (TestPoint.y < Offset.y || (TestPoint.y > (Offset.y + Scale.y))) // Y
    {
        return false;
    }

    return true;
}

void ProjectileInit(Projectile* projectile)
{
    projectile->Position = vec3(7, 14, 7);
    projectile->Mass = 1;
    projectile->Velocity = vec3(0,0,0);
}

void SimulateProjectile(Projectile* projectile, float DeltaTime)
{
    projectile->Position += projectile->Velocity * DeltaTime;
}

void ApplyForce(Projectile* projectile, vec3 force)
{
    projectile->Velocity += force / projectile->Mass;
}