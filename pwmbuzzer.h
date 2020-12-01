/*
 * pwmbuzzer.h: Starter code for Lab 7, Fall 2020
 *
 * Created by Zhao Zhang
 * Modified by: Drew, Iraklis and James
 */

#ifndef PWMBUZZER_H_
#define PWMBUZZER_H_

void pwmBuzzerInit(); // in c file

void pwmBuzzerSet(uint32_t pulseWidth, uint32_t period); // in assembly

#endif /* PWMBUZZER_H_ */
