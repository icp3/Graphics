
#pragma once

#include "Enemy.h"

#define STRING_LENGTH 16

struct Text_String
{
    vec2 Position;
    uint Length;
    char Text[STRING_LENGTH];
};

struct Text_Renderable
{
    vec2  Position;
    uint8 XOffset;
    uint8 YOffset;
};

/* -------------------------------------------------------------------------
 Fills a char buffer with text and stores the length of the text

 > TextString : Pointer to the Text_String object that will be initialized
 > Length : Number of chars in the string
 > Text   : The string of text that will be stored
  ------------------------------------------------------------------------- */
void TextStringInit(Text_String* TextString, vec2 Position, uint Length, const char* Text);

/* -------------------------------------------------------------------------
 Converts a text string into a renderable and stores it in memory

 > string     : Object containing the text information
 > renderable : Memory that the renderable will be written to
 > TotalChars : Number of characters that are going to be rendered
  ------------------------------------------------------------------------- */
void TextToRenderable(Text_String* string, Text_Renderable* renderable, uint* TotalChars);

const char* TextVertSource = R"glsl(
    #version 330 core

	layout (location = 0) in vec2 Position;
	layout (location = 1) in vec2 TexCoord;

	layout (location = 2) in vec2 PosOffset;
	layout (location = 3) in vec2 TexOffset;

	out vec2 texCoord;

    void main()
    {
		texCoord = TexCoord + (TexOffset * vec2(.99,1));
		gl_Position = vec4( Position + PosOffset, 0.0, 1.0);
    }
		)glsl";

const char* TextFragSource = R"glsl(
    #version 330 core

	in vec2 texCoord;
	uniform sampler2D FontTextures;

	out vec4 FragColor;

    void main()
    {
		FragColor = texture(FontTextures, texCoord);

		if (FragColor.rgb == vec3(0,0,0))
		{
			discard;
		}
		else
		{
			FragColor = vec4(.8,.8,.8,1);
		}
    }
		)glsl";

/*------------------ Button Logic ------------------*/

struct Quad
{
    vec2 Position;
    vec2 Scale;
    vec3 Color;

    uint ID;
};

struct Quad_Renderable
{
    vec2 Position;
    vec2 Scale;
    vec3 Color;
};

void QuadInit(Quad* quad, vec2 position, vec2 scale, vec3 color, uint* TotalQuads);

void QuadToRenderable(Quad* quad, Quad_Renderable* renderable);

void QuadUpdatePos(uint ID, vec2 NewPos, Quad_Renderable* memory);

void QuadUpdateScale(uint ID, vec2 NewScale, Quad_Renderable* memory);

void QuadUpdateColor(uint ID, vec3 NewColor, Quad_Renderable* memory);

bool MouseInQuad(Quad* quad, float xPos, float yPos);

const char* HUDVertSource = R"glsl(
    #version 330 core

	layout (location = 0) in vec2 Position;

	layout (location = 1) in vec2 Offset;
	layout (location = 2) in vec2 Scale;
	layout (location = 3) in vec3 Color;

	out vec3 color;

    void main()
    {
		color = Color;
		gl_Position = vec4( (Position * Scale) + Offset, 0.0, 1.0);
    }
		)glsl";

const char* HUDFragSource = R"glsl(
    #version 330 core

	in vec3 color;

	out vec4 FragColor;

    void main()
    {
		FragColor = vec4(color, 1);
    }
		)glsl";

/*------------------ Button Logic ------------------*/

struct Crosshair
{
    Quad Vertical;
    Quad Horizontal;
};

void CrosshairInit(Crosshair* cross, vec3 color, uint* TotalQuads);

void CrosshairToRenderable(Crosshair* cross, Quad_Renderable* memory);

/*------------------ Button Logic ------------------*/
#define INVENTORY_SIZE 12
struct Button //Function pointers?
{
    Quad Background;
    Text_String Text;
};

void ButtonInit(Button* button, const char* text, uint length, vec2 position, vec3 color, uint* TotalQuads);

void ButtonToRenderable(Button* button, uint* TotalChars, Quad_Renderable* quadMemory, Text_Renderable* textMemory);

/*------------------ Inventory Screen Logic ------------------*/
#define INVENTORY_SIZE 12
struct InventoryHUD
{
    Quad Background;
    Quad Items[INVENTORY_SIZE];

};

void PlayerHUDInit(InventoryHUD* inventoryHUD, uint* TotalQuads);

void PlayerHUDToRenderable(InventoryHUD* inventoryHUD, Quad_Renderable* memory);