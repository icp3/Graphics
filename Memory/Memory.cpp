//
// Created by ian on 7/2/19.
//

#include <Memory.h>

void ClearPage(Mem_Page* memory)
{
    void* mem = memory;

    uint page_size = MEM_PAGE_SIZE / 16;
    for (uint i = 0; i < page_size; ++i)
    {
        *(int64*)(mem) = 0;
        mem = ((int64*)mem) + 1;
    }
}

void GameMemoryInit(Game_Memory* GameMemory, Mem_Page* memory)
{
    //Give each game system the appropriate amount of pages
    for (uint i = 0; i < CHUNK_MEM_PAGES; ++i) GameMemory->ChunkPointers[i] = memory++;
    for (uint i = 0; i < PHYSX_MEM_PAGES; ++i) GameMemory->PhysxPointers[i] = memory++;
    for (uint i = 0; i < TEXT_MEM_PAGES;  ++i) GameMemory->TextPointers[i]  = memory++;
    for (uint i = 0; i < HUD_MEM_PAGES;   ++i) GameMemory->HUDPointers[i]   = memory++;
    for (uint i = 0; i < TIMER_MEM_PAGES; ++i) GameMemory->TimerPointers[i] = memory++;
}