//
// Created by ian on 7/2/19.
//

#include <Game_World.h>

void ChunkToRenderable(Chunk* chunk, Chunk_Renderable* buffer)
{
    for (int i = 0; i < CHUNK_X; ++i)
    {
        for (int j = 0; j < CHUNK_Z; ++j)
        {
            for (int k = 0; k < CHUNK_Y; ++k) // k is world depth (min = 0)
            {
                uint16 BlockType = chunk->Blocks[(i + (16 * j)) + (256 * k)];

                if (BlockType) // only store block data for non-air
                {
                    buffer->BlockID = chunk->Blocks[i];
                    buffer->Position = vec3(i + chunk->x, k, j + chunk->z);

                    switch (BlockType)
                    {
                        // Row 0
                        case STONE_ID: buffer->TexOffset = vec2(0.0, 0.0); break;
                        case DIRT_ID:  buffer->TexOffset = vec2(.25, 0.0); break;
                        case GRASS_ID: buffer->TexOffset = vec2(.50, 0.0); break;
                        case SAND_ID:  buffer->TexOffset = vec2(.75, 0.0); break;
                            // Row 1
                        case WATER_ID: buffer->TexOffset = vec2(0.0, -.25); break;
                        case WOOD_ID:  buffer->TexOffset = vec2(.25, -.25); break;
                        case CLAY_BRICK_ID:  buffer->TexOffset = vec2(.50, -.25); break;
                        case STONE_BRICK_ID: buffer->TexOffset = vec2(.75, -.25); break;
                            // Row 2
                        case COAL_ORE_ID: buffer->TexOffset = vec2(0.0, -.50); break;
                        case BEDROCK_ID:
                            //Texture Not Found
                        default: buffer->TexOffset = vec2(.75, -.75);
                    }

                    ++buffer;
                    ++chunk->NumBlocks;
                }
            }
        }
    }
}