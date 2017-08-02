#include <cs50.h>       
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./copy foresinc img\n");
        return 1;
    }
    
    // open input file 
    FILE *imgs = fopen(argv[1], "r");
    if (imgs == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    
    unsigned char buffer[512];
    int track = 0;
    int found = 0;
    FILE* img;
    
    // iterate over file contents until end of file
    do
    {
        // search for jpeg signature
        if (buffer[0] == 0xff && 
            buffer[1] == 0xd8 &&  
            buffer[2] == 0xff && 
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (found == 1)
            {
                // if a jpeg is already found, close previous img
                fclose(img);
            }
            else
            {
                // first jpeg was found
                found = 1;
            }
            
            char filename[8];
            sprintf(filename, "%03d.jpg", track);
            img = fopen(filename, "a");
            track++;
        }
        
        if (found == 1)
        {
            // keep writing 512 bytes until next img is found
            fwrite(&buffer, sizeof(buffer), 1, img);
        }
        
    }
    while (fread(buffer, sizeof(buffer), 1, imgs));

    //close files
    fclose(imgs);
    
    fclose(img);

    //success 
    return 0;
}