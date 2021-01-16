#include <stdlib.h>
#include <stdio.h>
#include "img_rotation.h"
#include "bmp_io.h"

struct image rotate( struct image const source ){
    struct image img_rotate = {.width = source.height,.height = source.width};
    img_rotate.data = (struct pixel *) malloc(source.height * source.width * sizeof(struct pixel));
    for (size_t row = 0; row < source.height; row++) {
        for (size_t col = 0; col < source.width; col++) {
            img_rotate.data[row+((img_rotate.height-col-1) * img_rotate.width)] = source.data[col+row * source.width];
        }
    }
    return img_rotate;
}