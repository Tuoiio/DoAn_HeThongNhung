#ifndef _KIEUGIAT_H_
#define _KIEUGIAT_H_

#include <stdint.h>

#pragma pack(1)
 typedef struct{
    uint8_t Weight_Max;
	uint8_t Switch_Temp;
    uint8_t Set_Temperature; //[6]; // 0, 20, 30, 40, 60, 95
	 uint8_t Switch_Spin;
    uint16_t Set_Spin; // [6]; //0, 400, 800, 1000, 1200, 1400
}KieuGiat;
 //ChiVat, TurboWash, DoHonHop, DoTraiGiuong, Fuzzy, *pKieuGiat;
#pragma pack()

extern KieuGiat VaiBong;
extern KieuGiat DoHonHop;
extern KieuGiat DoTraiGiuong;
extern KieuGiat GiatNhe;
extern KieuGiat DoTheThao;
 extern KieuGiat *pKieuGiat;
#endif
