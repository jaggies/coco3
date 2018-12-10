/*
 * ftest.c
 *
 *  Created on: Dec 10, 2018
 *      Author: jmiller
 */

int main(int argc, char** argv) {
    printf("isqrt(%d) = %d\n", (1 << 24), fsqrt(1 + (1 << 24)));
    for (int i = 0; i < 64; i++) {
        fixed value = toFixed(i);
        fixed rt = fsqrt(value);
        printf("isqrt(%d) = %d  (%f %f)\n", value, rt, toFloat(value), toFloat(rt));
    }
}



