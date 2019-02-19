/*
 * cc3hw.c
 *
 *  Created on: Feb 19, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"

uint8_t *irqVector = * (uint8_t **) 0xFFF8;
uint8_t *firqVector = * (uint8_t **) 0xFFF6;
uint8_t *hsyncCtrl = (uint8_t *) 0xff01;
uint8_t *vsyncCtrl = (uint8_t *) 0xff03;
uint8_t *init0 = (uint8_t *) 0xff90;
uint8_t *irqEn = (uint8_t *) 0xff92;
uint8_t *firqEn = (uint8_t *) 0xff93;
uint8_t *palCtrl = (uint8_t *) 0xffb0; // entry of palette 0

// Enables 6309 Native mode for higher performance
void set6309Native() {
    asm {
        ldmd  #$01
    }
}

void setFirq(interrupt void (*fptr)()) {
    *firqVector = 0x7E;  // extended JMP extension
    *(void **) (firqVector + 1) = (void *) fptr;
}

void setIrq(interrupt void (*fptr)()) {
    *irqVector = 0x7E;  // extended JMP extension
    *(void **) (irqVector + 1) = (void *) fptr;
}

