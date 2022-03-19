#include <nds.h>
#include <stdio.h>
#include <string.h>

#include <array>
#include <string>

SpriteEntry OAMCopy[128];
#include "ball.h"
#include "groner.hpp"

typedef uint16_t u16; 
typedef gro::vertex vertex;  

constexpr u16 MAX_VERTEX_COUNT = 128;
constexpr u16 THINNING_RADIUS = 4;


char text[256];
u16 text_pos = 0;
 
int main(void)
{
	touchPosition touch;
	bool recognition_performed = true;

	// 
	u16 vertex_index = 0;
	std::array<vertex, MAX_VERTEX_COUNT> vertices;
	std::vector<vertex> processed_vertices({});
	processed_vertices.reserve(MAX_VERTEX_COUNT);

	consoleDemoInit();

	// put the main screen on the top lcd
	lcdMainOnTop();
   
   //set the video mode
    videoSetMode(  MODE_0_2D | 
                   DISPLAY_SPR_ACTIVE |		//turn on sprites
                   DISPLAY_BG0_ACTIVE |		//turn on background 0
                   DISPLAY_SPR_1D			//this is used when in tile mode
                    );
 
	while(1)
	{


		// read the button states
		scanKeys();

		// read the touchscreen coordinates
		touchRead(&touch);
		
		int held = keysHeld();		// buttons currently held

		if (!(held & KEY_TOUCH))
		{
			if (!recognition_performed)
			{
				processed_vertices.clear();
				processed_vertices.insert(processed_vertices.end(), &vertices[0], &vertices[vertex_index]);
				vertex_index = 0;
				gro::smoothVertices(processed_vertices);
				gro::thinVertices(processed_vertices, THINNING_RADIUS);
				text[text_pos] = gro::recognizeNumeric(processed_vertices);
				text_pos++;
				recognition_performed = true;
			}
		}
		else if (vertex_index < MAX_VERTEX_COUNT)
		{
			vertices[vertex_index] = vertex{ touch.px, touch.py };
			vertex_index++;
			recognition_performed = false;
		}

		iprintf("%s", text);

		swiWaitForVBlank();
		consoleClear();
	}
	
 
	return 0;
}

