//
// Created by ian on 7/2/19.
//

#include <HUD.h>

void ButtonInit(Button* button, const char* text, uint length, vec2 position, vec3 color, uint* TotalQuads)
{
    QuadInit(&button->Background, position, vec2(length, 2), color, TotalQuads);
    TextStringInit(&button->Text, position, length, text);
}

void ButtonToRenderable(Button* button, uint* TotalChars, Quad_Renderable* quadMemory, Text_Renderable* textMemory)
{
    QuadToRenderable(&button->Background, quadMemory);
    TextToRenderable(&button->Text, textMemory, TotalChars);
}