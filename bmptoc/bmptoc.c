// ***** YOUR NAME HERE *****

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

// This is the array into which you will load the raw data from the file
char data_arr[0x36 + 240 * 160 * 4];

int main(int argc, char *argv[]) {
	//int data_size = 0x36 + 240 * 160 * 4;
	// 1. Make sure the user passed in the correct number of arguments
	if (argc != 2) {
		printf("Number of arguments is invalid\n");
		return 0;
	}

	// 2. Open the file; if it doesn't exist, tell the user and then exit
	FILE *infile;
	char name_without_ext[(strlen(argv[1]) - 4)];
	strncpy(name_without_ext, argv[1], (strlen(argv[1]) -4));
	name_without_ext[(strlen(argv[1]) - 4)] = 0;
	printf("%s\n", name_without_ext);
	char nwe_UPPER[strlen(name_without_ext)];
	int ii;
	for (ii = 0; ii < strlen(name_without_ext); ii++) {
		if (name_without_ext[ii] > 96 && name_without_ext[ii] < 123) {
			nwe_UPPER[ii] = name_without_ext[ii] - 32;
		} else {
			nwe_UPPER[ii] = name_without_ext[ii];
		}
	}
	nwe_UPPER[strlen(name_without_ext)] = 0;
	printf("%s\n", nwe_UPPER);
	infile = fopen(argv[1], "r");
	if (infile == NULL) {
		printf("File does not exists\n");
		return 0;
	}


	// 3. Read the file into the buffer then close it when you are done
	int try = fread(data_arr, 4, data_size, infile);
	fclose(infile);
	// 4. Get the width and height of the image
	unsigned int width;
	width = *(unsigned int*)&data_arr[0x12];
	unsigned int height;
	height = *(unsigned int*)&data_arr[0x16];
	printf("%d\n", height);
	// 5. Create header file, and write header contents; close it
	char header_name[strlen(name_without_ext) + 2];
	sprintf(header_name, "%s.h", name_without_ext);
	FILE* outfile1 = fopen(header_name, "w");
	fprintf(outfile1, "#define %s_WIDTH %d\n", nwe_UPPER, width);
	fprintf(outfile1, "#define %s_HEIGHT %d\n", nwe_UPPER, height);
	fprintf(outfile1, "const unsigned short %s_data[%d]", name_without_ext, width * height);
	fclose(outfile1);
	// 6. Create c file, and write pixel data; close it
	FILE* outfile2 = fopen("%s.c", name_without_ext);
	fprintf(outfile2, "const unsigned short %s_data[15750] = {", name_without_ext);
	int kk;
	int jj;
	unsigned short bmp_pixel_chrome;
	unsigned short gba_pixel;
	for (kk = (0x36 + (width - 1) * height * 4); kk >= 0x36; kk -= (width * 4)) {
		for (jj = kk; jj < (kk + width * 4); jj += 4) {
			gba_pixel = 0;
			bmp_pixel_chrome = (unsigned short)(data_arr[jj]  >> 3);
			gba_pixel += bmp_pixel_chrome << 10;
			bmp_pixel_chrome = (unsigned short)(data_arr[jj + 1] >> 3);
			gba_pixel += bmp_pixel_chrome << 5;
			bmp_pixel_chrome = (unsigned short)(data_arr[jj + 2] >> 3);
			gba_pixel += bmp_pixel_chrome;
		}
	}



	return 0;
}

