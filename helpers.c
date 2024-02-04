#include "helpers.h"
#include <math.h>

// Sets offsets of the pixels around and including the target pixel (used for blur and edges)
int offsets[3][3][2] = {
    {{-1, 1}, {0, 1}, {1, -1}},
    {{-1, 0}, {0, 0}, {1, 0}},
    {{-1, -1}, {0, -1}, {1, 1}}
};

// Convert image to grayscale
void grayscale(int height, int width, BMPPixel image[height][width])
{
    // Start to loop over image data
    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            // Apply luminosity equation over the pixels
            int luminosity = 0.21 * image[i][j].Red + 0.72 * image[i][j].Green + 0.07 * image[i][j].Blue;
            image[i][j].Red = luminosity;
            image[i][j].Green = luminosity;
            image[i][j].Blue = luminosity;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, BMPPixel image[height][width])
{   
    // Start loop of image data
    for (int i = 0; i < height; i++) 
    {
        // Start at opposite side of the file and work inward
        for (int j = 0, k = width; j <= width/2, k >= width/2 ; j++, k--) 
        {
            // Create temp pixel
            BMPPixel swap;

            // Swap opposite ends of pixels
            swap = image[i][j];
            image[i][j] = image[i][k];
            image[i][k] = swap;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, BMPPixel image[height][width])
{
    // GX array
    int GX[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1},
    };
    // GY array
    int GY[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1},
    };

    // Making temp file as to not distort the original while processing
    BMPPixel temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    // Start loop through image data
    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            // Initialize gx and gy values for every channel
            int gxRed = 0, gxGreen = 0, gxBlue = 0;
            int gyRed = 0, gyGreen = 0, gyBlue = 0;

            // Start loop through offsets
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    // Adding offsets to current row and column
                    int col = i + offsets[k][l][0]; 
                    int row = j + offsets[k][l][1];

                    // Making sure the offset is withing the image boundary
                    if (col >= 0 && col <= height && row >= 0 && row <= width)
                    {   

                        // Position of index within GX and GY array
                        int gxIndex = GX[k][l];
                        int gyIndex = GY[k][l];

                        // Multiply channels with GX/GY index 
                        gxRed += temp[col][row].Red * gxIndex;
                        gxGreen += temp[col][row].Green * gxIndex;
                        gxBlue += temp[col][row].Blue * gxIndex;

                        gyRed += temp[col][row].Red * gyIndex;
                        gyGreen += temp[col][row].Green * gyIndex;
                        gyBlue += temp[col][row].Blue * gyIndex;
                    }
                }

            }

            // Applying Sobel operation to all channels and applying the result to the original image
            image[i][j].Red = round(sqrt(pow(gxRed, 2) + pow(gyRed, 2)));
            image[i][j].Green = round(sqrt(pow(gxGreen, 2) + pow(gyGreen, 2)));
            image[i][j].Blue = round(sqrt(pow(gxBlue, 2) + pow(gyBlue, 2)));

            // Ensure values are capped at 255
            image[i][j].Red = (image[i][j].Red > 255) ? 255 : image[i][j].Red;
            image[i][j].Green = (image[i][j].Green > 255) ? 255 : image[i][j].Green;
            image[i][j].Blue = (image[i][j].Blue > 255) ? 255 : image[i][j].Blue;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, BMPPixel image[height][width])
{
    // Making temp file as to not distort the original while processing
    BMPPixel temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    // Start main for loop to loop through image data 1 pixel at a time
    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            // Initiate placeholders for pixel sums
            int redSum = 0, greenSum = 0, blueSum = 0, count = 0;

            // Loop through offsets
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++) 
                {
                    // Add offest to current row and column
                    int col = i + offsets[k][l][0];
                    int row = j + offsets[k][l][1];

                    // Making sure pixel position falls in bounds of the image
                    if (col >= 0 && col <= height && row >= 0 && row <= width)
                    {
                        // Incrementing the sums of each RGB value
                        redSum += temp[col][row].Red;
                        greenSum += temp[col][row].Green;
                        blueSum += temp[col][row].Blue;
                        // Counts the accurate total of squares accounted for
                        count++;
                    }
                }
                // Changing target pixel value to average of sum
                image[i][j].Red = round(redSum / count);
                image[i][j].Green = round(greenSum / count);
                image[i][j].Blue = round(blueSum / count);
            }
        }
    }

    return;
}