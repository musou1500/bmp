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
  free(img.data);
  free(img.palettes);
}

bi_img_t bi_read_img(FILE *fp) {
  bi_header_t header = read_header(fp);
  bi_info_t info = read_info(fp);

  int palette_size = get_palette_size(info);
  bi_palette_t *palettes =
      (bi_palette_t *)calloc(sizeof(bi_palette_t), palette_size);
  for (int i = 0; i < palette_size; i++) {
    palettes[i] = read_palette(fp);
  }

  unsigned char *data =
      (unsigned char *)calloc(sizeof(char), info.width * info.height);
  int pad_size = info.width % 4;
  for (int i = 0; i < info.height; i++) {
    int idx = i * info.width;
    fread(&data[idx], sizeof(unsigned char), info.width, fp);
    fseek(fp, pad_size, SEEK_CUR);
  }

  return (bi_img_t){header, info, palettes, data};
};
