#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include"FAT32Reader.h"
#include "NTFS.h"

int main(int argc, char** argv)
{

    BootsectorReader test('F');
    test.printDetailInformation();
    test.printBootsector();
    
    // NTFS
    wstring diskName;
    cout << "Enter Disk's Name: ";
    wcin >> diskName; //lúc test là gõ ổ 'C' vào đúng k?
    diskName = L"\\\\.\\" + diskName + L":";
    LPCWSTR drive = diskName.c_str();

    BYTE sector[512];
    int check = ReadSector(drive, 0, sector);

    if (check == 1)
    {
        Print_Sector(sector);

        Read_BPB(sector, drive);
    }
    else
    {
        cout << "Failed to read sector!" << endl;
        return 0;
    }
    
    return 0;
}
