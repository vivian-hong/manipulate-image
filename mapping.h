/* mapping.h
 * Authors: Andrew Savage and Vivian Hong
 * Date: 10/5/16
 * HW3
 * Header file for mapping functions for ppmtrans.c used in image manipulations
 */

#ifndef MAPPING_H
#define MAPPING_H

#include "a2methods.h"
#include "pnm.h"
#include "mapClosure.h"

void rotate0(int col, int row, A2Methods_UArray2 image, void *element,
             void *cl);
void rotate90(int col, int row, A2Methods_UArray2 image, void *element, 
              void *cl);
void rotate180(int col, int row, A2Methods_UArray2 image, void *element, 
               void *cl);
void rotate270(int col, int row, A2Methods_UArray2 image, void *element, 
               void *cl);
void flip_vert(int col, int row, A2Methods_UArray2 image, void *element, 
               void *cl);
void flip_horz(int col, int row, A2Methods_UArray2 image, void *element, 
               void *cl);
void transpose(int col, int row, A2Methods_UArray2 image, void *element, 
               void *cl);

#endif /* MAPPING_H */
