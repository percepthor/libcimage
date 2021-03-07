#include <stdlib.h>
#include <stdio.h>

#include <cimage/image.h>

int main (int argc, const char **argv) {

	if (argc > 2) {
		(void) printf ("\nTEST - Image shift values\n\n");

		Image *input = image_load_color (argv[1], 0, 0);

		if (input) {
			image_shift (input, 0, .4);
			image_shift (input, 1, .4);
			image_shift (input, 2, .4);

			if (!image_save (input, argv[2])) {
				(void) printf ("Done!\n\n");
			}

			image_delete (input);
		}
	}

	else {
		(void) printf ("\nUsage %s [input] [output]\n\n", argv[0]);
	}

	return 0;

}