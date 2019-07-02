//
// Created by ian on 7/2/19.
//
#include <HUD.h>

void TextToRenderable(Text_String* string, Text_Renderable* renderable, uint* TotalChars)
{
    renderable += *TotalChars;
    *TotalChars += string->Length;

    for (uint i = 0; i < string->Length; ++i)
    {
        switch (string->Text[i])
        {
            case 'a': renderable->XOffset = 0;   renderable->YOffset = 9;  break;
            case 'b': renderable->XOffset = 7;   renderable->YOffset = 9;  break;
            case 'c': renderable->XOffset = 14;  renderable->YOffset = 9;  break;
            case 'd': renderable->XOffset = 21;  renderable->YOffset = 9;  break;
            case 'e': renderable->XOffset = 28;  renderable->YOffset = 9;  break;
            case 'f': renderable->XOffset = 35;  renderable->YOffset = 9;  break;
            case 'g': renderable->XOffset = 42;  renderable->YOffset = 9;  break;
            case 'h': renderable->XOffset = 49;  renderable->YOffset = 9;  break;
            case 'i': renderable->XOffset = 56;  renderable->YOffset = 9;  break;
            case 'j': renderable->XOffset = 63;  renderable->YOffset = 9;  break;
            case 'k': renderable->XOffset = 70;  renderable->YOffset = 9;  break;
            case 'l': renderable->XOffset = 77;  renderable->YOffset = 9;  break;
            case 'm': renderable->XOffset = 84;  renderable->YOffset = 9;  break;
            case 'n': renderable->XOffset = 91;  renderable->YOffset = 9;  break;
            case 'o': renderable->XOffset = 98;  renderable->YOffset = 9;  break;
            case 'p': renderable->XOffset = 105; renderable->YOffset = 9;  break;
            case 'q': renderable->XOffset = 112; renderable->YOffset = 9;  break;
            case 'r': renderable->XOffset = 119; renderable->YOffset = 9;  break;
            case 's': renderable->XOffset = 126; renderable->YOffset = 9;  break;
            case 't': renderable->XOffset = 133; renderable->YOffset = 9;  break;
            case 'u': renderable->XOffset = 140; renderable->YOffset = 9;  break;
            case 'v': renderable->XOffset = 147; renderable->YOffset = 9;  break;
            case 'w': renderable->XOffset = 154; renderable->YOffset = 9;  break;
            case 'x': renderable->XOffset = 161; renderable->YOffset = 9;  break;
            case 'y': renderable->XOffset = 168; renderable->YOffset = 9;  break;
            case 'z': renderable->XOffset = 175; renderable->YOffset = 9;  break;

            case 'A': renderable->XOffset = 0;   renderable->YOffset = 19; break;
            case 'B': renderable->XOffset = 7;   renderable->YOffset = 19; break;
            case 'C': renderable->XOffset = 14;  renderable->YOffset = 19; break;
            case 'D': renderable->XOffset = 21;  renderable->YOffset = 19; break;
            case 'E': renderable->XOffset = 28;  renderable->YOffset = 19; break;
            case 'F': renderable->XOffset = 35;  renderable->YOffset = 19; break;
            case 'G': renderable->XOffset = 42;  renderable->YOffset = 19; break;
            case 'H': renderable->XOffset = 49;  renderable->YOffset = 19; break;
            case 'I': renderable->XOffset = 56;  renderable->YOffset = 19; break;
            case 'J': renderable->XOffset = 63;  renderable->YOffset = 19; break;
            case 'K': renderable->XOffset = 70;  renderable->YOffset = 19; break;
            case 'L': renderable->XOffset = 77;  renderable->YOffset = 19; break;
            case 'M': renderable->XOffset = 84;  renderable->YOffset = 19; break;
            case 'N': renderable->XOffset = 91;  renderable->YOffset = 19; break;
            case 'O': renderable->XOffset = 98;  renderable->YOffset = 19; break;
            case 'P': renderable->XOffset = 105; renderable->YOffset = 19; break;
            case 'Q': renderable->XOffset = 112; renderable->YOffset = 19; break;
            case 'R': renderable->XOffset = 119; renderable->YOffset = 19; break;
            case 'S': renderable->XOffset = 126; renderable->YOffset = 19; break;
            case 'T': renderable->XOffset = 133; renderable->YOffset = 19; break;
            case 'U': renderable->XOffset = 140; renderable->YOffset = 19; break;
            case 'V': renderable->XOffset = 147; renderable->YOffset = 19; break;
            case 'W': renderable->XOffset = 154; renderable->YOffset = 19; break;
            case 'X': renderable->XOffset = 161; renderable->YOffset = 19; break;
            case 'Y': renderable->XOffset = 168; renderable->YOffset = 19; break;
            case 'Z': renderable->XOffset = 175; renderable->YOffset = 19; break;

            case '0': renderable->XOffset = 0;   renderable->YOffset = 0;  break;
            case '1': renderable->XOffset = 7;   renderable->YOffset = 0;  break;
            case '2': renderable->XOffset = 14;  renderable->YOffset = 0;  break;
            case '3': renderable->XOffset = 21;  renderable->YOffset = 0;  break;
            case '4': renderable->XOffset = 28;  renderable->YOffset = 0;  break;
            case '5': renderable->XOffset = 35;  renderable->YOffset = 0;  break;
            case '6': renderable->XOffset = 42;  renderable->YOffset = 0;  break;
            case '7': renderable->XOffset = 49;  renderable->YOffset = 0;  break;
            case '8': renderable->XOffset = 56;  renderable->YOffset = 0;  break;
            case '9': renderable->XOffset = 63;  renderable->YOffset = 0;  break;

            case '!': renderable->XOffset = 70;  renderable->YOffset = 0;  break;
            case '@': renderable->XOffset = 77;  renderable->YOffset = 0;  break;
            case '#': renderable->XOffset = 84;  renderable->YOffset = 0;  break;
            case '$': renderable->XOffset = 91;  renderable->YOffset = 0;  break;
            case '%': renderable->XOffset = 98;  renderable->YOffset = 0;  break;
            case '^': renderable->XOffset = 105; renderable->YOffset = 0;  break;
            case '&': renderable->XOffset = 112; renderable->YOffset = 0;  break;
            case '*': renderable->XOffset = 119; renderable->YOffset = 0;  break;
            case '(': renderable->XOffset = 126; renderable->YOffset = 0;  break;
            case ')': renderable->XOffset = 133; renderable->YOffset = 0;  break;
            case '[': renderable->XOffset = 140; renderable->YOffset = 0;  break;
            case ']': renderable->XOffset = 147; renderable->YOffset = 0;  break;

            case ' ': renderable->XOffset = 200; renderable->YOffset = 200;  break;
        };

        renderable->Position = (string->Position + vec2(0.05 * i, 0));
        ++renderable;
    }
}
