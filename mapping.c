/* 
 *      mapping.c
 *      Authors: Andrew Savage and Vivian Hong
 *      Date: 10/5/16
 *      HW3 Part C
 *
 *      Contains all the mapping functions used in ppmtrans.c to manipulate the
 *      image
 */

#include "mapping.h"

/* Purpose: Apply function used in a mapping function that rotates an image 0 
 *          degrees, i.e. places pixels in the destination image in the same
 *          indices as they appear in the source image
 * Arguments: Col, row, source image, element at index (col, row), closure
 *            containing desintaion image and methods
 * Returns: Nothing
 */
void rotate0(int col, int row, A2Methods_UArray2 image, void *element, void *cl)
{
        (void)image;
        int rotated_col = col;
        int rotated_row = row;
        Pnm_rgb pixel = (((MapClosure *)cl) -> methods -> 
                         at(((MapClosure *)cl) -> destination_image, 
                            rotated_col, rotated_row));
        pixel -> red = ((Pnm_rgb)element) -> red;
        pixel -> green = ((Pnm_rgb)element) -> green;
        pixel -> blue = ((Pnm_rgb)element) -> blue;
}

/* Purpose: Apply function used in a mapping function that rotates an image 90 
 *          degrees, i.e. places pixel from index (i, j) in source image
 *          at index (height - j - 1, i) in destination image
 * Arguments: Col, row, source image, element at index (col, row), closure
 *            containing desintaion image and methods
 * Returns: Nothing
 */
void rotate90(int col, int row, A2Methods_UArray2 image, void *element, 
              void *cl)
{
        int rotated_col = ((MapClosure *)cl) -> 
                methods -> height(image) - row - 1;
        int rotated_row = col;
        Pnm_rgb pixel = (((MapClosure *)cl) -> methods -> 
                         at(((MapClosure *)cl) -> destination_image, 
                            rotated_col, rotated_row));
        pixel -> red = ((Pnm_rgb)element) -> red;
        pixel -> green = ((Pnm_rgb)element) -> green;
        pixel -> blue = ((Pnm_rgb)element) -> blue;
}

/* Purpose: Apply function used in a mapping function that rotates an image 180
 *          degrees, i.e. places pixel from index (i, j) in source image
 *          at index (width - i - 1, height - row - j) in destination image
 * Arguments: Col, row, source image, element at index (col, row), closure
 *            containing desintaion image and methods
 * Returns: Nothing
 */
void rotate180(int col, int row, A2Methods_UArray2 image, void *element, 
               void *cl)
{
        int rotated_col = ((MapClosure *)cl) -> 
                methods -> width(image) - col - 1;
        int rotated_row = ((MapClosure *)cl) -> 
                methods -> height(image) - row - 1;;
        Pnm_rgb pixel = (((MapClosure *)cl) -> methods -> 
                         at(((MapClosure *)cl) -> destination_image, 
                            rotated_col, rotated_row));
        pixel -> red = ((Pnm_rgb)element) -> red;
        pixel -> green = ((Pnm_rgb)element) -> green;
        pixel -> blue = ((Pnm_rgb)element) -> blue;
}

/* Purpose: Apply function used in a mapping function that rotates an image 270 
 *          degrees, i.e. places pixel from index (i, j) in source image
 *          at index (j, width - i - 1) in destination image
 * Arguments: Col, row, source image, element at index (col, row), closure
 *            containing desintaion image and methods
 * Returns: Nothing
 */
void rotate270(int col, int row, A2Methods_UArray2 image, void *element, 
               void *cl)
{
        int rotated_col = row;
        int rotated_row = ((MapClosure *)cl) -> 
                methods -> width(image) - col - 1;
        Pnm_rgb pixel = (((MapClosure *)cl) -> methods -> 
                         at(((MapClosure *)cl) -> destination_image, 
                            rotated_col, rotated_row));
        pixel -> red = ((Pnm_rgb)element) -> red;
        pixel -> green = ((Pnm_rgb)element) -> green;
        pixel -> blue = ((Pnm_rgb)element) -> blue;
}

/* Purpose: Apply function used in a mapping function that flips an image 
 *          vertically, i.e. places pixel from index (i, j) in source image
 *          at index (i, height - j - 1) in destination image
 * Arguments: Col, row, source image, element at index (col, row), closure
 *            containing desintaion image and methods
 * Returns: Nothing
 */
void flip_vert(int col, int row, A2Methods_UArray2 image, void *element, 
               void *cl)
{
        int flipped_col = col;
        int flipped_row = ((MapClosure *)cl) -> 
                methods -> height(image) - row - 1;
        Pnm_rgb pixel = (((MapClosure *)cl) -> methods -> 
                         at(((MapClosure *)cl) -> destination_image, 
                            flipped_col, flipped_row));
        pixel -> red = ((Pnm_rgb)element) -> red;
        pixel -> green = ((Pnm_rgb)element) -> green;
        pixel -> blue = ((Pnm_rgb)element) -> blue;
}

/* Purpose: Apply function used in a mapping function that flips an image 
 *          horizontally, i.e. places pixel from index (i, j) in source image
 *          at index (width - i - 1, j) in destination image
 * Arguments: Col, row, source image, element at index (col, row), closure
 *            containing desintaion image and methods
 * Returns: Nothing
 */
void flip_horz(int col, int row, A2Methods_UArray2 image, void *element, 
               void *cl)
{
        int flipped_col = ((MapClosure *)cl) -> 
                methods -> width(image) - col - 1;
        int flipped_row = row;
        Pnm_rgb pixel = (((MapClosure *)cl) -> methods -> 
                         at(((MapClosure *)cl) -> destination_image, 
                            flipped_col, flipped_row));
        pixel -> red = ((Pnm_rgb)element) -> red;
        pixel -> green = ((Pnm_rgb)element) -> green;
        pixel -> blue = ((Pnm_rgb)element) -> blue;
}

/* Purpose: Apply function used in a mapping function that transposes an image,
 *          i.e. places pixel from index (i, j) in source image
 *          at index (j, i) in destination image
 * Arguments: Col, row, source image, element at index (col, row), closure
 *            containing desintaion image and methods
 * Returns: Nothing
 */
void transpose(int col, int row, A2Methods_UArray2 image, void *element, 
               void *cl)
{
        (void)image;
        int transposed_col = row;
        int transposed_row = col;
        Pnm_rgb pixel = (((MapClosure *)cl) -> methods -> 
                         at(((MapClosure *)cl) -> destination_image, 
                            transposed_col, transposed_row));
        pixel -> red = ((Pnm_rgb)element) -> red;
        pixel -> green = ((Pnm_rgb)element) -> green;
        pixel -> blue = ((Pnm_rgb)element) -> blue;
}
