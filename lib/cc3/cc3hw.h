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
extern const uint16_t *vertOffset; // Graphics memory offset, bits [18:3]
extern const uint8_t *vertScroll; // vertical scroll register
extern const uint8_t *videoResolution; // Video resolution register
extern const uint8_t *videoMode;

#define MMU0000 ((uint8_t*) 0xffa0)
#define MMU2000 ((uint8_t*) 0xffa1)
#define MMU4000 ((uint8_t*) 0xffa2)
#define MMU6000 ((uint8_t*) 0xffa3)
#define MMU8000 ((uint8_t*) 0xffa4)
#define MMUA000 ((uint8_t*) 0xffa5)
#define MMUC000 ((uint8_t*) 0xffa6)
#define MMUE000 ((uint8_t*) 0xffa7)

extern void set6309Native(); // Enables 6309 Native mode for higher performance
extern void memset24(uint32_t addr, uint8_t value, uint8_t mask, uint16_t length);
extern void setFirq(interrupt void (*fptr)());
extern void setIrq(interrupt void (*fptr)());

#endif /* LIB_COCO3_CC3HW_H_ */
