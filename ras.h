/*
 * ECE 266, fall 2020, Lab 6
 *
 * ras.h: Header file for ras-related functions
 *
 * Created by Muhammad, Contributed by Siddhant
 *
 */

#ifndef RAS_H_
#define RAS_H_

// Initialize the RAS
void rasInit();
void ras2Init();
void rasRead(uint32_t* data);
void ras2Read(uint32_t* data);


#endif /* RAS_H_ */
