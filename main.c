#include "bmp.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void usage() { printf("usage: bmp [filename]"); }

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    usage();
    return EXIT_FAILURE;
  }

  const char *filename = argv[1];
  FILE *fp = fopen(filename, "r");
  bi_img_t img = bi_read_img(fp);

  printf("header blc\n");
  printf("\tmagic %x\n", img.header.magic);
  printf("\tsize %d\n", img.header.size);
  printf("\toffset %d\n", img.header.offset);

  printf("info blc:\n");
  printf("\tsize %d\n", img.info.size);
  printf("\tw %d\n", img.info.width);
  printf("\th %d\n", img.info.height);
  printf("\tx res %d\n", img.info.x_res);
  printf("\ty res %d\n", img.info.y_res);
  printf("\timage size %d\n", img.info.size_image);
  printf("\tbit count %d\n", img.info.bit_count);

  switch (img.info.compression) {
  case BI_RGB:
    printf("\tcompression BI_RGB\n");
    break;
  case BI_RLE8:
    printf("\tcompression BI_RLE8\n");
    break;
  case BI_RLE4:
    printf("\tcompression BI_RLE4\n");
    break;
  case BI_BITFIELDS:
    printf("\tcompression BI_BITFIELDS\n");
    break;
  default:
    printf("\tcompression %d(not supported)\n", img.info.compression);
  }

  SDL_Event event;
  SDL_Renderer *renderer;
  SDL_Window *window;
  int i;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(img.info.width, img.info.height, 0, &window,
                              &renderer);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  for (int i = img.info.height; i >= 0; i--) {
    for (int j = 0; j < img.info.width; j++) {
      int data_idx = img.info.width * i + j;
      bi_palette_t palette = bi_palette_at(img, j, i);
      SDL_SetRenderDrawColor(renderer, palette.r, palette.g, palette.b, 255);
      SDL_RenderDrawPoint(renderer, j, img.info.height - i);
    }
  }

  SDL_RenderPresent(renderer);
  while (1) {
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      break;
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  bi_free_img(img);
  return EXIT_SUCCESS;
}
