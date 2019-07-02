

#pragma once

#include "Game_World.h"

#define GRAVITY -9.81f
#define MASS 1.0f

/*
  Small Instanced Objects: Bullets, Mags, Debras
  Big bois Objects: Blocks, enemies?

  Have a queue of uninitialized projectiles that get initialized every frame
  Have a different queue of projectiles that actually get operated on
*/

struct Projectile
{
    vec3 Position;
    vec3 Velocity;
    float Mass;
};

bool InCube(vec3 TestPoint, vec3 Offset, vec3 Scale);

void ProjectileInit(Projectile* projectile);

void SimulateProjectile(Projectile* projectile, float DeltaTime);

void ApplyForce(Projectile* projectile, vec3 force);

const char* ProjectiletVertSource = R"glsl(
    #version 330 core

	layout (location = 0) in vec3 Position;
	layout (location = 1) in vec3 Offset;

	uniform mat4 Proj;
	uniform mat4 View;

    void main()
    {
		gl_Position = Proj * View * vec4(Position + Offset, 1.0);
    }
		)glsl";

const char* ProjectileFragSource = R"glsl(
    #version 330 core

	out vec4 FragColor;

    void main()
    {
		//FragColor = vec4(.83,.67,.22,1);
		FragColor = vec4(1,1,0,1);
    }
		)glsl";
