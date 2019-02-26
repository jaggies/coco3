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

#define DYNPAL 2 // number of dynamic palette entries
#define LINES 200

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

interrupt void horizontalISR() {
    asm {
        ldb 0xff93  // clear FIRQ
        ldb #DYNPAL
        ldu data
        ldx #0xffb0
    loop:
        lda ,u+
        sta ,x+
        decb
        bne loop
        stu data
    }
}

interrupt void verticalISR() {
    asm {
        ldb 0xff92 // clear IRQ
        ldu paletteData
        stu data
    }
}

interrupt void nmiISR() {

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
    setNMI(nmiISR);

    enableInterrupts();
}

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    /* Graphics */
    setMode(320, LINES, 4);
    simpleRGB();
    for (uint8_t i = 0; i < 16; i++) {
        clear(i);
    }

    /* Draw pixels */
    int height = getHeight();
    int width = getWidth();
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            setPixel(i, j, (uint8_t) i);
        }
    }

    /* Load per-line palette */
    paletteData = sbrk(DYNPAL*LINES);
    uint8_t* ptr = paletteData;
    for (uint8_t l = 0; l < height; l++) {
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
