//
// Created by ian on 7/2/19.
//

#include <HUD.h>

void QuadInit(Quad* quad, vec2 position, vec2 scale, vec3 color, uint* TotalQuads)
{
    quad->Position = position;
    quad->Color = color;
    quad->Scale = scale;

    quad->ID = *TotalQuads;
    *TotalQuads += 1;
}

void QuadToRenderable(Quad* quad, Quad_Renderable* renderable)
{
    renderable += quad->ID;

    renderable->Position = quad->Position;
    renderable->Color = quad->Color;
    renderable->Scale = quad->Scale;
}

void QuadUpdatePos(uint ID, vec2 NewPos, Quad_Renderable* memory)
{
    memory += ID;

    memory->Position = NewPos;
}

void QuadUpdateScale(uint ID, vec2 NewScale, Quad_Renderable* memory)
{
    memory += ID;

    memory->Scale = NewScale;
}

void QuadUpdateColor(uint ID, vec3 NewColor, Quad_Renderable* memory)
{
    memory += ID;

    memory->Color    = NewColor;
}

bool MouseInQuad(Quad* quad, float xPos, float yPos)
{
    if (xPos < quad->Position.x) return false;
    if (yPos < quad->Position.y) return false;
    if (xPos > (quad->Position.x + (.05 * quad->Scale.x))) return false;
    if (yPos > (quad->Position.y + (.10 * quad->Scale.y))) return false;

    return true;
}