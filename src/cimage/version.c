#include <stdio.h>

#include "cimage/version.h"

// print full cimage information
void cimage_print_full (void) {

	(void) printf ("Cimage Version: %s\n", CIMAGE_VERSION_NAME);
	(void) printf ("Release Date & time: %s - %s\n", CIMAGE_VERSION_DATE, CIMAGE_VERSION_TIME);
	(void) printf ("Author: %s\n", CIMAGE_VERSION_AUTHOR);

}

// print the version id
void cimage_print_version_id (void) {

	(void) printf ("Cimage Version ID: %s\n", CIMAGE_VERSION);

}

// print the version name
void cimage_print_version_name (void) {

	(void) printf ("Cimage Version: %s\n", CIMAGE_VERSION_NAME);

}