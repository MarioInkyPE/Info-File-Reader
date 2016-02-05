#ifndef READFILE_H_INCLUDED
#define READFILE_H_INCLUDED
#include <cstdio>
#include <cstdlib>
#include <string>
#include <direct.h>
#include <iostream>
#include <regex>
#include <array>
#include <vector>
#include <limits>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>

#endif // READFILE_H_INCLUDED
#include <cstdio>
#include <cstdlib>
#include <string>
#include <direct.h>
#include <iostream>
#include <regex>
#include <array>
#include <vector>
#include <limits>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>



typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct IFHeader{
    u32 NumberOfItems;
    u32 NumberOfProperties;
    u32 ItemsOffset;
    u32 ItemLength;
};

struct IFMainProperty{
    u32 PropID;
    u16 unknown1;
    u16 unknown2;
    u16 PropertyOffset;
    u16 PropCatagory;
};

struct IFPropertyValue{
    IFMainProperty theResourceStuff;
    std::string ValueName;
    u32 ValueID;
    u8 rawValue[4];
    std::string valueType;
    char TableString[16];
    char InfoString[32];
    u32 IntegerLol;
    float FloatLol;
};

struct IFItem{
    std::vector<IFPropertyValue> TheProperties;
    u32 ItemPosition;
};

struct IFWhole{
    IFHeader header;
    std::vector<IFMainProperty> Nodes;
    std::vector<IFItem> ItemsLolz;
};

BOOL ReadInfoFile(IFWhole &Bobby, LPCTSTR pathFileName);
BOOL WriteInfoFile(IFWhole &Bobby, LPCTSTR pszFileName);
