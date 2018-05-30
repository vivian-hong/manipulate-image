/*
 * commands.h
 * Authors: Vivian Hong and Andrew Savage
 * Date: 10/5/16
 * HW3 Part C
 *
 * Header file for functions used to manipulate images and time these
 * manipulations
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include "a2methods.h"
#include "cputiming.h"
#include "mapping.h"

A2Methods_UArray2 manipulate_image(A2Methods_UArray2 image, char *manipulate, 
                                   int rotation, A2Methods_T methods, 
                                   A2Methods_mapfun *map, int vert, 
                                   CPUTime_T timer, double *runtime);
A2Methods_UArray2 rotate_img(A2Methods_UArray2 image, int rotation,
                             A2Methods_T methods, A2Methods_mapfun *map,
                             CPUTime_T timer, double *runtime);
A2Methods_UArray2 flip(A2Methods_UArray2 image, A2Methods_T methods, 
                       A2Methods_mapfun map, int vert, CPUTime_T timer, 
                       double *runtime);
A2Methods_UArray2 transpose_img(A2Methods_UArray2 image, A2Methods_T methods,
                                A2Methods_mapfun *map, CPUTime_T timer, 
                                double *runtime);
void time_check(A2Methods_mapfun map, A2Methods_UArray2 image, 
                void apply(int col, int row, A2Methods_UArray2 image, void *element, void *cl), 
                void *cl, CPUTime_T timer, double *runtime);

#endif /* COMMANDS_H */
