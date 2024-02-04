#ifndef _FUZZY_LOGIC_H_
#define _FUZZY_LOGIC_H_

#include <stdio.h>

#define min(a,b) a < b ? a : b
#define max(a,b) a > b ? a: b

#define FIS_TYPE float
#define FIS_RESOLUSION 101
#define FIS_MIN -3.4028235E+38
#define FIS_MAX 3.4028235E+38
typedef FIS_TYPE(*_FIS_MF)(FIS_TYPE, FIS_TYPE*);
typedef FIS_TYPE(*_FIS_ARR_OP)(FIS_TYPE, FIS_TYPE);
typedef FIS_TYPE(*_FIS_ARR)(FIS_TYPE*, int, _FIS_ARR_OP);


void fis_evaluate(void);
void Fuzzy_Logic_Input(FIS_TYPE input1, FIS_TYPE input2);
unsigned char Fuzzy_Logic_Output(void);
#endif
