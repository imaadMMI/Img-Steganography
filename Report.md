# Who I am

- Student ID: <H00362645>

# Program design

A stack structure holds the content of the PPM file and also contains an array which holds the RGB values and a linked list

The program has got several functions including getPPM, readPPM, showPPM, copyPPM, copyComment, encode, decode, and main function.

The code also contains test, encode and decode modes.

# Choice of data structures and algorithms

A linked list data structure is used to store the comments. A dynamic array is also used to store the RGB values of PPM file. 

In getPPM function, iteration is done to retrieve the data of the PPM file and store it in the structure. Various functions are used including fscanf() and getc(). After this, the PPM structure is returned 

In showPPM function, the contents of the PPM structure is printed on the terminal

In readPPM function, a file is opened and passed into getPPM 

In copyPPM function, the PPM structure is copied and the copyComment function is used to copy PPM structure to new one 


In the encode function, a new PPM structure is used. Through random number the red values are replaced into the ppm file. The ascii values of the text are replaced randomly. If the encoding is successful then the PPM file is printed to the terminal otherwise null value is returned 


In the decode function, two PPM files are checked to make sure they are of the same size. And the images are compared by iterating through the pixels. If the red value is different then the red pixel of the new image is stored in the array and the message is decoded 


In the main function, if argv[1] == "t" and the number of arguments is 3 then the test mode is the PPM file is printed to the terminal. If argv[1] == "e" and the number of argumentd is 3 a PPM file is taken and encoded. If argv[1] == "d" and the number of arguments is 4, the  message is decoded
