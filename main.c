#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint8_t r, g, b;
} Pixel;

typedef struct {
    char type[3];
    int heigth;
    int width;
    int max_color;
    Pixel* pixel_array;
} Img;

void img_details(FILE* file) {
    char type[3];
    int heigth, width, max_color;
    fscanf(file, "%s", type);
    fscanf(file, "%d %d", &width, &heigth);
    fscanf(file, "%d", &max_color);

    printf("The type of the image is %s\n The width is %d\n The height is %d\n The max color value is %d\n", type, width, heigth, max_color);
}

Img* image_create_alocate(FILE* stream) {
    Img* img = malloc(sizeof(Img));
    if (img == NULL) {
        return NULL;
    }
    img->type[0] = '\0';
    img->width = 0;
    img->heigth = 0;
    img->max_color = 0;

    fscanf(stream, "%s", img->type);
    fscanf(stream, "%d", &img->width);
    fscanf(stream, "%d", &img->heigth);
    fscanf(stream, "%d", &img->max_color);

    // PULA A PORCARIA DA QUEBRA DE LINHA DPS DO MAX COLOR
    fgetc(stream);

    int img_size = img->width * img->heigth;

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
        return 1;
    }
    fprintf(new_image, "P6\n%d %d\n255\n", img->width, img->heigth);
    int total_pixel = img->width * img->heigth;
    fwrite(img->pixel_array, sizeof(Pixel), total_pixel, new_image);
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("ERROR, LACKS IMG\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        return 1;
    }
    Img* new_image = image_create_alocate(file);
    save_image(new_image);

    return 0;
}
