/* ppmtrans.c
 * Authors: Andrew Savage and Vivian Hong
 * Date: 10/6/16
 * HW3 Part C
 * 
 * Main and helper functions for program that will manipulate an image according
 * to specified commands and outputs the final image to stdout
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "commands.h"
#include "a2blocked.h"
#include "a2plain.h"

/* Can set methods to either the methods in a2plain or a2blocked based on the 
 * type of mapping. Mapping may also be changed from the defaults.
 */
#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (0)

Pnm_ppm read_image(FILE *image_file, A2Methods_T methods);
void write_image(A2Methods_UArray2 destination_image, Pnm_ppm pixmap, 
                 A2Methods_T methods);
double time_per_pixel(A2Methods_UArray2 destination_image, double runtime, 
                      A2Methods_T methods);
void write_timing_file(double runtime, double pixel_time, char *time_file_name,
                       int width, int height);

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}

int main(int argc, char *argv[]) 
{
        char   *time_file_name = NULL;
        char   *flip_dir       = "";          /* horizontal or vertical */
        char   *manipulate     = "";          /* Manipulation to be executed */
        int     rotation       = 0;
        int     vert           = 0;           /* Treated as bool, 1 = TRUE */
        int     given_input    = 0;           /* Treated as bool, 1 = TRUE */
        int     i;
        double  runtime;                      /* Total time of manipulation */

        FILE   *image_file;                   /* Image to be read */
        Pnm_ppm pixmap;                       /* Pixmap created from input */
        CPUTime_T timer        = NULL;        /* Used to time manipulation */

        /* 2D Uarray that mapping functions will input pixels into */
        A2Methods_UArray2 destination_image;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
                                    "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
                                    "column-major");
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
                                        "Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                        manipulate = "rotate";
                } else if (strcmp(argv[i], "-flip") == 0) {
                        if (!(i + 1 < argc)) {      /* no flip direction */
                                usage(argv[0]);
                        }
                        flip_dir = argv[++i];
                        if (strcmp(flip_dir, "horizontal") != 0 && 
                            strcmp(flip_dir, "vertical") != 0) {
                                fprintf(stderr, "Flip direction must be "
                                                "horizontal or vertical\n");
                                usage(argv[0]);
                        }
                        if (strcmp(flip_dir, "vertical") == 0) {
                                vert = 1;
                        }
                        manipulate = "flip";
                } else if (strcmp(argv[i], "-transpose") == 0) {
                        manipulate = "transpose";
                } else if (strcmp(argv[i], "-time") == 0) {
                        time_file_name = argv[++i];
                        if (time_file_name  == NULL) { /* no file name given */
                                usage(argv[0]);
                        }
                        timer = CPUTime_New();
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0], 
                                argv[i]);
                } else if (i == argc - 1) { /* checks for a provided file */
                        image_file = fopen(argv[i], "r");
                        if (image_file == NULL) {
                                usage(argv[0]);
                        }
                        pixmap = read_image(image_file, methods);
                        fclose(image_file);
                        given_input = 1;
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
                        break;
                }
        }
        if (given_input == 0) {
                pixmap = read_image(stdin, methods);
        }

        if (strcmp(manipulate, "") == 0) {
                manipulate = "rotate";
        }

        destination_image = manipulate_image(pixmap -> pixels, manipulate,
                                             rotation, methods, map, vert, 
                                             timer, &runtime);
        write_image(destination_image, pixmap, methods);
        if (time_file_name != NULL) {
                write_timing_file(runtime, time_per_pixel(destination_image, 
                                                          runtime, methods), 
                                  time_file_name, 
                                  methods -> width(destination_image), 
                                  methods -> height(destination_image));
        }
        Pnm_ppmfree(&pixmap);
        return EXIT_SUCCESS;
}

/* Transfers the image file to a Pnm_ppm file
 * Requires: a file pointer and A2Methods_T to be given to pnm.h to make the
 *           file a Pnm_ppm
 * Returns: The Pnm_ppm created from the image file
 */
Pnm_ppm read_image(FILE *image_file, A2Methods_T methods)
{
        return Pnm_ppmread(image_file, methods);
}

/* Sets the elements in the Pnm_ppm to use the manipulated image instead of
 * the original image
 * Requires: an A2Methods_UArray2 that contains the manipulated image (and all
 *           of its related information), A2Methods_T to access this info, and
 *           a Pnm_ppm to transfer this information to
 * Returns: Nothing
 */
void write_image(A2Methods_UArray2 destination_image, Pnm_ppm pixmap, 
                 A2Methods_T methods)
{
        methods -> free(&(pixmap -> pixels));
        pixmap -> pixels = destination_image;
        pixmap -> width = (unsigned)methods -> width(destination_image);
        pixmap -> height = (unsigned)methods -> height(destination_image);
        Pnm_ppmwrite(stdout, pixmap);
}

/* Calculates the time taken for the manipulation per pixel
 * Requires: an A2Methods_UArray2 with height and width info, A2Methods_T to
 *           access this information, and the already calculated total time
 *           taken for the manipulation
 * Returns: the time taken per pixel
 */
double time_per_pixel(A2Methods_UArray2 destination_image, double runtime, 
                      A2Methods_T methods)
{
        return runtime / (methods -> width(destination_image) * 
                          methods -> height(destination_image));
}

/* Writes the timing information to the specified file
 * Requires: two doubles containing timing info (one for total time, one for
 *           time for pixel) and the specified file name to write info to
 * Returns: Nothing
 */
void write_timing_file(double runtime, double pixel_time, char *time_file_name,
                       int width, int height)
{
        FILE *time_file = fopen(time_file_name, "a");
        fprintf(time_file, "Size of image was %i X %i \n"
                           "|%.1f|%f|%f|\n\n",
                width, height, runtime, pixel_time, pixel_time);
        fclose(time_file);
}
