#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Comments of the PPM file stored in the form of a linked list*/
struct Comment {

    char *value;
    struct Comment *nCmnt;

};

/* The RGB values of a pixel. */
struct Pixel {

    int red;
    int green;
    int blue;

};

/* An image loaded from a PPM file. */
struct PPM {
    
    // The magic number of the PPM file
    char magicNumber[3];
    // The comments of the PPM file
    struct Comment *cmnt;
    int width;
    int height;
    int max;
    // The pixels of the image
    struct Pixel *pixar;

};

/* Reads an image from an open PPM file.
 * Returns a new struct PPM, or NULL if the image cannot be read. */
struct PPM *getPPM(FILE * f)
{ 
    // Allocate memory for the PPM struct
    struct PPM *ppmImg = (struct PPM *)malloc(sizeof(struct PPM));

    // Read the magic number
    fscanf(f, "%s", ppmImg->magicNumber);
    // Check if the magic number is valid
    if (strcmp(ppmImg->magicNumber,"P3") != 0) 
    { 
        printf("Image is invalid\n");
        return NULL;
    }

    char character = getc(f);
    // Read the comments
    ppmImg->cmnt = (struct Comment*)malloc(sizeof(struct Comment));
    struct Comment *temp = ppmImg->cmnt; 
    while ((character = getc(f)) == '#') { 
        fseek(f, -1, SEEK_CUR); 
        char * string =(char*) malloc(255 * sizeof(char));
        fgets(string, 255, f);
        temp->value = (char*)malloc(strnlen(string, 255));
        strcpy(temp->value, string);
        temp->nCmnt = (struct Comment*)malloc(sizeof(struct Comment));
        temp = temp->nCmnt;
        temp->nCmnt = NULL;
    }

    // Read the width, height and max value
    fseek(f, -1, SEEK_CUR); 
    fscanf(f, "%d", &ppmImg->width);
    fscanf(f, "%d", &ppmImg->height);
    fscanf(f, "%d", &ppmImg->max);
    // Check if the max value is valid
    ppmImg->pixar = (struct Pixel *)malloc(ppmImg->height * ppmImg->width * sizeof(struct Pixel));

    int a = 0;
    int b = 0;
    // Read the pixels
    while (b < ppmImg->height * ppmImg->width) 
    {
        // Read the red, green and blue values
        a = fscanf(f, "%d", &ppmImg->pixar[b].red);
        a = fscanf(f, "%d", &ppmImg->pixar[b].green);
        a = fscanf(f, "%d", &ppmImg->pixar[b].blue);
        b = b + 1;

    }
   
    return ppmImg;
}

/* Writes img to stdout in PPM format. */
void showPPM(const struct PPM *img)
{
    // Print the magic number
    printf("%s\n", img->magicNumber);
    // Print the comments
    struct Comment *com = img->cmnt;
    // Print the width, height and max value
    while (com->nCmnt != NULL)
    {
        printf("%s\n", com->value);
        com = com->nCmnt;
    }
    // Print the pixels
    printf("%d %d\n%d\n", img->width, img->height, img->max);
    
    int x;
    // Print the red, green and blue values
    for (x = 0; x < img->height * img->width; ++x) 
    {
        printf("%d %d %d\n", img->pixar[x].red, img->pixar[x].green, img->pixar[x].blue);
    }

}

/* Opens and reads a PPM file, returning a pointer to a new struct PPM.
 * On error, prints an error message and returns NULL. */
struct PPM *readPPM(const char *filename)
{
    // Open the file
    FILE *f = fopen(filename, "r");
    // Check if the file was opened successfully
    if (f == NULL) {
        fprintf(stderr, "File %s could not be opened.\n", filename);
        return NULL;
    }

    // Read the image
    struct PPM *img = getPPM(f);

    // Close the file
    fclose(f);

    // Check if the image was read successfully
    if (img == NULL) {
        fprintf(stderr, "File %s could not be read.\n", filename);
        return NULL;
    }

    return img;
}

/*Copies the comments of the old PPM structure and returns the comments*/
struct Comment *copyComment(struct Comment *first) 
{
    struct Comment *scnd = NULL; 
    struct Comment *old = NULL;

    // Copy the comments
    while (first != NULL) 
    {
        // Allocate memory for the comment
        struct Comment *temp = (struct Comment*) malloc(sizeof(struct Comment));
        temp->value = first->value;
        temp->nCmnt = NULL;
    // Check if the comment is the first comment
        if (scnd == NULL) 
        {
            scnd = temp;
            old = temp;
        } 
        // Check if the comment is the last comment
        else 
        {
            old->nCmnt = temp;
            old = temp;
        }
        // Move to the next comment
        first = first->nCmnt;

        }

return scnd;

}

/*Copies the contents of old PPM structure and returns a new structure*/
struct PPM * copyPPM(struct PPM *old) 
{
    // Allocate memory for the PPM struct
    struct PPM *n = (struct PPM*) malloc(sizeof(struct PPM));
    // Copy the magic number
    strcpy(n->magicNumber, old->magicNumber); 
    // Copy the comments
    n->cmnt = copyComment(old->cmnt);
    n->height = old->height;
    n->width = old->width;
    n->max = old->max;
    // Copy the pixels
    n->pixar = (struct Pixel *) malloc(old->height * old->width * sizeof(struct Pixel));
    // Copy the red, green and blue values
    memcpy(n->pixar, old->pixar, old->height * old->width * sizeof(struct Pixel));
    
    return n;
}

/* Encode the string text into the red channel of image img.
 * Returns a new struct PPM, or NULL on error. */
struct PPM *encode(const char *text,struct PPM *img)
{
    struct PPM *encimg = copyPPM(img);
    // Check if the image was copied successfully
    int a = (img->height * img->width) / (strlen(text)+1);
    // Check if the image is big enough to encode the message

    int x = 0;
    int y = rand() % a;

        // Encode the text
        while (x < strlen(text)) 
        {   // Check if the red value is the same as the ASCII value of the letter
            if(encimg->pixar[y].red == text[x]) 
            {
                y++; 
            }
            // Check if the red value is not the same as the ASCII value of the letter
            else
            {
                encimg->pixar[y].red = text[x]; 
                x++;
                y = y + (rand() % a);
            }
        }    
        // Check if the image was encoded successfully
        if (encimg == NULL) 
        {
            return NULL;
        }
        // Check if the image was not encoded successfully 
        else 
        {
            fprintf(stderr, "Message has been encoded successfully.\n "); 
            // Print the comments
            struct Comment *a = encimg->cmnt;
            // Print the width, height and max value
            while (a->nCmnt != NULL)
            {
                fprintf(stderr,"%s\n", a->value);
                a = a->nCmnt;
            }
            return encimg;
        }

}

/* Extract the string encoded in the red channel of newimg, by comparing it
 * with oldimg. The two images must have the same size.
 * Returns a new C string, or NULL on error. */
char *decode(const struct PPM *oldimg, const struct PPM *newimg)
{
    
    int a= 0;
    int b = 0;
    // Check if the images are the same size
    if(oldimg->height*oldimg->width == newimg->height*newimg->width){ 
        char *string = (char*)malloc(oldimg->height*oldimg->width*sizeof(char));
        // Decode the text
        while (a < oldimg->height * oldimg->width) 
        {      // Check if the red values are the same
            if(oldimg->pixar[a].red != newimg->pixar[a].red) /*And if the red values are not the same we add the red value of the new image to str*/
            {
                string[b] = newimg->pixar[a].red;
                b = b+1;
            }

            a = a + 1;
        
        }

        return string; /*Resulting with the decoded message*/
    
    }
    else
    {
        return NULL;
    }
}


int main(int argc, char *argv[])
{
    // Seed the random number generator
    srand(time(NULL));

    // Check the number of arguments
    if (argc == 3 && strcmp(argv[1], "t") == 0) {
        // Mode "t" - test PPM

        struct PPM *image = readPPM(argv[2]); 
        showPPM(image);

    } else if (argc == 3 && strcmp(argv[1], "e") == 0) {
        // Mode "e" - encode PPM
        fprintf(stderr, "Enter a message to encode: ");
        // Read the PPM file
        struct PPM *oldimg = readPPM(argv[2]); 
        // Read the message
        char * encoding =(char*) malloc(255 * sizeof(char));
        // Encode the message
        fgets (encoding,255, stdin); 
        // Print the encoded message
        struct PPM *newimg = encode(encoding, oldimg); 
        showPPM(newimg);

    } else if (argc == 4 && strcmp(argv[1], "d") == 0) {
        // Mode "d" - decode PPM

        // Read the PPM files
        struct PPM *oldimg = readPPM(argv[2]); 
        // Decode the message
        struct PPM *newimg = readPPM(argv[3]); 
        char * message = (char *)malloc(255);
        // Print the decoded message
        message = decode(oldimg,newimg); 

        // Check if the message was decoded successfully
        if (message != NULL) {

            printf("Message decoded successfully: "); 
            printf("%s", message); /*Encoded message is printed*/

        }
        // Check if the message was not decoded successfully
        else
        {
            fprintf(stderr, "Unable to decode image\n");
            return 1;
        }

    } // Check if the command line is incomplete
    else {
        fprintf(stderr, "Unrecognised or incomplete command line.\n");
        return 1;
    }

    return 0;
}
