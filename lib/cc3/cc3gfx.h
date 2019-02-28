/*
 * cc3gfx.h
 *
 *  Created on: Feb 19, 2019
 *      Author: jmiller
 */

#ifndef LIB_COCO3_CC3GFX_H_
#define LIB_COCO3_CC3GFX_H_

// Constructs a palette entry of RGBRGB in the hardware format for Coco3.
extern uint8_t toPalette(uint8_t r, uint8_t g, uint8_t b);

extern int setMode(uint16_t xres, uint16_t yres, uint8_t depth);
extern void setPalette(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
extern void clear(uint8_t color);

// Sets one pixel at location (x,y) in current graphics mode
extern void (*setPixel)(uint16_t x, uint16_t y, uint8_t clr);

// Sets multiple pixels starting at location (x,y) for n bytes in length.
// Assumes pixels are packed into the native screen format, e.g. 2 pixels per byte
// in 16-color mode.
//
// Note: x must land on a byte boundary. Use setPixel() to handle unaligned pixels.
// Returns number of bytes written.
extern void (*setPixels)(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n);

// Pack pixels from 1 pixel per byte to native pixel format chosen by setMode().
// Returns number of bytes written to output
extern uint16_t packPixels(uint8_t* const in, uint8_t * out, uint16_t n);

// Sets the graphics library to render at the given base address and points
// the GIME video register to it.
extern void setGraphicsBase(uint32_t base);

extern uint8_t getPixel(uint16_t x, uint16_t y);
extern uint16_t getWidth();
extern uint16_t getHeight();
extern uint16_t getBytesPerRow();
extern uint16_t getFrameSize();

#define HS320x192x4 1
#define HS320x192x16 2
#define HS640x192x2 3
#define HS640x192x4 4

#endif /* LIB_COCO3_CC3GFX_H_ */
