/*
 * BIT_WISE_OPS.h
 *
 *  Created on: Oct 31, 2024
 *      Author: Youssef Hany
 */

#ifndef BIT_WISE_OPS_H_
#define BIT_WISE_OPS_H_

#define SET_BIT(REG,BIT_NUM)		REG |= (1U<<BIT_NUM)
#define CLR_BIT(REG,BIT_NUM)		REG &= ~(1U<<BIT_NUM)
#define TOG_BIT(REG,BIT_NUM)		REG ^= (1U<<BIT_NUM)
#define GET_BIT(REG,BIT_NUM)		((REG>>BIT_NUM)&1U)

#endif /* BIT_WISE_OPS_H_ */
