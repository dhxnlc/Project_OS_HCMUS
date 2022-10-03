#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include"FAT32Reader.h"
int main(int argc, char** argv)
{

    BootsectorReader test('F');
    test.printDetailInformation();
    test.printBootsector();
    return 0;
}