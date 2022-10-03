#pragma once

#include <iostream>
#include <sstream>
#include <windows.h>

using namespace std;

struct PartitionBootSector
{
    char OEM_ID[9];
    int Byte_Per_Sector;
    int Sector_Per_Cluster;
    int SectorPerTrack;
    int NumberOfHead;
    int StartClusterOfMFT;
    int MFT_entry_size;
    int VolumeSize;
};

void infoNTFS();