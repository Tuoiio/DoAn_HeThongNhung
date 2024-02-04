#include "KieuGiat.h"



KieuGiat VaiBong = {
	.Weight_Max = 2,
	.Switch_Temp = 1, // Mo may nen hien thi o 40 do
	.Set_Temperature = 20,
	.Switch_Spin = 1,
	.Set_Spin = 400
};


KieuGiat DoHonHop = {
	.Weight_Max = 2,
	.Switch_Temp = 2, // Mo may nen hien thi o 40 do
	.Set_Temperature = 30,
	.Switch_Spin = 2,
	.Set_Spin = 800
};

KieuGiat DoTraiGiuong = {
	.Weight_Max = 3,
	.Switch_Temp = 3, // Mo may nen hien thi o 40 do
	.Set_Temperature = 40,
	.Switch_Spin = 3,
	.Set_Spin = 1000
};

KieuGiat GiatNhe = {
	.Weight_Max = 2,
	.Switch_Temp = 4, // Mo may nen hien thi o 40 do
	.Set_Temperature = 60,
	.Switch_Spin = 4,
	.Set_Spin = 1200
};

KieuGiat DoTheThao = {
	.Weight_Max = 4,
	.Switch_Temp = 2, // Mo may nen hien thi o 40 do
	.Set_Temperature = 30,
	.Switch_Spin = 5,
	.Set_Spin = 1200
};

KieuGiat *pKieuGiat;
