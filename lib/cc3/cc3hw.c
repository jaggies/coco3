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
uint8_t *vertOffset = (uint8_t*) 0xff9d; // vertical offset, bits [18:3]

uint8_t* mmuEntry0000 = (uint8_t*) 0xffa0;
uint8_t* mmuEntry2000 = (uint8_t*) 0xffa1;
uint8_t* mmuEntry4000 = (uint8_t*) 0xffa2;
uint8_t* mmuEntry6000 = (uint8_t*) 0xffa3;
uint8_t* mmuEntry8000 = (uint8_t*) 0xffa4;
uint8_t* mmuEntryA000 = (uint8_t*) 0xffa5;
uint8_t* mmuEntryC000 = (uint8_t*) 0xffa6;
uint8_t* mmuEntryE000 = (uint8_t*) 0xffa7;

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

