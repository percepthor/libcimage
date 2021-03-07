#ifndef _CIMAGE_IMAGE_H_
#define _CIMAGE_IMAGE_H_

struct _Image {

	int w;
	int h;
	int c;
	float *data;

};

typedef struct _Image Image;

extern Image *image_new (void);

extern void image_delete (void *image_ptr);

extern Image *image_create (
	int w, int h, int c
);

extern Image *image_load_mem (
	const unsigned char *buffer, int buffer_len,
	int w, int h, int c
);

extern Image *image_load (
	const char *filename, int w, int h, int c
);

extern Image *image_load_color (
	const char *filename, int w, int h
);

extern float image_get_pixel (
	Image *im, int x, int y, int c
);

extern void image_set_pixel (
	Image *im, int x, int y, int c, float val
);

extern Image *image_grayscale (Image *input);

extern void image_shift (Image *im, int c, float v);

extern void image_clamp (Image *im);

extern void image_rgb_to_hsv (Image *im);

extern int image_save (Image *im, const char *name);

#endif