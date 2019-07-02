#pragma once

#include "Everything.h"

#define MEM_PAGE_SIZE 4096

struct Mem_Page
{
    byte Memory[4096];
};

// Clears a page of memory
void ClearPage(Mem_Page* memory);

/* num pages each game system needs */
#define CHUNK_MEM_PAGES 4 // 1 page = 8 layers
#define PHYSX_MEM_PAGES 1
#define TEXT_MEM_PAGES  1 // 1 page = 256 strings
#define HUD_MEM_PAGES   1 // 1 page = 128 quads
#define TIMER_MEM_PAGES 1 // 1 page = 1024 tiemrs

#define TOTAL_MEM_PAGES (CHUNK_MEM_PAGES + PHYSX_MEM_PAGES + TEXT_MEM_PAGES + HUD_MEM_PAGES +\
						 TIMER_MEM_PAGES )

struct Game_Memory
{
    Mem_Page* ChunkPointers[CHUNK_MEM_PAGES];
    Mem_Page* PhysxPointers[PHYSX_MEM_PAGES];
    Mem_Page* TextPointers[TEXT_MEM_PAGES];
    Mem_Page* HUDPointers[HUD_MEM_PAGES];
    Mem_Page* TimerPointers[TIMER_MEM_PAGES];
};

void GameMemoryInit(Game_Memory* GameMemory, Mem_Page* memory);