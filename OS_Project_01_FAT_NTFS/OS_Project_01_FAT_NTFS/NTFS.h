#pragma once

#include<iostream>
#include<cstdlib>
#include<stdio.h>
#include<vector>
#include<windows.h>
#include<iomanip>
#include<sstream>
#include<map>
#include<string>

using namespace std;

bool ReadSector(LPCWSTR drive, int readPoint, BYTE sector[512]);
void Print_Sector(BYTE sector[512]);
void Read_BPB(BYTE* sector, LPCWSTR disk);
