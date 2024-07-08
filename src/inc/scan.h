/**
* @file scan.h
* @brief 
* @details 
*/

#include "stdint.h"

#ifndef _scan_h_
#define _scan_h_

enum {
    SCAN0,
    SCAN1,
    SCAN2,
    SCAN3,
    NUMOF_SCANLINE,
};

void InitScan(void);
void StartScan(void);

#endif // _scan_h_