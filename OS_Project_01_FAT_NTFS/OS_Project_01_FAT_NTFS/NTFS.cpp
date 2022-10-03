#include "NTFS.h"

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

string convertByteToHex(BYTE byte) {
    stringstream sstream;
    sstream << std::hex << (int)byte;
    string result = sstream.str();
    if (result.length() == 1)
        result = "0" + result;
    return result;
}

int convertHexToDec(string hexVal)
{
    int len = hexVal.size();
    int base = 1;
    int dec_val = 0;

    for (int i = len - 1; i >= 0; i--) {

        if (hexVal[i] >= '0' && hexVal[i] <= '9') {
            dec_val += (int(hexVal[i]) - 48) * base;
            base = base * 16;
        }

        else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
            dec_val += (int(hexVal[i]) - 55) * base;
            base = base * 16;
        }
        else if (hexVal[i] >= 'a' && hexVal[i] <= 'f') {
            dec_val += (int(hexVal[i]) - 87) * base;
            base = base * 16;
        }

    }
    return dec_val;
}

void infoNTFS()
{
    PartitionBootSector PBS;
    BYTE* sector = new BYTE[512];
    string* HSector = new string[512]; //Mảng lưu thông tin sector dạng hexa
    string temp;

    ReadSector(L"\\\\.\\D:", 0, sector);

    //Chuyển mảng sector thành mảng hex lưu trong HSector
    for (int i = 0; i < 512; i++) {
        HSector[i] = convertByteToHex(sector[i]);
    }

    cout << "Doc thong tin phan vung NTFS" << endl;

    //OS Version
    for (int i = 3; i < 11; i++) {
        PBS.OEM_ID[i - 3] = sector[i];
    }
    PBS.OEM_ID[8] = '\0';
    cout << "+ OEM/Name of management system: " << PBS.OEM_ID << endl;

    //Số byte trên một sector
    temp = HSector[12] + HSector[11];
    PBS.Byte_Per_Sector = convertHexToDec(temp);
    cout << "+ So byte tren moi sector la: 0x" << temp << " = " << PBS.Byte_Per_Sector << " byte" << endl;

    //Số sector trên một cluster
    temp = HSector[13];
    PBS.Sector_Per_Cluster = convertHexToDec(temp);
    cout << "+ So sector tren moi cluster la: 0x" << temp << " = " << PBS.Sector_Per_Cluster << " sector" << endl;

    //Số sector trên một track
    temp = HSector[25] + HSector[24];
    PBS.SectorPerTrack = convertHexToDec(temp);
    cout << "+ So sector tren mot track la: 0x" << temp << " = " << PBS.SectorPerTrack << " sector" << endl;

    //Số mặt đĩa
    temp = HSector[27] + HSector[26];
    PBS.NumberOfHead = convertHexToDec(temp);
    cout << "+ So head(side) la: 0x" << temp << " = " << PBS.NumberOfHead << endl;

    //Cluster bắt đầu của MFT
    for (int i = 54; i >= 48; i--)
        temp = temp + HSector[i];
    PBS.StartClusterOfMFT = convertHexToDec(temp);
    cout << "+ Cluster bat dau MFT la: 0x" << temp << " = " << PBS.StartClusterOfMFT << endl;

    //Kích thước mỗi bản ghi trong MFT
    temp = HSector[64];
    PBS.MFT_entry_size = convertHexToDec(temp);
    cout << "+ Kich thuoc moi ban ghi trong MFT la: 0x" << temp << " = " << PBS.MFT_entry_size << " byte" << endl;

    //Tổng số sector của ổ đĩa
    for (int i = 47; i >= 40; i--)
        temp = temp + HSector[i];
    PBS.VolumeSize = convertHexToDec(temp);
    cout << "+ Tong so sector la: 0x" << temp << " = " << PBS.VolumeSize << endl;

    delete[] sector;
    delete[] HSector;
}