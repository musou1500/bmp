#ifndef BMP_H

#define BMP_H
#include <stdio.h>

typedef enum {
  BI_MAGIC_BM = 0x424d,
  BI_MAGIC_IC = 0x4943,
  BI_MAGIC_CI = 0x4349,
  BI_MAGIC_PT = 0x5054,
  BI_MAGIC_CP = 0x4350,
} bi_magic_t;

typedef enum {
  BI_RGB,
  BI_RLE8,
  BI_RLE4,
  BI_BITFIELDS,
  BI_JPEG,
  BI_PNG
} bi_compression_t;

typedef struct {
  short magic;
  int size;
  int offset;
} bi_header_t;

typedef struct {
  int size;
  int width;
  int height;
  short planes;
  short bit_count;
  int compression;
  int size_image;
  int x_res;
  int y_res;
  int clr_used;
  int clr_important;
} bi_info_t;

typedef struct {
  unsigned char b;
  unsigned char g;
  unsigned char r;
  char reserved;
} bi_palette_t;

typedef unsigned char bi_data8_t;
typedef struct {
  unsigned char b;
  unsigned char g;
  unsigned char r;
} bi_data24_t;
typedef bi_palette_t bi_data32_t;

typedef struct {
  bi_header_t header;
  bi_info_t info;
  bi_palette_t *palettes;

  union {
    unsigned char *data8;
    bi_data24_t *data24;
    bi_data32_t *data32;
  };
} bi_img_t;

bi_palette_t bi_palette_at(bi_img_t img, int x, int y);
void bi_free_img(bi_img_t img);
bi_img_t bi_read_img(FILE *fp);

#endif /* end of include guard: BMP_H */

