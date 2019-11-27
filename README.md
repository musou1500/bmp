# bmp

bmp reader can read uncompressed(BI_RGB) 8, 24, 32bit bmp.

## build

this library requires sdl2.

```
$ make
```

## usage

see `example/main.c` for more detail.

```c
FILE *fp = fopen(filename, "r");
bi_img_t img = bi_read_img(fp);
printf("width %d\n", img.info.width);
printf("height %d\n", img.info.height);

for (int i = img.info.height; i >= 0; i--) {
  for (int j = 0; j < img.info.width; j++) {
    bi_palette_t palette = bi_palette_at(img, j, i);
    printf("rgb %x %x %x", palette.r, palette.g, palette.b)
  }
}

bi_free_img(img);

```

