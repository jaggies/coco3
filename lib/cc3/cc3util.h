/*
 * cc3util.h
 *
 *  Created on: Mar 1, 2019
 *      Author: jmiller
 */

#ifndef LIB_CC3_CC3UTIL_H_
#define LIB_CC3_CC3UTIL_H_

#include "ivec3i.h"

extern void makeRgbPalette(uint8_t rb, uint8_t gb, uint8_t bb);
extern uint8_t ditherVec3i(uint8_t i, uint8_t j, Vec3i* color);

extern uint16_t min(uint16_t a, uint16_t b);
extern uint16_t max(uint16_t a, uint16_t b);
extern int16_t abs(int16_t v);

#endif /* LIB_CC3_CC3UTIL_H_ */
