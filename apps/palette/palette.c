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

#define HBRD 0x10 // IRQ_EN/FIRQ_EN enables
#define VBRD 0x08

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
interrupt void firqISR() {
    asm {
        lda 0xff93  // clear FIRQs
        anda #VBRD
        bne vert
    horiz:
        ldx data
        ldy #0xffb0
        ldw #DYNPAL // 6309 ONLY
        tfm x+,y+ // 6309 ONLY
        stx data
        rti
    vert:
        ldx paletteData
        stx data
        rti
    }
}

void enableVideoIRQs() {
    disableInterrupts();

    *INIT0 |= 0x30; // GIME FIRQ and IRQ output enabled
    *FIRQ_EN |= HBRD | VBRD; // horizontal boarder firq enabled
    *IRQ_EN |= 0x00; // vertical irq enabled

    *HSYNC_CTRL &= 0xfe; // disable historic PIA Hsync IRQ
    *VSYNC_CTRL &= 0xfe; // disable historic PIA Vsync IRQ

    setFirq(firqISR);

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
        uint8_t clr = ((uint8_t)l & 3);
        for (uint8_t p = 0; p < DYNPAL; p++) {
            uint8_t r = ((p+l) & 4) ? clr : 0;
            uint8_t g = ((p+l) & 2) ? clr : 0;
            uint8_t b = ((p+l) & 1) ? clr : 0;
            *ptr++ = toPalette(r, g, b);
        }
    }
    enableVideoIRQs();

    while (1)
        ;

    return 0;
}
