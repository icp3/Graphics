
#pragma once

#include "Camera.h"

// Number of chunks that render infront of you
// Total number of chunks rendered = ((n^2) * 4)
#define NUM_CHUNKS 1

#define CHUNK_X 16
#define CHUNK_Z 16
#define CHUNK_Y 16

#define TEXTURES_PER_ROW 4

/*
 chunks are 16x16 blocks of land. Chunks contain layers and are responsible
 for maintaining them. Chunks are identified by the x,z coordinate of the
 point nearest 0,0. All world coordinates are positive
*/

/*------------------------------- Chunk Logic -------------------------------*/



struct Chunk //typedef uint16 BlockID;
{
    union
    {
        ivec2 Coordinates;

        struct
        {
            uint32 x;
            uint32 z;
        };
    }; //Coordinates

    uint16* Blocks; // pointer to memory for block ID storage

    uint NumLayers; // Max height % 8(layers per mem page)
    uint NumBlocks; // Number of non-transparent(air) blocks
    uint MaxHeight; // Height of tallest block
};



struct Block_Renderable
{
    uint16 BlockID;
    vec3  Position;
    vec2 TexOffset;
};

struct Chunk_Renderable
{
    uint16 BlockID;
    vec3  Position;
    vec2 TexOffset;
};

struct Game_World
{
    Chunk Chunks[NUM_CHUNKS];
};

struct Water_Renderable
{
    vec3 Offset;
};
/* -------------------------------------------------------------------------
  Takes an (x,z) pair and returns a height at that location
  ------------------------------------------------------------------------- */
uint32 ChunkHeight(uint32 x, uint32 z)
{
    uint val = 12;
    if (val > 15) val = 15;
    if (val < 0) val = 0;
    return val;
}

void ChunkInit(Chunk* chunk, uint x, uint z);

void ChunkToRenderable(Chunk* chunk, Chunk_Renderable* buffer);

/* -------------------------------------------------------------------------
 Initializes a chunk & fills memory with appropriate block IDs
  ------------------------------------------------------------------------- */
void ChunkInit(Chunk* chunk, uint x, uint z, void* memory);

/* -------------------------------------------------------------------------
 Initializes a chunk & fills memory with appropriate block IDs
  ------------------------------------------------------------------------- */
void ChunkRemoveBlock(Chunk* chunk, uint x, uint z, uint y)
{
    if (chunk->Blocks[(x + (16 * z)) + (256 * y)])
    {
        chunk->Blocks[(x + (16 * z)) + (256 * y)] = AIR_ID;
    }
}

/* -------------------------------------------------------------------------
 This function loops through every block in a chunk, disgards all air blocks
 puts the block IDs & offsets into a buffer, and stores the size of the buffer.
  ------------------------------------------------------------------------- */
void ChunkUpdate(Chunk* chunk, Chunk_Renderable* buffer);

const char* WorldVertSource = R"glsl(
    #version 330 core

	layout (location = 0) in vec3 BasePos;
	layout (location = 1) in vec2 BaseTex;

	layout (location = 2) in vec3 PosOffset;
	layout (location = 3) in vec2 TexOffset;

	uniform mat4 Proj;
	uniform mat4 View;

	out vec2 TexCoord;

    void main()
    {
		gl_Position = Proj * View * vec4( BasePos + PosOffset , 1.0);

		TexCoord.x = (TexOffset.x + BaseTex.x);
		TexCoord.y = (TexOffset.y + BaseTex.y);
    }
		)glsl";

const char* WorldFragSource = R"glsl(
    #version 330 core

	in vec2 TexCoord;

	uniform sampler2D WorldTextures;

	out vec4 FragColor;

    void main()
    {
		FragColor = texture(WorldTextures, TexCoord);
    }
		)glsl";

/*------------------------------- Water Logic -------------------------------*/



const char* WaterVertSource = R"glsl(
    #version 330 core

	layout (location = 0) in vec3 Position;
	layout (location = 1) in vec3 Offset;

	uniform mat4 Proj;
	uniform mat4 View;
    uniform float Timer;

	out float Height;

    void main()
    {
		float x1 = Position.x + Offset.x;
		float x2 = Position.z + Offset.z;
		float factor = sqrt( (x1 * x1) + (x2 * x2) );

		vec3 FinalPos = Position + Offset;
		FinalPos.y += .025 * cos( (Timer * 1 * 3.14159) + (3.1415 * factor) );
		FinalPos.y -= .1;
		Height = cos( (Timer * 1 * 3.14159) + (3.1415 * factor) );

		gl_Position = Proj * View * vec4( FinalPos, 1);
	}
		)glsl";

const char* WaterFragSource = R"glsl(
    #version 330 core

	out vec4 FragColor;
	in float Height;

    void main()
    {
		float Might = Height * .2;
		//if(Height < .9) Might = 0;
		FragColor = vec4(Might,Might,1,1);
    }
		)glsl";

/*------------------------------- World Logic -------------------------------*/

void GameWorldInit(Game_World* world, float playerX, float playerZ, uint16* world_memory);