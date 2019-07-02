
#pragma once

#include "Physics.h"

struct Enemy
{
    uint Health;
    vec3 Position;
};

struct Enemy_Renderable
{
    vec3 Position;
};

const char* EnemyVertSource = R"glsl(
    #version 330 core

	layout (location = 0) in vec3 Position;
	layout (location = 1) in vec2 TexCoord;

	layout (location = 2) in vec3 PosOffset;

	uniform mat4 Proj;
	uniform mat4 View;

	out vec2 texCoord;

    void main()
    {
		texCoord = TexCoord;
		gl_Position =  Proj * View * vec4(Position + PosOffset, 1.0);
    }
		)glsl";

const char* EnemyFragSource = R"glsl(
    #version 330 core

	in vec2 texCoord;

	uniform sampler2D EnemyTextures;

	out vec4 FragColor;

    void main()
    {
		FragColor = texture(EnemyTextures, texCoord);
    }
		)glsl";
