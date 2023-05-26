#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int block = 512;

typedef uint8_t BYTE;


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw IMAGE\n");
        return 1;
    }

    FILE *crd = fopen(argv[1], "r");

    if (crd == NULL)
    {
        printf("Error, couldnt open file!!!\n" );
        return 1;
    }
    fseek(crd, 0, SEEK_END);
    int end = ftell(crd);
    fseek(crd, 0, SEEK_SET);

    int count = 0;
    char buffer1[8] = {0};
    BYTE buffer[block];

    // loop to search file for pictures
    while(fread(&buffer, block, 1, crd))  // loop till end of file
    {

        if (buffer[0] == 0xff & buffer[1] == 0xd8 & buffer[2] == 0xff & (buffer[3] == 0xe0 | buffer[3] == 0xe1 | buffer[3] == 0xe2 | buffer[3] == 0xe4 | buffer[3] == 0xe5 | buffer[3] == 0xe6 | buffer[3] == 0xe7 | buffer[3] == 0xe8 | buffer[3] == 0xe9 | buffer[3] == 0xea | buffer[3] == 0xeb | buffer[3] == 0xec | buffer[3] == 0xed | buffer[3] == 0xee | buffer[3] == 0xef))
        {

            sprintf(buffer1, "%03d.jpg", count++);
            FILE *picture = fopen(buffer1, "w");


                if (picture == NULL)
                {
                    printf("picture failed");
                    return 1;
                 }
                 fwrite(&buffer, block, 1, picture);
                 fread(&buffer, block, 1, crd);

            while(buffer[0] != 0xff & buffer[1] != 0xd8 & buffer[2] != 0xff & (buffer[3] != 0xe0 | buffer[3] != 0xe1 | buffer[3] != 0xe2 | buffer[3] != 0xe4 | buffer[3] != 0xe5 | buffer[3] != 0xe6 | buffer[3] != 0xe7 | buffer[3] != 0xe8 | buffer[3] != 0xe9 | buffer[3] != 0xea | buffer[3] != 0xeb |  buffer[3] != 0xec | buffer[3] != 0xed | buffer[3] != 0xee | buffer[3] != 0xef))
            {
                fwrite(&buffer, block, 1, picture);
                fread(&buffer, block, 1, crd);
                
                if (ftell(crd) == end)
                {
                    fclose(picture);
                    fclose(crd);
                    return 0;
                }
            }

            fclose(picture);

            if (count != 50)
            {
                fseek(crd, -512, SEEK_CUR);
            }

        }
    }
    fclose(crd);
    }

