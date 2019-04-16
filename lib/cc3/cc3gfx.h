/*
 * cc3gfx.h
 *
 *  Created on: Feb 19, 2019
 *      Author: jmiller
 */

#ifndef LIB_COCO3_CC3GFX_H_
#define LIB_COCO3_CC3GFX_H_

typedef struct _GfxState {
    int16_t width_pixels;
    uint16_t height_pixels;
    uint8_t bytes_per_row; // TODO: use 16-bit math if 256-color modes ever are a thing
    uint16_t buffer_size_bytes;
    uint8_t bpp;
    uint32_t base_addr;
    uint8_t base_page;
    int16_t base_y_offset;
    uint8_t pixel_mask; // current pixel masks (varies with position)
    int16_t rasterX;
    int16_t rasterY;
    uint8_t color;
} GfxState;

enum { X = 0, Y = 1, Z = 2 };

// Constructs a palette entry of RGBRGB in the hardware format for Coco3.
extern uint8_t toPalette(uint8_t r, uint8_t g, uint8_t b);

// Chooses a mode based on the parameters. Unknowns modes will result in upredictable results.
extern int setMode(uint16_t xres, uint16_t yres, uint8_t depth);

// Sets the palette index to the RGB values indicated by r, g, and b parameters.
// Valid values for r, g and b are 0..3.
extern void setPalette(uint8_t index, uint8_t r, uint8_t g, uint8_t b);

// Clear the screen to the given color. Valid values for color depend on the mode, up to 15.
extern void clear(uint8_t color);

// Sets one pixel at location (x,y) in current graphics mode to the current color
extern void (*setPixel)(uint16_t x, uint16_t y);

// Sets multiple pixels starting at location (x,y) for n bytes in length.
// Assumes pixels are packed into the native screen format, e.g. 2 pixels per byte
// in 16-color mode.
//
// Note: x must land on a byte boundary. Use setPixel() to handle unaligned pixels.
// Returns number of bytes written.
extern void (*setPixels)(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n);

// Fills multiple pixels starting at location (x,y) for n bytes in length.
// Note: x must land on a byte boundary. Use setPixel() to handle unaligned pixels.
// Returns number of bytes written.
extern void (*fillPixels)(uint16_t x, uint16_t y, int16_t n);

// Pack pixels from 1 pixel per byte to native pixel format chosen by setMode().
// Returns number of bytes written to output
extern uint16_t packPixels(uint8_t* const in, uint8_t * out, uint16_t n);

// Sets the graphics library to draw at the given base address. Must be on an MMU page boundary.
extern void setGraphicsDrawBase(uint32_t base);

// Sets the graphics library to display at the given base address.
extern void setGraphicsViewBase(uint32_t base);

// Gets the pixel value at location (x, y)
extern uint8_t getPixel(uint16_t x, uint16_t y);

// Gets the current width of the screen, as determined by last call to setMode()
extern uint16_t getWidth();

// Gets the current height of the screen, as determined by the last call to setMode()
extern uint16_t getHeight();

// Returns the total frame size for 1 frame of data, as determined by the last call to setMode()
extern uint16_t getFrameSize();

#endif /* LIB_COCO3_CC3GFX_H_ */
