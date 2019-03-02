/*
 * palette.c
 *
 *  Created on: Feb 12, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"

const char GFXMODE = HS320x192x16;

#define WIDTH 320
#define HEIGHT 200
#define VBLANK (262 - HEIGHT)
#define DEPTH 4

#define DYNPAL 16 // number of dynamic palette entries that can be swapped per line

uint8_t* paletteData;
uint8_t* data;

void simpleRGB() {
    for (uint8_t i = 0; i < 4; i++) {
        paletteRGB(i, i, 0, 0); // red
        paletteRGB(i+4, 0, i, 0); // green
        paletteRGB(i+8, 0, 0, i); // blue
        paletteRGB(i+12, i, i, i); // white
    }
}

// Note: trashes x, y and w for performance reasons... BEWARE!
interrupt void horizontalISR() {
    asm {
        lda 0xff93  // clear IRQs
        ldb #DYNPAL // 6309 ONLY
        clra
        tfr d,w
        ldx data
        ldy #0xffb0
        tfm x+,y+ // 6309 ONLY
        stx data
    }
}

interrupt void verticalISR() {
    asm {
        ldb 0xff92 // clear IRQ
        ldu paletteData
        stu data
    }
}

void enableVideoIRQs() {
    disableInterrupts();

    *INIT0 |= 0x30; // GIME FIRQ and IRQ output enabled
    *FIRQ_EN |= 0x10; // horizontal boarder firq enabled
    *IRQ_EN |= 0x08; // vertical irq enabled

    *HSYNC_CTRL &= 0xfe; // disable historic PIA Hsync IRQ
    *VSYNC_CTRL &= 0xfe; // disable historic PIA Vsync IRQ

    setIrq(verticalISR);
    setFirq(horizontalISR);

    enableInterrupts();
}

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    /* Graphics */
    setMode(WIDTH, HEIGHT, DEPTH);
    simpleRGB();
    for (uint8_t i = 0; i < (1 << DEPTH); i++) {
        clear(i);
    }

    /* Draw pixels */
    int height = getHeight();
    int width = getWidth();
    uint8_t* buffer = (uint8_t*) sbrk(WIDTH);
    for (int i = 0; i < width; i++) {
       buffer[i] = (uint8_t) i & 0x0f;
    }

    uint16_t bytes = packPixels(buffer, buffer, WIDTH);

    for (int j = 0; j < height; j++) {
        setPixels(0, j, buffer, bytes);
    }

    /* Load per-line palette */
    paletteData = (uint8_t*) sbrk(DYNPAL*(height+VBLANK));
    uint8_t* ptr = paletteData;
    for (int l = 0; l < height+VBLANK; l++) {
        for (uint8_t p = 0; p < DYNPAL; p++) {
            uint8_t r = (p & 4) ? (l & 3) : 0;
            uint8_t g = (p & 2) ? (l & 3) : 0;
            uint8_t b = (p & 1) ? (l & 3) : 0;
            *ptr++ = toPalette(r, g, b);
        }
    }
    enableVideoIRQs();

    while (1)
        ;

    return 0;
}
