//
// Created by ian on 7/2/19.
//

#include <HUD.h>


void CrosshairInit(Crosshair* cross, vec3 color, uint* TotalQuads)
{
    QuadInit(&cross->Vertical  , vec2(0, 0), vec2(.1, 1), color, TotalQuads);
    QuadInit(&cross->Horizontal, vec2(-.025, .05), vec2(1, .1), color, TotalQuads);
}

void CrosshairToRenderable(Crosshair* cross, Quad_Renderable* memory)
{
    QuadToRenderable(&cross->Vertical, memory);
    QuadToRenderable(&cross->Horizontal, memory);
}
