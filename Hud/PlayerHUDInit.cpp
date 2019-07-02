//
// Created by ian on 7/2/19.
//

#include <HUD.h>

void PlayerHUDInit(InventoryHUD* inventoryHUD, uint* TotalQuads)
{
    for (uint i = 0; i < INVENTORY_SIZE; ++i)
    {
        QuadInit(&inventoryHUD->Items[i], vec2(-.785 + (.13 * i), -.86), vec2(2, 2), vec3(.3, .3, .3), TotalQuads);
    }

    QuadInit(&inventoryHUD->Background, vec2(-.8, -.9), vec2(28, 4.25), vec3(.1, .1, .1), TotalQuads);
}

void PlayerHUDToRenderable(InventoryHUD* inventoryHUD, Quad_Renderable* memory)
{
    for (uint i = 0; i < INVENTORY_SIZE; ++i)
    {
        QuadToRenderable(&inventoryHUD->Items[i], memory);
    }

    QuadToRenderable(&inventoryHUD->Background, memory);
}