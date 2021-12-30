/*
 * RandomGen.h
 *
 *  Created on: Nov 12, 2021
 *      Author: ASUS
 */

#ifndef RANDOMGEN_H_
#define RANDOMGEN_H_



#endif /* RANDOMGEN_H_ */
/****************************************************************************************
* RandomGen.h - Some random stuff based on the Kinetis RNGA module.
* 11/02/2016 TDM
****************************************************************************************/
#ifndef RNG_INCL
#define RNG_INCL
/****************************************************************************************
* RNGInit() - Turns on the RNGA without an error interrupt.
* - Public
****************************************************************************************/
void RNGInit(void);
/****************************************************************************************
* RNGGet() - Returns a random number from the RNGA. Blocks based on OREG_LVL
****************************************************************************************/
INT32U RNGGet(void);
#endif
