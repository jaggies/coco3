/*
 * cc3raster.h
 *
 *  Created on: Apr 3, 2019
 *      Author: jmiller
 */

#ifndef LIB_CC3_CC3RASTER_H_
#define LIB_CC3_CC3RASTER_H_

extern void rasterPos(int x, int y);
extern void rasterColor(uint8_t color);
extern void rasterSet();
extern void rasterIncX();
extern void rasterDecX();
extern void rasterIncY();
extern void rasterDecY();

#endif /* LIB_CC3_CC3RASTER_H_ */
