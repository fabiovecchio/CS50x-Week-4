#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float gs_filter;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            gs_filter = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = gs_filter;
            image[i][j].rgbtGreen = gs_filter;
            image[i][j].rgbtRed = gs_filter;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < round(width / 2); j++)
        {
            tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new_image[height][width];
    // Create a copy of the image
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            new_image[y][x] = image[y][x];
        }
    }
    // Loop through each pixel
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Initialize the counters for each channel
            int red = 0, green = 0, blue = 0;
            float cnt = 0;
            //Checking the edges
            for (int i = - 1; i < 2; i++)
            {
                for (int j = - 1; j < 2; j++)
                {
                    // If the pixel is < 0 or > height/width it means it is out of the image, so we skip it
                    if (y + i < 0 || x + j < 0 || y + i >= height || x + j >= width)
                    {
                        continue;
                    }
                    else // It is inside the pic, so we get its 3 channels values from the copied image
                    {
                        red += new_image[y + i][x + j].rgbtRed;
                        green += new_image[y + i][x + j].rgbtGreen;
                        blue += new_image[y + i][x + j].rgbtBlue;
                        cnt ++;
                    }
                }
            }
            // Once collected the values we calculate the avg value and store it into the original image pixel
            image[y][x].rgbtRed = round(red / cnt);
            image[y][x].rgbtGreen = round(green / cnt);
            image[y][x].rgbtBlue = round(blue / cnt);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new_image[height][width];
    // Create a copy of the image
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            new_image[y][x] = image[y][x];
        }
    }
    // Loop through each pixel
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Initialize the counters
            float red_gx = 0, green_gx = 0, blue_gx = 0;
            float red_gy = 0, green_gy = 0, blue_gy = 0;
            float cnt_red = 0, cnt_green = 0, cnt_blue = 0;
            // Create the gx and gy matrixes
            int gx[3][3] =  {{-1, 0, 1},{-2, 0, 2},{-1, 0, 1}};
            int gy[3][3] =  {{-1, -2, -1},{0, 0, 0},{1, 2, 1}};

            //Checking the edges
            for (int i = - 1; i < 2; i++)
            {
                for (int j = - 1; j < 2; j++)
                {
                    // If the pixel is < 0 or > height/width it means it is out of the image, so we skip it
                    if (y + i < 0 || x + j < 0 || y + i >= height || x + j >= width)
                    {
                        continue;
                    }
                    else // It is inside the pic, so we get its 3 channels values from the copied image and we multiply them by gx and gy matrix
                    {
                        // Calculate the gx matrix
                        red_gx += new_image[y + i][x + j].rgbtRed * gx[i+1][j+1];
                        green_gx += new_image[y + i][x + j].rgbtGreen * gx[i+1][j+1];
                        blue_gx += new_image[y + i][x + j].rgbtBlue * gx[i+1][j+1];
                        // Calculate the gy matrix
                        red_gy += new_image[y + i][x + j].rgbtRed * gy[i+1][j+1];
                        green_gy += new_image[y + i][x + j].rgbtGreen * gy[i+1][j+1];
                        blue_gy += new_image[y + i][x + j].rgbtBlue * gy[i+1][j+1];

                    }
                }
            }
            // Calculate the value for the pixel image
            cnt_red = round(sqrt(red_gx*red_gx + red_gy*red_gy));
            cnt_green = round(sqrt(green_gx*green_gx + green_gy*green_gy));
            cnt_blue = round(sqrt(blue_gx*blue_gx + blue_gy*blue_gy));

            // Once collected the values we calculate the avg value and store it into the original image pixel, if > 255 it caps at 255.
            if (cnt_red > 255)
            {
                image[y][x].rgbtRed = 255;
            }
            else
            {
                image[y][x].rgbtRed = cnt_red;
            }
            if (cnt_green > 255)
            {
                image[y][x].rgbtGreen = 255;
            }
            else
            {
                image[y][x].rgbtGreen = cnt_green;
            }
            if (cnt_blue > 255)
            {
                image[y][x].rgbtBlue = 255;
            }
            else
            {
                image[y][x].rgbtBlue = cnt_blue;
            }

        }
    }
    return;
}
