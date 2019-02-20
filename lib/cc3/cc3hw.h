/*
 * cc3hw.h
 *
 *  Created on: Feb 19, 2019
 *      Author: jmiller
 */

#ifndef LIB_COCO3_CC3HW_H_
#define LIB_COCO3_CC3HW_H_

extern const uint8_t *irqVector; // MC6809E IRQ vector in memory
extern const uint8_t *firqVector; // MC6809E FIRQ vector in memory
extern const uint8_t *hsyncCtrl;
extern const uint8_t *vsyncCtrl;
extern const uint8_t *init0;
extern const uint8_t *irqEn; // IRQ enable register in GIME chip
extern const uint8_t *firqEn; // FIRQ enable register in GIME chip
extern const uint8_t *palCtrl; // Palette entry 0
extern const uint8_t *vertOffset; // Graphics memory offset, bits [18:3]

extern const uint8_t* mmuEntry0000;
extern const uint8_t* mmuEntry2000;
extern const uint8_t* mmuEntry4000;
extern const uint8_t* mmuEntry6000;
extern const uint8_t* mmuEntry8000;
extern const uint8_t* mmuEntryA000;
extern const uint8_t* mmuEntryC000;
extern const uint8_t* mmuEntryE000;

extern void set6309Native(); // Enables 6309 Native mode for higher performance
extern void setFirq(interrupt void (*fptr)());
extern void setIrq(interrupt void (*fptr)());

#endif /* LIB_COCO3_CC3HW_H_ */
