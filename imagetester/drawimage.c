// Put any additional header includes here
#include "smugcat.h"
void drawImage(int width, int height, const unsigned short *image_data);
void draw_an_image() {

	// Make a call to draw the image here
	drawImage(SMUGCAT_WIDTH, SMUGCAT_HEIGHT, smugcat_data);

}

