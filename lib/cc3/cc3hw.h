/*
 * cc3hw.h
 *
 *  Created on: Feb 19, 2019
 *      Author: jmiller
 */

#ifndef LIB_COCO3_CC3HW_H_
#define LIB_COCO3_CC3HW_H_

extern const uint8_t *irqVector;
extern const uint8_t *firqVector;
extern const uint8_t *hsyncCtrl;
extern const uint8_t *vsyncCtrl;
extern const uint8_t *init0;
extern const uint8_t *irqEn;
extern const uint8_t *firqEn;
extern const uint8_t *palCtrl; // entry of palette 0

extern void set6309Native(); // Enables 6309 Native mode for higher performance
extern void setFirq(interrupt void (*fptr)());
extern void setIrq(interrupt void (*fptr)());

#endif /* LIB_COCO3_CC3HW_H_ */
