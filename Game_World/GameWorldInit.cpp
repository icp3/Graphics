//
// Created by ian on 7/2/19.
//

#include <Game_World.h>

void GameWorldInit(Game_World* world, float playerX, float playerZ, uint16* world_memory)
{
    for (uint i = 0; i < NUM_CHUNKS; ++i)
    {
        ChunkInit((world->Chunks + i), playerX, playerZ, (void*)world_memory);
    }
}