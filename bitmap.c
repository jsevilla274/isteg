//
// Created by Miguel Alonso Jr on 2019-02-28.
//
#include <stdlib.h>
#include "bitmap.h"
#include "util.h"

Bitmap *read_bitmap(char *filename, char **error) {
    // open the file for reading as binary
    FILE *file = _open_file(filename, "rb");
    Bitmap *image = malloc(sizeof(*image));
    //TODO: Check that memory was allocated properly
    rewind(file);

    // Read bitmap header
    int num_read = (int) fread(&image->header, sizeof(image->header), 1, file);
    //TODO: check that header was read properly

    //TODO: check that read header matches file

    //Allocate memory for the bitmap data
    //sizeof uses struct definitions to make appropriate sized allocation
    image->data = malloc(sizeof(*image->data) * image->header.image_size_bytes);
    //TODO: check that memory was allocated properly

    //Read bitmap image data
    num_read = (int) fread(image->data, image->header.image_size_bytes, 1, file);
    //TODO: check that bitmap image data was read properly

    return image;
}

bool write_bitmap(const char *filename, Bitmap *image, char **error) {
    // open the file for writing as binary
    FILE *file = _open_file(filename, "wb");
    rewind(file);

    // write the bitmap header
    int num_read = (int) fwrite(&image->header, sizeof(image->header), 1, file);
    // TODO: check that header was written properly

    // write the bitmap image data
    num_read = (int) fwrite(image->data, image->header.image_size_bytes, 1, file);
    //TODO: check that image data was written properly

    return true;

}

bool check_bitmap_header(BitmapHeader header, char *filename) {
    return true;
}
