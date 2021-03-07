#include <stdlib.h>
#include <stdio.h>

#include <cimage/image.h>

int main (int argc, const char **argv) {

	if (argc > 2) {
		(void) printf ("\nTEST - Image from RGB to gray\n\n");

		Image *input = image_load_color (argv[1], 0, 0);

		if (input) {
			Image *gray = image_grayscale (input);
			if (gray) {
				if (!image_save (gray, argv[2])) {
					(void) printf ("Done!\n\n");
				}

				image_delete (gray);
			}

			image_delete (input);
		}
	}

	else {
		(void) printf ("\nUsage %s [input] [output]\n\n", argv[0]);
	}

	return 0;

}