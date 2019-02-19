/*
 * cc3gfx.h
 *
 *  Created on: Feb 19, 2019
 *      Author: jmiller
 */

#ifndef LIB_COCO3_CC3GFX_H_
#define LIB_COCO3_CC3GFX_H_

// Constructs a palette entry of RGBRGB in the hardware format for Coco3.
extern uint8_t toPal(uint8_t r, uint8_t g, uint8_t b);

#define HS320x192x4 1
#define HS320x192x16 2
#define HS640x192x2 3
#define HS640x192x4 4

#endif /* LIB_COCO3_CC3GFX_H_ */
