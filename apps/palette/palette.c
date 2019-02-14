/*
 * palette.c
 *
 *  Created on: Feb 12, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cocogfx.h"

const char GFXMODE = HS320x192x16;

uint8_t *irqVector = * (uint8_t **) 0xFFF8;
uint8_t *firqVector = * (uint8_t **) 0xFFF6;
uint8_t *hsyncCtrl = (uint8_t *) 0xff01;
uint8_t *palCtrl = (uint8_t *) 0xffb0; // entry of palette 0

// Enables 6309 Native mode for higher speed
void set6309Native() {
    asm {
        ldmd  #$01
    }
}

static uint8_t clr;

interrupt void twiddle() {
    *palCtrl = clr++;
}

void enableHzIrq() {
    disableInterrupts();
    *hsyncCtrl = 0xf7; // enable Horizontal IRQ
    *irqVector = 0x7E;  // extended JMP extension
    *(void **) (irqVector + 1) = (void *) twiddle;
    enableInterrupts();
}

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    /* Graphics */
    hscreen(GFXMODE);
    enableHzIrq();

    while (1)
        ;

    return 0;
}
