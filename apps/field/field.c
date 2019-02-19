/*
 * palette.c
 *
 *  Created on: Feb 12, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "coco.h"
#include "cc3hw.h"
#include "cc3gfx.h"

const char GFXMODE = HS320x192x16;
int field = 0;

interrupt void verticalISR() {
    for (uint8_t i = 0; i < 16; i++) {
        paletteRGB(i, field == 0 ? i : 0, field == 1 ? i : 0, field == 2 ? i : 0);
    }
    field = field < 2 ? (field+1) : 0;
}

void enableVideoIRQs() {
    disableInterrupts();
    *init0 |= 0x30; // GIME FIRQ and IRQ output enabled
    *irqEn |= 0x08; // vertical irq enabled
    setIrq(verticalISR);
    enableInterrupts();
}

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    /* Graphics */
    hscreen(GFXMODE);

    /* Draw pixels */
    for (int j = 0; j < 192; j++) {
        for (int i = 0; i < 320; i++) {
            hset(i, j, (uint8_t) (i >> 4));
        }
    }

    enableVideoIRQs();

    while (1)
        ;

    return 0;
}
