#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
// Import definitions
#include "helpers.h"

void printImageValues(int height, int width, BMPPixel image[height][width]);

int main(int argc, char *argv[])
{   
    // Set filter options
    char *filters = "begr";

    // Process command line options
    char filter = getopt(argc, argv, filters);

    // Make sure only 1 flag was provided
    if (getopt(argc, argv, filters) != -1) 
    {
        fprintf(stderr, "Only 1 filter allowed\n");
        exit(EXIT_FAILURE);
    }

    // Make sure only 3 arguments exist
    if (argc != optind + 2) 
    {
        fprintf(stderr, "Usage: main [flag] inFile.bmp outFile.bmp");
        exit(EXIT_FAILURE);
    }

    // Get both filenames in arguments
    char *infile = argv[optind];
    char *outfile = argv[optind + 1];

    // Open input file
    FILE *inputFile = fopen(infile, "rb");
    if (infile == NULL) 
    {
        fprintf(stderr, "Could not open %s\n", infile);
        exit(EXIT_FAILURE);
    }

    // Create output file
    FILE *outputFile = fopen(outfile, "wb");
    if (outputFile == NULL) 
    {
        fprintf(stderr, "Could not create %s\n", outfile);
        fclose(inputFile);
        exit(EXIT_FAILURE);
    }

    // Read BMP Header from in
    BMPHeader header;
    fread(&header, sizeof(BMPHeader), 1, inputFile);

    // Read BMP Info Header from in
    BMPInfoHeader infoHeader;
    fread(&infoHeader, sizeof(BMPInfoHeader), 1, inputFile);

    // Check to see if type = "BM" and that it includs true image pixels 
    if ((header.signature[0] != 'B' && header.signature[1] != 'M') || infoHeader.bitsPerPixel != 24)
    {
        fprintf(stderr, "File is not of the correct format [.bmp]\n");
        fclose(inputFile);
        fclose(outputFile);
        exit(EXIT_FAILURE);
    }

    int width = infoHeader.width;
    int height = abs(infoHeader.height);

    BMPPixel (*image)[width] = calloc(height, width * sizeof(BMPPixel));
    // printf("Width: %i, height %i\n", infoHeader.width, abs(infoHeader.height));

    if (!image) 
    {
        fprintf(stderr, "Could not allocate memeory for image\n");
        fclose(inputFile);
        fclose(outputFile);
        exit(EXIT_FAILURE);
    }

    // Calculate padding for scanlines
    int padding = (4 - (width * sizeof(BMPPixel)) % 4) % 4;

    for (int i = 0; i < height; i++) 
    {
        // Read row into image array
        fread(image[i], sizeof(BMPPixel), width, inputFile);
        // Skip over padding
        fseek(inputFile, padding, SEEK_CUR);
    }

    // printImageValues(height, width, image);

    switch (filter)
    {
        case 'b': 
            blur(height, width, image);
            break;
        case 'e': 
            edges(height, width, image);
            break;
        case 'g':
            grayscale(height, width, image);
            break;
        case 'r':
            reflect(height, width, image);
            break;
    }

    // Write header to out file
    fwrite(&header, sizeof(BMPHeader), 1, outputFile);
    // Write info header to out file
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, outputFile);

    // Write image data to out file
    for (int i = 0; i < height; i++)
    {
        // Write image row to the out file
        fwrite(&image[i], sizeof(BMPPixel), width, outputFile);

        // Write padding at end of row
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outputFile);
        }
    }

    // Free image allocation
    free(image);

    // Close files
    fclose(inputFile);
    fclose(outputFile);

    exit(EXIT_SUCCESS);
}

void printImageValues(int height, int width, BMPPixel image[height][width]) 
{
    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            printf("Red: %i, Green: %i, Blue: %i\n", image[i][j].Red, image[i][j].Green, image[i][j].Blue);
        }
    }
}