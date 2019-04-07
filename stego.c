//
// Created by Miguel Alonso Jr on 2019-02-28.
//

#include "stego.h"
#include "bitmap.h"
#include "util.h"

#define BYTES_PER_PIXEL 3
#define MSG_OFFSET 4

// gets last bit of char, effectively writing it backwards
unsigned char _get_bit(char byte, int bit_number) {
    return (unsigned char) ((byte >> bit_number) & 1); // masking against 0...01
}

// stores the byte count of text in first 4 bytes of data portion
void _store_length(long size, unsigned char *data) {
    data[0] = (unsigned char) ((size >> 24) & 0xFF);
    data[1] = (unsigned char) ((size >> 16) & 0xFF);
    data[2] = (unsigned char) ((size >> 8) & 0xFF);
    data[3] = (unsigned char) (size & 0xFF);
}

long _get_length(const unsigned char *data) {
    long length = 0;
    length = (long) (data[3] & 0xFF);
    length = length | (long) ((data[2] & 0xFF) << 8);
    length = length | (long) ((data[1] & 0xFF) << 16);
    length = length | (long) ((data[0] & 0xFF) << 24);

    return length;
}

/*
 *  Sets aside 4 bytes in the data portion to store text file byte count
 *  using "_get_text_length" and "_store_length" respectively
 *  This is the purpose for "index" offset of 4.
 *  
 *  Because of this we lose effectively 4 bits per image to store encoded data
*/
bool encode(char *text_source, char *original_image, char *destination_image) {
    //TODO: add size of text source to encoded image
    char *error_message = NULL;
    char buff;
    int index = MSG_OFFSET; // offset for message bytes
    unsigned char mask = 1;
    FILE *text_file = _open_file(text_source, "r");
    long text_size = _get_text_length(text_file);   // byte count in txtfile
    Bitmap *image = read_bitmap(original_image, &error_message);
    _store_length(text_size, image->data); // stores byte count as message

    // padding byte protection
    long used_row_bytes = image->header.width_px * BYTES_PER_PIXEL;
    int padding_skip = 0;
    if (used_row_bytes % 4 != 0)
    {
        /* 
            calculates offset to next greatest multiple of 4
            effectively giving you number of padding bytes to skip 
        */
        padding_skip = 4 - (used_row_bytes % 4);
    }

    int byte_ctr = MSG_OFFSET;
    do {
        buff = (char) fgetc(text_file);
        for (int i = 0; i < 8; i++) { // iterating over one byte (8 bits)
            if (byte_ctr < used_row_bytes)
            {
                // (zeroes out last bmpdata bit) | (retrieves next textfile bit)
                image->data[index] = (image->data[index] & ~mask) | (_get_bit(buff, i) & mask);
                index++;
                byte_ctr++;
            }
            else
            {
                index += padding_skip;
                byte_ctr = 0;
                i--;    // rewinds i so as to not skip encoding ith bit
            }
        }
    } while (!feof(text_file));

    write_bitmap(destination_image, image, &error_message);
    bool is_valid = check_bitmap_header(image->header, "sw_poster_copy.bmp");
    return is_valid;
}

bool decode(char *image_source, char *text_destination) {
    char *error_message = NULL;
    unsigned char buff = 0;
    unsigned char mask = 1;
    long message_length;
    Bitmap *image = read_bitmap(image_source, &error_message);
    FILE *text_file = _open_file(text_destination, "w");
    message_length = _get_length(image->data); // gets byte count in txtfile

    // padding byte protection
    long used_row_bytes = image->header.width_px * BYTES_PER_PIXEL;
    int padding_skip = 0;
    if (used_row_bytes % 4 != 0)
    {
        padding_skip = 4 - (used_row_bytes % 4);
    }

    /*
     * Setting i and index to u. long to futureproof against large data
     * addresses from failing to write out
    */
    unsigned long i = 0;
    unsigned long index = 0;
    int byte_ctr = MSG_OFFSET;
    for (index = MSG_OFFSET; index < message_length * 8; index++) {
        if (byte_ctr < used_row_bytes)
        {
            //shifts and writes encoded byte to buffer
            buff = buff | ((image->data[index] & mask) << (i % 8));

            if ((i % 8) > 6) { //if we've shifted by 7
                //write to textfile, reset buffer
                fputc(buff, text_file); 
                buff = 0;
            }
            byte_ctr++;
            i++;
        }
        else
        {
            index += padding_skip - 1; //-1 to account for loop increment
            byte_ctr = 0;
        }
    }

    bool is_valid = true;
    return is_valid;
}

//TODO: decode