//
// Created by Miguel Alonso Jr on 2019-02-28.
//
#include <stdio.h>
#include "bitmap.h"

#ifndef FILE_UTIL_H
#define FILE_UTIL_H

FILE *_open_file(const char *filename, const char *mode);
void _handle_error(char **error, FILE *fp, Bitmap *image);
void _clean_up(FILE *fp, Bitmap *image, char **error);
void _free_bmp(Bitmap *image);
long _get_text_length(FILE *filename);

#endif //FILE_UTIL_H
