//
// Created by Miguel Alonso Jr on 2019-02-28.
//

#ifndef STEGO_H
#define STEGO_H

#include <stdbool.h>

void _store_length(long size, unsigned char *data);
long _get_length(const unsigned char *data);
bool encode(char *text_source, char *original_image, char *destination_image);
bool decode(char *image_source, char *text_destination);

#endif //STEGO_H
