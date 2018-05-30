/* 
 *  uarray2b.c
 *  by Vivian Hong and Andrew Savage, 9/28/16
 *  HW3 Part A
 *
 *  Implementation of a blocked array, in which elements in a block are near
 *  each other in memory. ADD DESIGN TODO
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "uarray2b.h"
#include "uarray.h"
#include "except.h"

#define T UArray2b_T

const Except_T NULL_ARRAY = { "UArray2b functions cannot take null arrays" };
const Except_T OUT_OF_BOUNDS = { "Trying to access out of bounds index" };
const Except_T INVALID_UARRAY2B = { "Can't make array with given parameters" };
const Except_T NULL_MALLOC = { "Couldn't allocate sufficient space" };
const double BLOCK64K = 65536;

/* definition of struct UArray2b_T */
struct T {
        int blocksize;
        int width;
        int height;
        UArray_T block_array; 
};

/* block_array is a one dimensional array representing a two dimensional array
 * stored by block with blocks in the array bieng stored column major and 
 * elements within the blocks stored column major as well
 */

/* Initializes the 2D blocked UArray
 * Size of the UArray2b will always be >= width * height
 * Requires: an int for the number of rows and cols, for the size of elements
 *           to be stored, and for the size of blocks that the elements should 
 *           stored in
 * Returns: a UArray2b_T for the client to have a reference to
 */
T UArray2b_new(int width, int height, int size, int blocksize)
{
        if (width <= 0 || height <= 0 || size <= 0 || blocksize < 1) {
                RAISE(INVALID_UARRAY2B);
        }
        T array2b = malloc(sizeof(*array2b));
        if (array2b == NULL) {
                RAISE(NULL_MALLOC);
        }
        int array_len = ((width + blocksize - 1) / blocksize) * 
                ((height + blocksize - 1) / blocksize) * blocksize * blocksize;
        array2b -> width = width;
        array2b -> height = height;
        array2b -> blocksize = blocksize;
        array2b -> block_array = UArray_new(array_len, size);
        return array2b;
}

/* Initializes the 2D blocked UArray with most efficient blocksize
 * Size of the UArray2b will always be >= width * height
 * Requires: an int for the number of rows and cols, for the size of elements
 *           to be stored
 * Returns: a UArray2b_T for the client to have a reference to
 */
T UArray2b_new_64K_block(int width, int height, int size)
{
        if (width <= 0 || height <= 0 || size <= 0) {
                RAISE(INVALID_UARRAY2B);
        }
        T array2b = malloc(sizeof(*array2b));
        int blocksize;
        if (array2b == NULL) {
                RAISE(NULL_MALLOC);
        }
        blocksize = (int)sqrt(BLOCK64K / (double)size);
        if (blocksize == 0) {
                blocksize = 1;
        }
        int array_len = ((width + blocksize - 1) / blocksize) * 
                ((height + blocksize - 1) / blocksize) * blocksize * blocksize;
        array2b -> width = width;
        array2b -> height = height;
        array2b -> blocksize = blocksize; 
        array2b -> block_array = UArray_new(array_len, size);
        return array2b;
}

/* Frees associated UArray2b_T from heap
 * Requires: a pointer to a UArray2b_T to free
 * Returns: nothing
 */
void UArray2b_free(T *array2b)
{
        if (array2b == NULL || *array2b == NULL) {
                RAISE(NULL_ARRAY);
        }
        UArray_free(&((*array2b) -> block_array));
        free(*array2b);
}

/* Accesses the width of the blocked 2D UArray
 * Requires: a UArray2b_T to get the width of
 * Returns: an int for the width of the array
 */
int UArray2b_width(T array2b)
{
        if (array2b == NULL) {
                RAISE(NULL_ARRAY);
        }
        return array2b -> width;
}

/* Accesses the height of the blocked 2D UArray
 * Requires: a UArray2b_T to get the height of
 * Returns: an int for the height of the array
 */
int UArray2b_height(T array2b)
{
        if (array2b == NULL) {
                RAISE(NULL_ARRAY);
        }
        return array2b -> height;
}

/* Accesses the size of the blocked 2D UArray elements
 * Requires: a UArray2b_T to get the element size of
 * Returns: an int for the size of the elements in the array
 */
int UArray2b_size(T array2b)
{
        if (array2b == NULL) {
                RAISE(NULL_ARRAY);
        }
        return UArray_size(array2b -> block_array);
}

/* Accesses the blocksize of the blocked 2D UArray
 * Requires: a UArray2b_T to get the blocksize of
 * Returns: an int for the blocksize of the array
 */
int UArray2b_blocksize(T array2b)
{
        if (array2b == NULL) {
                RAISE(NULL_ARRAY);
        }
        return array2b -> blocksize;
}

/* Accesses the element at the specified row and col indexes
 * Requires: a UArray2b_T to get the element from, and an int for the
 *           row and col of the desired element
 * Returns: a void pointer to the desired element
 */
void *UArray2b_at(T array2b, int column, int row)
{
        if (array2b == NULL) {
                RAISE(NULL_ARRAY);
        }
        if (row < 0 || column  < 0 || row >= array2b -> height ||
            column >= array2b -> width) {
                RAISE(OUT_OF_BOUNDS);
        }
        /* In order not to have to access blocksize every time */
        int bsize = array2b -> blocksize;

        /* Block indices in which the desired cell is located */
        int block_col = column / bsize;
        int block_row = row / bsize;

        /* Index of the first element of the block specified in the UArray_T */
        int block_index = bsize * bsize * (((array2b -> height + bsize - 1) 
                                            / bsize) * block_col + block_row);

        /* Index of the desired cell WITHIN the specified block */
        int cell_index = bsize * (column % bsize) + (row % bsize);
        return UArray_at(array2b -> block_array, cell_index + block_index);
}

/* Applies a function to each element in the blocked 2D UArray, one full block
 * at a time with cells within a block being close to eachother in memory
 * Requires: a UArray2b_T to map to, a function pointer to apply to each 
 *           element, and a void pointer to the closure data
 * Returns: nothing
 */
void UArray2b_map(T array2b, void apply(int col, int row, T array2b,
                                               void *elem, void *cl),
                         void *cl)
{
        if (array2b == NULL) {
                RAISE(NULL_ARRAY);
        }
        /* In order not to have to access blocksize every time */
        int bsize = array2b -> blocksize;

        for (int i = 0; i < UArray_length(array2b -> block_array); i++) {
                /* Returns index of block in UArray_T relative to other blocks
                 * in the array, so that small_block_index is between 0 and the
                 * number of blocks - 1
                 */
                int small_block_index = (i / (bsize * bsize));

                /* These variables represent the same thing as in UArray2b_at */
                int cell_index = i % (bsize * bsize);
                int block_col =  small_block_index / 
                        ((array2b -> height + bsize - 1) / bsize);
                int block_row = small_block_index % 
                        ((array2b -> height + bsize - 1) / bsize);
                int col = block_col * bsize + cell_index / bsize;
                int row = block_row * bsize + cell_index % bsize;

                /* Check if [col,row] is a cell in use in array */
                if (col < array2b -> width && row < array2b -> height) {
                        apply(col, row, array2b, 
                              UArray_at(array2b -> block_array, i), cl);
                }
        }
}
