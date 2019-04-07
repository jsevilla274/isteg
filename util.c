//
// Created by Miguel Alonso Jr on 2019-02-28.
//

#include <stdlib.h>
#include "util.h"

FILE *_open_file(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        fprintf(stderr, "Could not open file %s", filename);

        exit(EXIT_FAILURE);
    }

    return fp;
}

void _handle_error(char **error, FILE *fp, Bitmap *image) {
    fprintf(stderr, "ERROR: %s\n", *error);
    _clean_up(fp, image, error);

    exit(EXIT_FAILURE);
}

void _clean_up(FILE *fp, Bitmap *image, char **error) {
    if (fp != NULL) {
        fclose(fp);
    }
    _free_bmp(image);
    free(*error);
}

void _free_bmp(Bitmap *image) {
    if (image) {
        free(image->data);
        free(image);
    }
}

/*
 * Bizzare fseek behavior causes ftell to sometimes count before 
 * terminating newline in return value (depending on filesize?)
 * 
 * For this reason we add + 1 and assume newline exists
*/
long _get_text_length(FILE *filename) {
    fseek(filename, 0L, SEEK_END);
    long size = ftell(filename) + 1; 
    fseek(filename, 0L, SEEK_SET);
    return (size);
}