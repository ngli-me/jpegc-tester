#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jerror.h>
#include <jpeglib.h>

struct rawImage {
    unsigned int components;
    unsigned long int width, height;
    
    unsigned char* lpData;
};

struct rawImage* loadJpegImageFile(char* lpFilename) {
    struct jpeg_decompress_struct info;
    struct jpeg_error_mgr err;

    struct rawImage* lpNewImage;

    unsigned long int imgWidth, imgHeight;
    int numComponents;

    unsigned long int dwBufferBytes;
    unsigned char* lpData;

    unsigned char* lpRowBuffer[1];

    FILE* fHandle;

    fHandle = fopen(lpFilename, "rb");
    if (fHandle == NULL) {
        #ifdef DEBUG
        fprintf(stderr, "%s:%u: Failed to read file %s\n", __FILE__, __LINE__, lpFilename);
        #endif
        return NULL; // TODO: better err handling
    }

    info.err = jpeg_std_error(&err);
    jpeg_create_decompress(&info);

    jpeg_stdio_src(&info, fHandle);
    jpeg_read_header(&info, TRUE);

    jpeg_start_decompress(&info);
    imgWidth  = info.output_width;
    imgHeight = info.output_height;
    numComponents = info.num_components;

    #ifdef DEBUG
    #endif

    dwBufferBytes = imgWidth * imgHeight * 3; // only reading RGB
    lpData = (unsigned char*)malloc(sizeof(unsigned char)*dwBufferBytes);

    lpNewImage = (struct rawImage*) malloc(sizeof(struct rawImage));
    lpNewImage->components = numComponents;
    lpNewImage->width = imgWidth;
    lpNewImage->height = imgHeight;
    lpNewImage->lpData = lpData;

    /* Read scanline by scanline */
    while (info.output_scanline < info.output_height) {
        lpRowBuffer[0] = (unsigned char*) (&lpData[3*info.output_width*info.output_scanline]);
        jpeg_read_scanlines(&info, lpRowBuffer, 1);
    }

    jpeg_finish_decompress(&info);
    jpeg_destroy_decompress(&info);
    fclose(fHandle);

    return lpNewImage;
}

int main() {
    struct rawImage* img = loadJpegImageFile("peppers.jpg");
    if (!img) {
        printf("Error while reading");
        return 1;
    }


    FILE* r_channel = fopen("r_buf.txt", "rb");
    FILE* g_channel = fopen("g_buf.txt", "rb");
    FILE* b_channel = fopen("b_buf.txt", "rb");

    char r_str[200 * 200 * 4];
    fgets(r_str, 200 * 200 * 4, r_channel);
    char* r;
    char* r_rest = r_str;
    
    char g_str[200 * 200 * 4];
    fgets(g_str, 200 * 200 * 4, g_channel);
    char* g;
    char* g_rest = g_str;

    char b_str[200 * 200 * 4];
    fgets(b_str, 200 * 200 * 4, b_channel);
    char* b;
    char* b_rest = b_str;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            r = strtok_r(r_rest, ",", &r_rest);
            g = strtok_r(g_rest, ",", &g_rest);
            b = strtok_r(b_rest, ",", &b_rest);

            unsigned int red = (int) img->lpData[(y * img->width * 3)+(x*3) + 0]; // Red Pixel
            unsigned int green = (int) img->lpData[(y * img->width * 3)+(x*3) + 1]; // Green Pixel
            unsigned int blue = (int) img->lpData[(y * img->width * 3)+(x*3) + 2]; // Blue Pixel
            if (atoi(r) != red
             || atoi(g) != green
             || atoi(b) != blue) {
                printf("NOT EQUAL %d, %d - ORIGINAL: %s, %s, %s\n", y, x, r, g, b);
                printf("FROM LIBJPEG: %d, %d, %d\n", red, green, blue);
             }
        }
    }

    fclose(r_channel);
    fclose(g_channel);
    fclose(b_channel);

    free(img);
    return 0;
}