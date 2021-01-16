#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "bmp_io.h"
#include "bmp_struct.h"

struct image * malloc_bmp(){
   return (struct image *) malloc(sizeof(struct image));
}
void free_bmp(struct image* img){
    free(img->data);
    free(img);
}

struct bmp_header _generate_header (struct image const *img) {

    struct bmp_header header = {
    .bfType = 0x4D42,
    .bfileSize = img->width * img->height * sizeof(struct pixel) + img->height * (img->width % 4) + sizeof(struct bmp_header),
    .bfReserved = 0,
    .bOffBits = sizeof(struct bmp_header),

    .biSize = 40,
    .biWidth = img->width,
    .biHeight = img->height,
    .biPlanes = 1,
    .biBitCount = 24,
    .biCompression = 0,
    .biSizeImage = header.bfileSize - header.bOffBits,
    .biXPelsPerMeter = 0,
    .biYPelsPerMeter = 0,
    .biClrUsed = 0,
    .biClrImportant = 0};
    return header;
}

enum read_status _check_header(const struct bmp_header header){
if (header.bfType != 0x4D42){ return READ_INVALID_SIGNATURE; }
    if (header.biBitCount != 24){ return READ_INVALID_BITS; }
    if (header.biSize !=40 
    || header.biCompression!=0
    || header.bfileSize != header.bOffBits + header.biSizeImage){
       return READ_INVALID_HEADER;
    }
return 0;
}

enum read_status from_bmp( FILE* in, struct image* img){
    if (in == NULL)  return READ_INVALID_PATH; 
    struct bmp_header header ;
    fread(&header, 1, sizeof(struct bmp_header), in);
    enum read_status header_status = _check_header(header);
    if (header_status) return header_status;

    uint64_t data_size = header.biHeight * header.biWidth * sizeof(struct pixel);
    img->data = (struct pixel *) malloc(data_size);
    img->height = header.biHeight;
    img->width = header.biWidth;
    uint16_t padding = header.biWidth % 4;

    for (size_t i = 0; i < header.biHeight; i++) {
            fread(&(img->data[i * img->width]), sizeof(struct pixel), img->width, in);
            fseek(in, padding, SEEK_CUR);
    }

    return READ_OK;
}

enum write_status to_bmp( FILE* out, struct image const* img ){   
    struct bmp_header header = _generate_header(img);
    uint64_t padding = img->width % 4;

    if(!fwrite(&header, 1, sizeof(struct bmp_header), out)){
        return WRITE_ERROR;
    }
    const uint8_t nulls = 0;
    for (size_t i = 0; i < img->height; i++) {
        if(!fwrite(&img->data[i * img->width], sizeof(struct pixel), img->width, out) ||
        !fwrite(&nulls, 1, padding, out)) {return WRITE_ERROR;}
    }
    return WRITE_OK;
}

char* const read_status_string[] = {
        [READ_OK]                     = "Image form file is loaded\n",
        [READ_INVALID_SIGNATURE]      = "Invalid Signature\n",
        [READ_INVALID_BITS]           = "Only 24-bit file supported\n",
        [READ_INVALID_HEADER]         = "Invalid header\n",
        [READ_INVALID_PATH]           = "File path not found\n"
};


enum read_status print_read_status(enum read_status status){
    printf(read_status_string[status]);
    return status;
}

char* const write_status_string[] = {
        [WRITE_OK]      = "Image is saved in file\n",
        [WRITE_ERROR]   = "File write error\n"
};

enum read_status print_write_status(enum write_status status){
    printf(write_status_string[status]);
    return status;
}