#include "bmp.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bi_header_t read_header(FILE *fp) {
  bi_header_t h;
  fread(&h.magic, sizeof(short), 1, fp);
  fread(&h.size, sizeof(int), 1, fp);
  // skip reserved
  fseek(fp, sizeof(short) * 2, SEEK_CUR);
  fread(&h.offset, sizeof(int), 1, fp);
  return h;
}

static bi_info_t read_info(FILE *fp) {
  bi_info_t info;
  fread(&info.size, sizeof(int), 1, fp);
  fread(&info.width, sizeof(int), 1, fp);
  fread(&info.height, sizeof(int), 1, fp);
  fread(&info.planes, sizeof(short), 1, fp);
  fread(&info.bit_count, sizeof(short), 1, fp);
  fread(&info.compression, sizeof(int), 1, fp);
  fread(&info.size_image, sizeof(int), 1, fp);
  fread(&info.x_res, sizeof(int), 1, fp);
  fread(&info.y_res, sizeof(int), 1, fp);
  fread(&info.clr_used, sizeof(int), 1, fp);
  fread(&info.clr_important, sizeof(int), 1, fp);
  return info;
}

static bi_palette_t read_palette(FILE *fp) {
  bi_palette_t p;
  fread(&p.b, sizeof(char), 1, fp);
  fread(&p.g, sizeof(char), 1, fp);
  fread(&p.r, sizeof(char), 1, fp);
  fread(&p.reserved, sizeof(char), 1, fp);
  return p;
}

static int get_palette_size(bi_info_t info) {
  return info.clr_used > 0 ? info.clr_used : 1 << info.bit_count;
}

void bi_free_img(bi_img_t img) {
  switch (img.info.bit_count) {
  case 8:
    free(img.data8);
    free(img.palettes);
    break;
  case 24:
    free(img.data24);
    break;
  case 32:
    free(img.data32);
    break;
  }
}

bi_img_t bi_read_img(FILE *fp) {
  bi_img_t img;
  img.header = read_header(fp);
  img.info = read_info(fp);

  if (img.info.bit_count == 8) {
    int palette_size = get_palette_size(img.info);
    img.palettes = (bi_palette_t *)calloc(sizeof(bi_palette_t), palette_size);
    for (int i = 0; i < palette_size; i++) {
      img.palettes[i] = read_palette(fp);
    }
  }

  void *data = calloc(img.info.bit_count / 8, img.info.width * img.info.height);
  switch (img.info.bit_count) {
  case 8:
    img.data8 = data;
    break;
  case 24:
    img.data24 = data;
    break;
  case 32:
    img.data32 = data;
    break;
  }

  int pad_size = img.info.width % 4;
  for (int i = 0; i < img.info.height; i++) {
    int idx = i * img.info.width;
    switch (img.info.bit_count) {
    case 8:
      fread(&img.data8[idx], sizeof(bi_data8_t), img.info.width, fp);
      break;
    case 24:
      fread(&img.data24[idx], sizeof(bi_data24_t), img.info.width, fp);
      break;
    case 32:
      fread(&img.data32[idx], sizeof(bi_data32_t), img.info.width, fp);
      break;
    }
    fseek(fp, pad_size, SEEK_CUR);
  }

  return img;
};
