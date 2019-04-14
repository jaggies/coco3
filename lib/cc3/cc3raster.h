/*
 * cc3raster.h
 *
 *  Created on: Apr 3, 2019
 *      Author: jmiller
 */

#ifndef LIB_CC3_CC3RASTER_H_
#define LIB_CC3_CC3RASTER_H_

// Move raster to given position
extern void rasterPos(int16_t x, int16_t y);

// Specify the current color
extern void rasterColor(uint8_t color);

// Set pixel at current location to current color
extern void rasterSet();

// Move one pixel to the right
extern void rasterIncX();

// Move one pixel to the left
extern void rasterDecX();

// Move one pixel up
extern void rasterIncY();

// Move one pixel down
extern void rasterDecY();

#endif /* LIB_CC3_CC3RASTER_H_ */
