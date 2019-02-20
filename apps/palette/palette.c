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
#define LINES 192

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
        pshs a,b,u,x // FIRQ is responsible for stacking these
        ldb firqEn // clear FIRQ
        ldb #DYNPAL
        ldu data
        ldx palCtrl
    loop:
        lda ,u+
        sta ,x+
        decb
        bne loop
        stu data
        puls a,b,u,x
    }
}

interrupt void verticalISR() {
    asm {
        ldb irqEn // clear IRQ
        ldu paletteData
        stu data
    }
}

void enableVideoIRQs() {
    disableInterrupts();

    *init0 |= 0x30; // GIME FIRQ and IRQ output enabled
    *firqEn |= 0x10; // horizontal boarder firq enabled
    *irqEn |= 0x08; // vertical irq enabled

    // This does bad things. WHY!?
    // *hsyncCtrl &= 0xfe; // disable historic PIA Hsync IRQ
    // *vsyncCtrl &= 0xfe; // disable historic PIA Vsync IRQ

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
    hscreen(GFXMODE);
    simpleRGB();

    /* Draw pixels */
    for (int j = 0; j < 192; j++) {
        for (int i = 0; i < 320; i++) {
            hset(i, j, (uint8_t) (i >> 4));
        }
    }

    /* Load per-line palette */
    paletteData = sbrk(DYNPAL*LINES);
    uint8_t* ptr = paletteData;
    for (uint8_t l = 0; l < LINES; l++) {
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
