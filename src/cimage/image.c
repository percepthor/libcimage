#include <stdlib.h>

#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
#endif

#include "cimage/stb/image.h"

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
	#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

#include "cimage/stb/image_write.h"

#include "cimage/image.h"
#include "cimage/utils.h"

Image *image_new (void) {

	Image *image = (Image *) malloc (sizeof (Image));
	if (image) {
		image->w = 0;
		image->h = 0;
		image->c = 0;

		image->data = NULL;
	}

	return image;

}

void image_delete (void *image_ptr) {

	if (image_ptr) {
		Image *image = (Image *) image_ptr;

		if (image->data) free (image->data);

		free (image);
	}

}

Image *image_create (int w, int h, int c) {

	Image *image = image_new ();
	if (image) {
		image->w = w;
		image->h = h;
		image->c = c;

		image->data = (float *) calloc (h * w * c, sizeof (float));
	}

	return image;

}

static Image *image_make (
	unsigned char *data,
	int w, int h, int c
) {

	Image *image = image_create (w, h, c);

	if (image) {
		int dst_index = 0;
		int src_index = 0;
		for (int k = 0; k < c; ++k) {
			for (int j = 0; j < h; ++j) {
				for (int i = 0; i < w; ++i) {
					dst_index = i + w * j + w *h * k;
					src_index = k + c * i + c * w * j;
					image->data[dst_index] = (float) data[src_index] / 255.0;
				}
			}
		}
	}

	return image;

}

static Image *image_load_mem_stb (
	const unsigned char *buffer, int buffer_len, int channels
) {

	Image *image = NULL;

	if (buffer) {
		int w, h, c;
		unsigned char *data = stbi_load_from_memory (
			buffer, buffer_len,
			&w, &h, &c, channels
		);
		if (data) {
			if (channels) c = channels;

			image = image_make (data, w, h, c);

			free (data);
		}

		else {
			// osiris_log_error ("Failed to load image from file!");
			#ifdef OSIRIS_DEBUG
			fprintf (stderr, "STB Reason: %s\n", stbi_failure_reason ());
			#endif
		}
	}

	return image;

}

Image *image_load_mem (
	const unsigned char *buffer, int buffer_len,
	int w, int h, int c
) {

	Image *image = NULL;

	if (buffer) {
		image = image_load_mem_stb (buffer, buffer_len, c);

		// TODO: resize image if needed
		// if ((h && w) && (h != out.h || w != out.w)){
		//     image resized = resize_image(out, w, h);
		//     free_image(out);
		//     out = resized;
		// }
	}

	return image;

}

static Image *image_load_stb (
	const char *filename, int channels
) {

	Image *image = NULL;

	if (filename) {
		int w, h, c;
		unsigned char *data = stbi_load (filename, &w, &h, &c, channels);
		if (data) {
			if (channels) c = channels;

			image = image_make (data, w, h, c);

			free (data);
		}

		else {
			(void) fprintf (stderr, "Failed to load image from file!");
			#ifdef OSIRIS_DEBUG
			(void) fprintf (
				stderr, "STB Reason: %s\n", stbi_failure_reason ()
			);
			#endif
		}
	}

	return image;

}

Image *image_load (const char *filename, int w, int h, int c) {

	Image *image = NULL;

	if (filename) {
		image = image_load_stb (filename, c);

		// TODO: resize image if needed
		// if ((h && w) && (h != out.h || w != out.w)){
		//     image resized = resize_image(out, w, h);
		//     free_image(out);
		//     out = resized;
		// }
	}

	return image;

}

Image *image_load_color (const char *filename, int w, int h) {

	return filename ? image_load (filename, w, h, 3) : NULL;

}

float image_get_pixel (Image *im, int x, int y, int c) {

	return (x < im->w && y < im->h && c < im->c) ?
		im->data[c * im->h * im->w + y * im->w + x] : 0;

}

void image_set_pixel (Image *im, int x, int y, int c, float val) {

	if (!(x < 0 || y < 0 || c < 0 || x >= im->w || y >= im->h || c >= im->c)) {
		im->data[c * im->h * im->w + y * im->w + x] = val;
	}

}

Image *image_grayscale (Image *input) {

	Image *gray = image_create (input->w, input->h, 1);
	if (gray) {
		float scale[] = { 0.299, 0.587, 0.114 };
		int i = 0, j = 0, k = 0;
		for (k = 0; k < input->c; k++) {
			for (j = 0; j < input->h; j++) {
				for (i = 0; i < input->w; i++) {
					gray->data[i + input->w * j] +=
						(scale[k] * input->data[k * input->h * input->w + j * input->w + i]);
				}
			}
		}
	}

	return gray;

}

void image_shift (Image *im, int c, float v) {

	unsigned int stop = (c + 1) * im->w * im->h;
	for (unsigned int i = c * im->w * im->h; i < stop; i++) {
		im->data[i] += v;
	}

}

void image_clamp (Image *im) {

	unsigned int stop = im->w * im->h * im->c;
	for (int x = 0; x < stop; x++) {
		if (im->data[x] > 1) im->data[x] = 1;
		else if (im->data[x] < 0)
			im->data[x] = 0;
	}

}

void image_rgb_to_hsv (Image *im) {

	if (im->c == 3) {
		int i = 0, j = 0;
		float r = 0, g = 0, b = 0;
		float h = 0, s = 0, v = 0;
		for (j = 0; j < im->h; j++) {
			for (i = 0; i < im->w; i++) {
				r = image_get_pixel (im, i , j, 0);
				g = image_get_pixel (im, i , j, 1);
				b = image_get_pixel (im, i , j, 2);

				float max = three_way_max (r, g, b);
				float min = three_way_min (r, g, b);
				float delta = max - min;

				v = max;

				if (max == 0) {
					s = 0;
					h = 0;
				}

				else {
					s = delta / max;

					if (r == max) h = (g - b) / delta;
					else if (g == max) h = 2 + (b - r) / delta;
					else h = 4 + (r - g) / delta;

					if (h < 0) h += 6;

					h = h / 6.0;
				}

				image_set_pixel (im, i, j, 0, h);
				image_set_pixel (im, i, j, 1, s);
				image_set_pixel (im, i, j, 2, v);
			}
		}
	}

}

int image_save (Image *im, const char *name) {

	int retval = 1;

	if (im) {
		unsigned char *data = calloc (im->w * im->h * im->c, sizeof (char));
		int i = 0, k = 0;
		for (k = 0; k < im->c; k++){
			for (i = 0; i < im->w * im->h; i++) {
				data[i * im->c + k] = (unsigned char) (255 * im->data[i + k* im->w * im->h]);
			}
		}

		retval = stbi_write_jpg (name, im->w, im->h, im->c, data, 80);

		free (data);
	}

	return retval;

}