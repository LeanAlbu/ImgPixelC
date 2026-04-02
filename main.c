#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct{
   uint8_t r,g,b;
}Pixel;

typedef struct{
   int heigth;
   int length;
   Pixel* pixel_array;
}Img;


void img_details(FILE* file){
   char type[3];
   int heigth, width, max_color;
   fscanf(file, "%s", type);
   fscanf(file, "%d %d", &width, &heigth);
   fscanf(file, "%d", &max_color);

   printf("The type of the image is %s\n The width is %d\n The height is %d\n The max color value is %d\n", type, width, heigth, max_color);
}



int main (int argc, char** argv) {

   if(argc < 2){
      printf("ERROR, LACKS IMG\n");
      return 1;
   }


   FILE *file = fopen(argv[1], "r");
   if(file == NULL){
      return 1;
   }
   img_details(file);
   
}

