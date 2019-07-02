//
// Created by ian on 7/2/19.
//

#include <Game_World.h>

void ChunkInit(Chunk* chunk, uint x, uint z)
{
    chunk->x = x;
    chunk->z = z;

    chunk->NumLayers = 0;
    chunk->NumBlocks = 0;
    chunk->MaxHeight = 0;

    //Fill memory with Block Id based on height function
    for (uint i = 0; i < CHUNK_X; ++i)
    {
        for (uint j = 0; j < CHUNK_Z; ++j)
        {
            uint height = ChunkHeight(i, j);
            if (height > chunk->MaxHeight) chunk->MaxHeight = height;

            for (uint k = 0; k < CHUNK_Y; ++k) // k is world depth (min = 0)
            {
                if (k > height)
                {
                    chunk->Blocks[(i + (16 * j)) + (256 * k)] = AIR_ID;
                }
                else if (k == height) // Top layer is grass
                {
                    chunk->Blocks[(i + (16 * j)) + (256 * k)] = GRASS_ID;
                }
                else if (k < height - 2) // Bottom Layers are stone
                {
                    chunk->Blocks[(i + (16 * j)) + (256 * k)] = STONE_ID;
                }
                else // Between grass and stone is dirt
                {
                    chunk->Blocks[(i + (16 * j)) + (256 * k)] = DIRT_ID;
                }
            }
        }
    }
}

void ChunkInit(Chunk* chunk, uint x, uint z, void* memory)
{
    chunk->x = x;
    chunk->z = z;

    chunk->Blocks = (uint16*)memory;

    chunk->NumLayers = 0;
    chunk->NumBlocks = 0;
    chunk->MaxHeight = 0;

    //Fill memory with Block Id based on height function
    for (uint i = 0; i < CHUNK_X; ++i)
    {
        for (uint j = 0; j < CHUNK_Z; ++j)
        {
            uint height = ChunkHeight(i, j);
            if (height > chunk->MaxHeight) chunk->MaxHeight = height;

            for (uint k = 0; k < CHUNK_Y; ++k) // k is world depth (min = 0)
            {
                if (k > height)
                {
                    chunk->Blocks[(i + (16 * j)) + (256 * k)] = AIR_ID;
                }
                else if (k == height) // Top layer is grass
                {
                    chunk->Blocks[(i + (16 * j)) + (256 * k)] = GRASS_ID;
                }
                else if (k < height - 2) // Bottom Layers are stone
                {
                    chunk->Blocks[(i + (16 * j)) + (256 * k)] = STONE_ID;
                }
                else // Between grass and stone is dirt
                {
                    chunk->Blocks[(i + (16 * j)) + (256 * k)] = DIRT_ID;
                }
            }
        }
    }
}