//
// Created by ian on 7/2/19.
//
#include <HUD.h>

void TextStringInit(Text_String* TextString, vec2 Position, uint Length, const char* Text)
{
    TextString->Position = Position;
    TextString->Length = Length;

    for (uint i = 0; i < Length; ++i) TextString->Text[i] = *(Text + i);
}