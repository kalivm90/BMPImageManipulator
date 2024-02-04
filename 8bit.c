#include <stdio.h>
#include <stdlib.h>
// Custom Imports 
#include "bmp.h"

void readBMP(char *inputFilename, char *outputFilename) {
    FILE *inputFile = fopen(inputFilename, "rb");
    if (!inputFile) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    FILE *outputFile = fopen(outputFilename, "wb");
    if (!outputFile) {
        perror("Error opening output file");
        fclose(inputFile);
        exit(EXIT_FAILURE);
    }

    BMPHeader bmpHeader;
    fread(&bmpHeader, sizeof(BMPHeader), 1, inputFile);
    fwrite(&bmpHeader, sizeof(BMPHeader), 1, outputFile);

    if (bmpHeader.signature[0] == 'B' && bmpHeader.signature[1] == 'M') {
        fprintf(stderr, "Not a BMP file\n");
        fclose(inputFile);
        fclose(outputFile);
        exit(EXIT_FAILURE);
    }

    printf("BMP Header:\n");
    printf("Type: %s\n", bmpHeader.signature); // BM
    printf("Size: %u\n", bmpHeader.size); // 1094
    printf("Reserved1: %u\n", bmpHeader.reserved1); // 0
    printf("Reserved2: %u\n", bmpHeader.reserved2); // 0
    printf("Offset: %u\n", bmpHeader.offset); // 1078

    BMPInfoHeader bmpInfoHeader;
    fread(&bmpInfoHeader, sizeof(BMPInfoHeader), 1, inputFile);
    fwrite(&bmpInfoHeader, sizeof(BMPInfoHeader), 1, outputFile);

    printf("\nBMP Info Header:\n");
    printf("Size: %u\n", bmpInfoHeader.size);  // 40
    printf("Width: %d\n", bmpInfoHeader.width); // 8
    printf("Height: %d\n", bmpInfoHeader.height); // 2
    printf("Planes: %u\n", bmpInfoHeader.planes); // 1
    printf("Bits Per Pixel: %u\n", bmpInfoHeader.bitsPerPixel); // 8
    printf("Compression: %u\n", bmpInfoHeader.compression); // 0
    printf("Image Size: %u\n", bmpInfoHeader.imageSize); // 16
    printf("X Pixels Per Meter: %d\n", bmpInfoHeader.xPixelsPerMeter); // 0
    printf("Y Pixels Per Meter: %d\n", bmpInfoHeader.yPixelsPerMeter); // 0
    printf("Colors Used: %u\n", bmpInfoHeader.colorsUsed); // 256
    printf("Colors Important: %u\n", bmpInfoHeader.colorsImportant); // 256

    if (bmpInfoHeader.bitsPerPixel <= 8) {
        // Bitwise opertation that calculates 2 ^ colorsUsed
        int numColors = bmpInfoHeader.colorsUsed == 0 ? 1 << bmpInfoHeader.bitsPerPixel : bmpInfoHeader.colorsUsed;
        // ColorTable array to store all possible color indexs
        BMPColorTable colorTable[numColors];

        // Read colorTable from input file
        fread(colorTable, sizeof(BMPColorTable), numColors, inputFile);
        // Wrtie colorTable to output file
        fwrite(colorTable, sizeof(BMPColorTable), numColors, outputFile);

        // Prints all colors in colorTable
        printf("\nColor Table:\n");
        for (int i = 0; i < numColors; i++) {
            printf("Color %d: (%u, %u, %u) \n", i+1, colorTable[i].Red, colorTable[i].Green, colorTable[i].Blue);
        }
    }

    // Seek where the image data occurs (may not be necessary since it should already be at the image data)
    fseek(inputFile, bmpHeader.offset, SEEK_SET);

    // Read and write the pixel data
    unsigned char *pixelData = (unsigned char *)malloc(bmpInfoHeader.imageSize);
    fread(pixelData, sizeof(unsigned char), bmpInfoHeader.imageSize, inputFile);
    fwrite(pixelData, sizeof(unsigned char), bmpInfoHeader.imageSize, outputFile);

    // Clean up
    free(pixelData);
    fclose(inputFile);
    fclose(outputFile);
}   

int main() {
    char inputFilename[] = "all_gray.bmp";
    char outputFilename[] = "output.bmp";

    readBMP(inputFilename, outputFilename);

    return 0;
}
