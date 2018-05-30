/*
 * mapClosure.h
 * Authors: Vivian Hong and Andrew Savage
 * Date: 10/5/16
 *
 * Struct for closure for mapping functions
 */

#ifndef MAPCLOSURE_H
#define MAPCLOSURE_H

typedef struct {
        A2Methods_UArray2 destination_image;
        A2Methods_T methods;
} MapClosure;

#endif /* MAPCLOSURE */
