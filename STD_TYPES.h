/*
 * type.h
 *
 *  Created on: Oct 27, 2024
 *      Author: Youssef Hany
 */

#ifndef TYPE_H_
#define TYPE_H_

typedef unsigned char uint8;
typedef signed char sint8;

typedef unsigned short int uint16;
typedef signed short int sint16;

typedef unsigned long int uint32;
typedef signed long int sint32;

typedef unsigned long long int uint64;
typedef signed long long int sint64;

typedef float float32;
typedef double float64;

typedef volatile unsigned char Vuint8;
typedef volatile signed char Vsint8;

typedef volatile unsigned short int Vuint16;
typedef volatile signed short int Vsint16;

typedef volatile unsigned long int Vuint32;
typedef volatile signed long int Vsint32;

typedef volatile unsigned long long int Vuint64;
typedef volatile signed long long int Vsint64;

typedef volatile float Vfloat32;
typedef volatile double Vfloat64;

typedef unsigned char* uint8_ptr;
typedef signed char* sint8_ptr;

typedef unsigned short int* uint16_ptr;
typedef signed short int* sint16_ptr;

typedef unsigned long int* uint32_ptr;
typedef signed long int* sint32_ptr;

typedef unsigned long long int* uint64_ptr;
typedef signed long long int* sint64_ptr;

typedef float* float32_ptr;
typedef double* float64_ptr;

#endif /* TYPE_H_ */
