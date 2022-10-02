#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <clocale>
#include <cstdlib>
#include <wchar.h>
#include <string.h>
class BootsectorReader {
private:
	//Sector
	BYTE sector[512];
    bool success; 
	//First 36 bytes
	BYTE BS_jmpBoot[3];
	BYTE BS_OEMNAME[8];
	BYTE BPB_BytsPerSec[2];
	BYTE BPB_SecPerClus[1];
	BYTE BPB_RsvdSecCnt[2];
	BYTE BPB_NumFATs[1];
	BYTE BPB_RootEntCnt[2];
	BYTE BPB_TotSec16[2];
	BYTE BPB_Media[1];
	BYTE BPB_FATSz16[2];
	BYTE BPB_SecPerTrk[2];
	BYTE BPB_NumHeads[2];
	BYTE BPB_HiddSec[4];
	BYTE BPB_TotSec32[4];
	//FAT32 Structure Starting at Offset 36
	BYTE BPB_FATSz32[4];
	BYTE BPB_ExtFlags[2];
	BYTE BPB_FSVer[2];
	BYTE BPB_RootClus[4];
	BYTE BPB_FSInfo[2];
	BYTE BPB_BkBootSec[2];
	BYTE BPB_Reserved[12];
	BYTE BS_DrvNum[1];
	BYTE BS_Reserved1[1];
	BYTE BS_BootSig[1];
	BYTE BS_VolID[4];
	BYTE BS_VolLab[11];
	BYTE BS_FilSysType[8];
    BYTE BootstrapCode[420];
    BYTE Signature[2];
    wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
    {
        wchar_t* wString = new wchar_t[4096];
        MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
        return wString;
    }
    bool ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512])
    {
        int retCode = 0;
        DWORD bytesRead;
        HANDLE device = NULL;

        device = CreateFile(drive,    // Drive to open
            GENERIC_READ,           // Access mode
            FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
            NULL,                   // Security Descriptor
            OPEN_EXISTING,          // How to create
            0,                      // File attributes
            NULL);                  // Handle to template

        if (device == INVALID_HANDLE_VALUE) // Open Error
        {
            printf("CreateFile: %u\n", GetLastError());
            return false;
        }

        SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

        if (!ReadFile(device, sector, 512, &bytesRead, NULL))
        {
            printf("ReadFile: %u\n", GetLastError());
            return false;
        }
        else
        {
            printf("Success!\n");
            return true;
        }
    }
    void ReadInformation() {
        memcpy(BS_jmpBoot, &sector[0x0], sizeof(BS_jmpBoot));
        memcpy(BS_OEMNAME, &sector[0x3], sizeof(BS_OEMNAME));
        memcpy(BPB_BytsPerSec, &sector[0xB], sizeof(BPB_BytsPerSec));
        memcpy(BPB_SecPerClus, &sector[0xD], sizeof(BPB_SecPerClus));
        memcpy(BPB_RsvdSecCnt, &sector[0xE], sizeof(BPB_RsvdSecCnt));
        memcpy(BPB_NumFATs, &sector[0x10], sizeof(BPB_NumFATs));
        memcpy(BPB_RootEntCnt, &sector[0x11], sizeof(BPB_RootEntCnt));
        memcpy(BPB_TotSec16, &sector[0x13], sizeof(BPB_TotSec16));
        memcpy(BPB_Media, &sector[0x15], sizeof(BPB_Media));
        memcpy(BPB_FATSz16, &sector[0x16], sizeof(BPB_FATSz16));
        memcpy(BPB_SecPerTrk, &sector[0x18], sizeof(BPB_SecPerTrk));
        memcpy(BPB_NumHeads, &sector[0x1A], sizeof(BPB_NumHeads)); 
        memcpy(BPB_HiddSec, &sector[0x1C], sizeof(BPB_HiddSec));
        memcpy(BPB_TotSec32, &sector[0x20], sizeof(BPB_TotSec32));
        memcpy(BPB_FATSz32, &sector[0x24], sizeof(BPB_FATSz32));
        memcpy(BPB_ExtFlags, &sector[0x28], sizeof(BPB_ExtFlags));
        memcpy(BPB_FSVer, &sector[0x2A], sizeof(BPB_FSVer));
        memcpy(BPB_RootClus, &sector[0x2C], sizeof(BPB_RootClus));
        memcpy(BPB_FSInfo, &sector[0x30], sizeof(BPB_FSInfo));
        memcpy(BPB_BkBootSec, &sector[0x32], sizeof(BPB_BkBootSec));
        memcpy(BPB_Reserved, &sector[0x34], sizeof(BPB_Reserved));
        memcpy(BS_DrvNum, &sector[0x40], sizeof(BS_DrvNum));
        memcpy(BS_Reserved1, &sector[0x41], sizeof(BS_Reserved1));
        memcpy(BS_BootSig, &sector[0x42], sizeof(BS_BootSig));
        memcpy(BS_VolID, &sector[0x43], sizeof(BS_VolID));
        memcpy(BS_VolLab, &sector[0x47], sizeof(BS_VolLab));
        memcpy(BS_FilSysType, &sector[0x52], sizeof(BS_FilSysType));
        memcpy(BootstrapCode, &sector[0x5A], sizeof(BootstrapCode));
        memcpy(Signature, &sector[0x1FE], sizeof(Signature));
    }
public:
    BootsectorReader(char c) {
        std::string path = "\\\\.\\?:";
        path[4] = c;
        const char* fixedPath = path.c_str();
        wchar_t* diskPath = convertCharArrayToLPCWSTR(fixedPath);
        LPWSTR input = diskPath;
        success = ReadSector(input, 0, sector);
        if (success) {
            ReadInformation();
        }
    }
    std::string getJMPinstruction() {
        std::string s;
        for (int i = 0; i < sizeof(BS_jmpBoot); i++) {
            if (i != 0) {
                s += " ";
            }
            char temp[3];
            sprintf(temp, "%02hhX", BS_jmpBoot[i]);
            s += temp;
        }
        return s;
    }
    std::string getOEMID() {
        std::string s;
        for (int i = 0; i < sizeof(BS_OEMNAME); i++) {
            s.push_back(BS_OEMNAME[i]);
        }
        return s;
    }
    unsigned long long getBytesPerSector() {
        unsigned long long res = 0;
        int n = (int)sizeof(BPB_BytsPerSec);
        for (int i = n - 1; i >= 0; i--) {
            if (i == 0) {
                res += (unsigned long long)BPB_BytsPerSec[i];
            }
            else {
                res += (unsigned long long)BPB_BytsPerSec[i] * 256;
            }
        }
        return res;
    }
    unsigned long long getSectorsPerCluster() {
        unsigned long long res = (unsigned long long)BPB_SecPerClus[0];
        return res;
    }
    unsigned long long getReservedSectors() {
        unsigned long long res = 0;
        int n = (int)sizeof(BPB_RsvdSecCnt);
        for (int i = n - 1; i >= 0; i--) {
            if (i == 0) {
                res += (unsigned long long)BPB_RsvdSecCnt[i];
            }
            else {
                res += (unsigned long long)BPB_RsvdSecCnt[i] * 256;
            }
        }
        return res;
    }
    unsigned long long getNumberOfFATS() {
        unsigned long long res = (unsigned long long)BPB_NumFATs[0];
        return res;
    }
    std::string getMediaDescriptor() {
        std::string s;
        char temp[5];
        sprintf(temp, "0x%02hhX", BPB_Media[0]);
        s += temp;
        return s;
    }
    unsigned long long getSectorsPerTrack() {
        unsigned long long res = 0;
        int n = (int)sizeof(BPB_SecPerTrk);
        for (int i = n - 1; i >= 0; i--) {
            if (i == 0) {
                res += (unsigned long long)BPB_SecPerTrk[i];
            }
            else {
                res += (unsigned long long)BPB_SecPerTrk[i] * 256;
            }
        }
        return res;
    }
    unsigned long long getNumberOfHeads() {
        unsigned long long res = 0;
        int n = (int)sizeof(BPB_NumHeads);
        for (int i = n - 1; i >= 0; i--) {
            if (i == 0) {
                res += (unsigned long long)BPB_NumHeads[i];
            }
            else {
                res += (unsigned long long)BPB_NumHeads[i] * 256;
            }
        }
        return res;
    }
    unsigned long long getHiddenSectors() {
        unsigned long long res = 0;
        int n = (int)sizeof(BPB_HiddSec);
        for (int i = n - 1; i >= 0; i--) {
            if (i == 0) {
                res += (unsigned long long)BPB_HiddSec[i];
            }
            else {
                res += (unsigned long long)BPB_HiddSec[i] * 256;
            }
        }
        return res;
    }
    unsigned long long getTotalSectors() {
        unsigned long long res = 0;
        int n = (int)sizeof(BPB_TotSec32);
        unsigned long long k = 1;  //Handle multiply with 16
        for (int i = 0; i < n-1; i++) {
            k *= 256;
        }
        for (int i = n - 1; i >= 0; i--) {
            if (i == 0) {
                res += (unsigned long long)BPB_TotSec32[i];
            }
            else {
                res += (unsigned long long)BPB_TotSec32[i] * k;
                k /= 256;
            }
        }
        return res;
    }
    unsigned long long getSectorsPerFAT() {
        unsigned long long res = 0;
        int n = (int)sizeof(BPB_FATSz32);
        unsigned long long k = 1;  //Handle multiply with 16
        for (int i = 0; i < n - 1; i++) {
            k *= 256;
        }
        for (int i = n - 1; i >= 0; i--) {
            if (i == 0) {
                res += (unsigned long long)BPB_FATSz32[i];
            }
            else {
                res += (unsigned long long)BPB_FATSz32[i] * k;
                k /= 256;
            }
        }
        return res;
    }
    unsigned long long getExtendedFlags() {
        unsigned long long res = 0;
        int n = (int)sizeof(BPB_ExtFlags);
        for (int i = n - 1; i >= 0; i--) {
            if (i == 0) {
                res += (unsigned long long)BPB_ExtFlags[i];
            }
            else {
                res += (unsigned long long)BPB_ExtFlags[i] * 256;
            }
        }
        return res;
    }
    unsigned long long getVersion() {
        unsigned long long res = 0;
        int n = (int)sizeof(BPB_FSVer);
        for (int i = n - 1; i >= 0; i--) {
            if (i == 0) {
                res += (unsigned long long)BPB_FSVer[i];
            }
            else {
                res += (unsigned long long)BPB_FSVer[i] * 256;
            }
        }
        return res;
    }
    unsigned long long getRootCluster() {
        unsigned long long res = 0;
        int n = (int)sizeof(BPB_RootClus);
        unsigned long long k = 1;  //Handle multiply with 16
        for (int i = 0; i < n - 1; i++) {
            k *= 256;
        }
        for (int i = n - 1; i >= 0; i--) {
            if (i == 0) {
                res += (unsigned long long)BPB_RootClus[i];
            }
            else {
                res += (unsigned long long)BPB_RootClus[i] * k;
                k /= 256;
            }
        }
        return res;
    }
    unsigned long long getSystemInformationSector() {
        unsigned long long res = 0;
        int n = (int)sizeof(BPB_FSInfo);
        for (int i = n - 1; i >= 0; i--) {
            if (i == 0) {
                res += (unsigned long long)BPB_FSInfo[i];
            }
            else {
                res += (unsigned long long)BPB_FSInfo[i] * 256;
            }
        }
        return res;
    }
    unsigned long long getBackupBootSector() {
        unsigned long long res = 0;
        int n = (int)sizeof(BPB_BkBootSec);
        for (int i = n - 1; i >= 0; i--) {
            if (i == 0) {
                res += (unsigned long long)BPB_BkBootSec[i];
            }
            else {
                res += (unsigned long long)BPB_BkBootSec[i] * 256;
            }
        }
        return res;
    }
    std::string getReserved() {
        std::string s;
        for (int i = 0; i < sizeof(BPB_Reserved); i++) {
            if (i != 0) {
                s += " ";
            }
            char temp[3];
            sprintf(temp, "%02hhX", BPB_Reserved[i]);
            s += temp;
        }
        return s;
    }
    unsigned long long getPhysicalDrive() {
        return (unsigned long long)BS_DrvNum[0];
    }
    unsigned long long getReserved1() {
        return (unsigned long long)BS_Reserved1[0];
    }
    unsigned long long getExtendedSignature() {
        return (unsigned long long)BS_BootSig[0];
    }
    std::string getSerialNumber() {
        std::string s;
        for (int i = 0; i < sizeof(BS_VolID); i++) {
            if (i != 0) {
                s += " ";
            }
            char temp[3];
            sprintf(temp, "%02hhX", BS_VolID[i]);
            s += temp;
        }
        return s;
    }
    std::string getVolumeLabel() {
        std::string s;
        for (int i = 0; i < sizeof(BS_VolLab); i++) {
            s.push_back(BS_VolLab[i]);
        }
        return s;
    }
    std::string getFileSystem() {
        std::string s;
        for (int i = 0; i < sizeof(BS_FilSysType); i++) {
            s.push_back(BS_FilSysType[i]);
        }
        return s;
    }
    std::string getBootstrapCode() {
        std::string s;
        for (int i = 0; i < sizeof(BootstrapCode); i++) {
            if (i != 0) {
                s += " ";
            }
            char temp[3];
            sprintf(temp, "%02hhX", BootstrapCode[i]);
            s += temp;
        }
        return s;
    }
    std::string getSignature() {
        std::string s;
        for (int i = 0; i < sizeof(Signature); i++) {
            if (i != 0) {
                s += " ";
            }
            char temp[3];
            sprintf(temp, "%02hhX", Signature[i]);
            s += temp;
        }
        return s;
    }
    void printDetailInformation() {
        std::cout << "JMP instruction: " << getJMPinstruction() << "\n";
        std::cout << "OEM ID: " << getOEMID() << "\n";
        std::cout << "BIOS Parameter Block" << "\n";
        std::cout << "Bytes per sector: " << getBytesPerSector() << "\n";
        std::cout << "Sectors per cluster: " << getSectorsPerCluster() << "\n";
        std::cout << "Reserved sectors: " << getReservedSectors() << "\n";
        std::cout << "Number of FATS: " << getNumberOfFATS() << "\n";
        std::cout << "Media descriptor: " << getMediaDescriptor() << "\n";
        std::cout << "Sectors per track: " << getSectorsPerTrack() << "\n";
        std::cout << "Number of heads: " << getNumberOfHeads() << "\n";
        std::cout << "Hidden sectors: " << getHiddenSectors() << "\n";
        std::cout << "Total sectors: " << getTotalSectors() << "\n";
        std::cout << "Sectors per FAT: " << getSectorsPerFAT() << "\n";
        std::cout << "Extended flags: " << getExtendedFlags() << "\n";
        std::cout << "Version: " << getVersion() << "\n";
        std::cout << "Root cluster: " << getRootCluster() << "\n";
        std::cout << "System information sector: " << getSystemInformationSector() << "\n";
        std::cout << "Backup Boot sector: " << getBackupBootSector() << "\n";
        std::cout << "(Reserved): " << getReserved() << "\n";
        std::cout << "Extended BIOS Parameter Block" << "\n";
        std::cout << "Physical drive: " << getPhysicalDrive() << "\n";
        std::cout << "Reserved: " << getReserved1() << "\n";
        std::cout << "Extended signature: " << getExtendedSignature() << "\n";
        std::cout << "Serial number: " << getSerialNumber() << "\n";
        std::cout << "Volume label: " << getVolumeLabel() << "\n";
        std::cout << "File system: " << getFileSystem() << "\n";
        std::cout << "Bootstrap code: " << getBootstrapCode() << "\n";
        std::cout << "Signature (55 AA): " << getSignature() << "\n";
    }
    void printBootsector() {
        for (int i = 0; i < 512; i++)
            {
                // create a row after every 16 
                // columns so that display 
                // looks good 
                if (0 == i % 16){
                    printf("\n");
               }
                BYTE b = sector[i];
                //printf("%c", isascii(b) ? b : '.');
                printf("%02hhX ", b);
            }
    }
};