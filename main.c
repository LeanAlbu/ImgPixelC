#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


typedef struct {
    uint8_t r, g, b;
} Pixel;

typedef struct {
    char type[3];
    int height;
    int width;
    int max_color;
    Pixel* pixel_array;
} Img;


void img_details(FILE* file);
Img* image_create_allocate(FILE* stream);
int save_image(Img* img);
Img* image_pixelize(Img* img);


void img_details(FILE* file) {
    char type[3];
    int height, width, max_color;
    fscanf(file, "%s", type);
    fscanf(file, "%d %d", &width, &height);
    fscanf(file, "%d", &max_color);

    printf("The type of the image is %s\n", type);
    printf("The width is %d\n", width);
    printf("The height is %d\n", height);
    printf("The max color value is %d\n", max_color);
}

Img* image_create_allocate(FILE* stream) {
    Img* img = malloc(sizeof(Img));
    if (img == NULL) {
        return NULL;
    }
    
    img->type[0] = '\0';
    img->width = 0;
    img->height = 0;
    img->max_color = 0;

    fscanf(stream, "%s", img->type);
    fscanf(stream, "%d", &img->width);
    fscanf(stream, "%d", &img->height);
    fscanf(stream, "%d", &img->max_color);

    // PULA A PORCARIA DA QUEBRA DE LINHA DPS DO MAX COLOR
    fgetc(stream);

    int img_size = img->width * img->height;

    img->pixel_array = malloc(img_size * sizeof(Pixel));
    if (img->pixel_array == NULL) {
        free(img);
        return NULL;
    }
    
    fread(img->pixel_array, sizeof(Pixel), img_size, stream);

    return img;
}

int save_image(Img* img) {
    FILE* new_image = fopen("pixelated_image.ppm", "wb");
    if (new_image == NULL) {
        return 1; // Falha
    }
    
    // Escreve o cabeçalho
    fprintf(new_image, "P6\n%d %d\n%d\n", img->width, img->height, img->max_color);
    
    // Escreve os pixels
    int total_pixel = img->width * img->height;
    fwrite(img->pixel_array, sizeof(Pixel), total_pixel, new_image);
    
    fclose(new_image);
    return 0; // Sucesso
}

Img* image_pixelize(Img* img) {
    Img* new_image = malloc(sizeof(Img));
    if(new_image == NULL){
        return NULL;
    }

    int block_size = img->width / 160;
    if (block_size < 1) block_size = 1;

    strcpy(new_image->type, img->type);
    new_image->width = img->width;
    new_image->height = img->height;
    new_image->max_color = img->max_color;

    int image_size = new_image->width * new_image->height;
    new_image->pixel_array = malloc(image_size * sizeof(Pixel));
    if(new_image->pixel_array == NULL){
        free(new_image);
        return NULL;
    }

    for(int y = 0; y < img->height; y += block_size){
        for(int x = 0; x < img->width; x += block_size){
            
            long r_total = 0, g_total = 0, b_total = 0;
            int cont_pixels = 0;

            int lim_y = (y + block_size > img->height) ? img->height : y + block_size;
            int lim_x = (x + block_size > img->width) ? img->width : x + block_size;

            for(int i = y; i < lim_y; i++){
                for(int j = x; j < lim_x; j++){
                    int idx = i * img->width + j;
                    r_total += img->pixel_array[idx].r;
                    g_total += img->pixel_array[idx].g;
                    b_total += img->pixel_array[idx].b;
                    cont_pixels++;
                }
            }

            uint8_t r_med = r_total / cont_pixels;
            uint8_t g_med = g_total / cont_pixels;
            uint8_t b_med = b_total / cont_pixels;

            for(int i = y; i < lim_y; i++){
                for(int j = x; j < lim_x; j++){
                    int idx = i * new_image->width + j;
                    new_image->pixel_array[idx].r = r_med;
                    new_image->pixel_array[idx].g = g_med;
                    new_image->pixel_array[idx].b = b_med;
                }
            }
        }
    }
    return new_image;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("ERROR, LACKS IMG\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "rb"); 
    if (file == NULL) {
        printf("ERROR OPENING FILE\n");
        return 1;
    }

    Img* original_image = image_create_allocate(file);
    fclose(file); 

    if (original_image == NULL) {
        printf("ERROR ALLOCATING IMAGE\n");
        return 1;
    }

    Img* pixelized_image = image_pixelize(original_image);
    
    if (pixelized_image != NULL) {
        save_image(pixelized_image);
        
        free(pixelized_image->pixel_array);
        free(pixelized_image);
    }

    free(original_image->pixel_array);
    free(original_image);

    printf("Success! Image processed and saved.\n");
    return 0;
}
