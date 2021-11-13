#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define block_size 512 // dimension of each block of memory
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    // Create input and output file
    char *infile = argv[1];
    // Array to store the name of the image "000.jpg\0"
    char outfile[8];
    int cnt = 0;
    BYTE buffer[block_size];

    FILE *input = fopen(infile, "r");
    if (input == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    sprintf(outfile, "%03i.jpg", cnt);
    FILE *output = fopen(outfile, "w");
    if (output == NULL)
    {
        fclose(input);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // Read and iterate through all the blocks
    while (fread(buffer, sizeof(buffer), 1, input))
    {
        // If we meet the "signature" of a jpeg
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        {
            // If the counter is 0 means it is the very first picture we find
            if (cnt == 0)
            {
                fwrite(buffer, sizeof(buffer), 1, output);
                cnt++;
            }
            else // It means we've found the signature of another image, in that case we close and open a new file with a different name
            {
                fclose(output);
                sprintf(outfile, "%03i.jpg", cnt);
                cnt++;

                output = fopen(outfile, "w");
                if (output == NULL)
                {
                    fclose(input);
                    fprintf(stderr, "Could not create %s.\n", outfile);
                    return 3;
                }
                fwrite(buffer, sizeof(buffer), 1, output);
            }

        }
        // If we haven't ideentified the jpeg's signature and the counter is != 0 it means we are still seeing the data of the picture we're writing and we just keep doing it
        else if (cnt != 0)
        {
            fwrite(buffer, sizeof(buffer), 1, output);
        }
    }
    fclose(input);
    fclose(output);
    return 0;
}
