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
uint8_t field = 0;

interrupt void verticalISR() {
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t v = i & 3;
        uint8_t r = field == 0 ? v : 0;
        uint8_t g = field == 1 ? v : 0;
        uint8_t b = field == 2 ? v : 0;
        PALETTE_BASE[i] = toPalette(r, g, b);
    }
    field = field < 2 ? (field+1) : 0;
    asm {
        ldb 0xff93
    }
}

void enableVideoIRQs() {
    disableInterrupts();
    *INIT0 = 0x5c; // GIME CC3, MMU, DRAM, 16k/16k ROM, FIRQ_en
    *IRQ_EN = 0x00; // DISABLE IRQs
    *FIRQ_EN = 0x08; // FIRQ for vertical ISR
    *HSYNC_CTRL &= 0xfe; // disable historic PIA Hsync IRQ
    *VSYNC_CTRL &= 0xfe; // disable historic PIA Vsync IRQ
    setFirq(verticalISR);
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