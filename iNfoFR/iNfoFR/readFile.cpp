

#include "stdafx.h"
#include <memory.h>
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "readFile.h"
#include "DocumentReader.h"
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
#include <string.h>



using namespace std;


int minl(int a, int b)
{
	return a < b ? a : b;
}

int maxl(int a, int b)
{
	return a > b ? a : b;
}

#pragma pack(push, 1)


#pragma pack(pop)

std::string TstrToStdStr(LPCTSTR psz)
{
	std::string s;
	if (psz == NULL) return s;
	if (sizeof(TCHAR) == sizeof(wchar_t))
	{
		std::wstring ws = psz;
		int wlen = (int)ws.length();
		// get needed space
		int slen = (int)wcstombs(NULL, &ws[0], 2 * wlen);

		if (slen > 0)
		{
			s.resize(slen, '\0');
			wcstombs(&s[0], &ws[0], slen);
		}
	}
	else
	{
		//s = psz;
	}
	return s;
}

void toWORD(u16& w)
{
	u8 w1 = w & 0xFF;
	u8 w2 = w >> 8;
	w = (w1 << 8) | w2;
}

void toDWORD(u32& d)
{
	u8 w1 = d & 0xFF;
	u8 w2 = (d >> 8) & 0xFF;
	u8 w3 = (d >> 16) & 0xFF;
	u8 w4 = d >> 24;
	d = (w1 << 24) | (w2 << 16) | (w3 << 8) | w4;
}

std::string convert(LPCSTR str) {
	return std::string(str);
}


void swapFloat(float * floatpointer) {
	u8 mem;
	u8 * first = (u8*)floatpointer;
	mem = *first;
	*first = *(first + 0x03);
	*(first + 0x03) = mem;
	first++;
	mem = *first;
	*first = *(first + 0x01);
	*(first + 0x01) = mem;
}

//Reads the Info and Table files. If it is named 'info' at the end,
//it uses the 32-byte string. Else, it is 16-Byte.
BOOL ReadInfoFile(IFWhole &Bobby, LPCTSTR pathFileName, std::vector<DocumentReader::DocumentItem>& DocInit)
{
    HANDLE hFile;
    BOOL bSuccess = FALSE;
    IFWhole tempBobby;
    OVERLAPPED overlapped;
    DWORD yoBobby;
    ZeroMemory(&overlapped, sizeof(OVERLAPPED));

    hFile = CreateFile(pathFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, 0 , NULL);
    if(hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwFileSize;

        dwFileSize = GetFileSize(hFile, NULL);
        if(dwFileSize != 0xFFFFFFFF)
        {
                IFHeader tempPoops;
                BOOL isHeadRead = ReadFile(hFile, &tempPoops, 16, &yoBobby, NULL);
                if(isHeadRead == TRUE){

                    toDWORD(tempPoops.NumberOfItems);
                    toDWORD(tempPoops.NumberOfProperties);
                    toDWORD(tempPoops.ItemsOffset);
                    toDWORD(tempPoops.ItemLength);

                    tempBobby.header = tempPoops;

                    for (u32 i=0; i<tempBobby.header.NumberOfProperties; ++i){
                        IFMainProperty poopScoop;
                        BOOL isNodeRead = ReadFile(hFile, &poopScoop, 12, &yoBobby, NULL);
                        if(isNodeRead == FALSE){
                            CloseHandle(hFile);
                            return FALSE;
                        }
                        toWORD(poopScoop.PropCatagory);
                        toWORD(poopScoop.PropertyOffset);
                        toWORD(poopScoop.unknown1);
                        toWORD(poopScoop.unknown2);
                        toDWORD(poopScoop.PropID);
                        tempBobby.Nodes.push_back(poopScoop);
                    }
                    for (u32 i=0; i<tempBobby.header.NumberOfItems; ++i){
                        IFItem UsingFred;
                        UsingFred.ItemPosition = tempBobby.header.ItemsOffset + (tempBobby.header.ItemLength * i);
                        for (u32 j=0; j<tempBobby.header.NumberOfProperties; ++j){
                            IFPropertyValue someProperty;
                            IFMainProperty NodeLol = tempBobby.Nodes[j];
                            overlapped.Offset = (int)UsingFred.ItemPosition + NodeLol.PropertyOffset;
                            if(NodeLol.PropCatagory == 1){
                                std::string endString = TstrToStdStr(pathFileName);
								
                                if(endString.substr(endString.length() - 4) == "info"){
                                    someProperty.valueType = "s32";
                                    ReadFile(hFile, &someProperty.InfoString, 32, &yoBobby, &overlapped);
                                }
                                else{
                                    someProperty.valueType = "s16";
                                    ReadFile(hFile, &someProperty.TableString, 16, &yoBobby, &overlapped);
                                }
                            }
                            else if(NodeLol.PropCatagory == 2){
                                someProperty.valueType = "f4";
                                ReadFile(hFile, &someProperty.rawValue, 4, &yoBobby, &overlapped);
                                u8 * ptr = someProperty.rawValue;
                                std::reverse(ptr, ptr + 4);
                                someProperty.FloatLol = *reinterpret_cast<float*>(ptr);
                                std::reverse(ptr, ptr + 4);
                            }
                            else{
                                someProperty.valueType = "i4";
                                ReadFile(hFile, &someProperty.IntegerLol, 4, &yoBobby, &overlapped);
                                toDWORD(someProperty.IntegerLol);
                            }
							if (!(NodeLol.unknown1 == 0x0000 && NodeLol.unknown2 == 0x0000) && !(NodeLol.unknown2 == 0xFFFF && NodeLol.unknown1 == 0xFFFF)) {
								someProperty.isLinked = true;
							}
							DocumentReader DocFunc = DocumentReader::DocumentReader();
                            someProperty.ValueID = NodeLol.PropID;
							if (DocInit.size() >= 1) {
								int GoodDoc = DocFunc.IDMatch(DocInit, NodeLol.PropID);
								if (GoodDoc > -1) {
									someProperty.ValueName = DocInit[GoodDoc].Name;
								}
								else {
									someProperty.ValueName = "(Undocumented)";
								}
							}
                            someProperty.theResourceStuff = NodeLol;
                            UsingFred.TheProperties.push_back(someProperty);
                        }
                        tempBobby.ItemsLolz.push_back(UsingFred);
                    }
                    bSuccess = TRUE;
                    Bobby = tempBobby;
                }
        }
        CloseHandle(hFile);

    }
    return bSuccess;
}


//lololl
BOOL WriteInfoFile(IFWhole &Bobby, LPCTSTR pszFileName)
{
    HANDLE hFile;
    BOOL bSuccess = FALSE;
    IFWhole tempBobby;
    OVERLAPPED overlapped;
    ZeroMemory(&overlapped, sizeof(OVERLAPPED));

    hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile != INVALID_HANDLE_VALUE)
    {
        tempBobby = Bobby;
        
                    DWORD dwWritten;

                    toDWORD(tempBobby.header.NumberOfItems);
                    toDWORD(tempBobby.header.NumberOfProperties);
                    toDWORD(tempBobby.header.ItemsOffset);
                    toDWORD(tempBobby.header.ItemLength);
                    WriteFile(hFile, &tempBobby.header, 16, &dwWritten, NULL);
                    for(u32 i=0; i<Bobby.header.NumberOfProperties; ++i){
                        IFMainProperty poopScoop = tempBobby.Nodes[i];

                        toWORD(poopScoop.PropCatagory);
                        toWORD(poopScoop.PropertyOffset);
                        toWORD(poopScoop.unknown1);
                        toWORD(poopScoop.unknown2);
                        toDWORD(poopScoop.PropID);
                        WriteFile(hFile, &poopScoop, 12, &dwWritten, NULL);
                    }
                    for(u32 i=0; i<Bobby.header.NumberOfItems; ++i){
                        IFItem UsingFred = tempBobby.ItemsLolz[i];
                        for(u32 j=0; j<Bobby.header.NumberOfProperties; ++j){
                            IFPropertyValue someProperty = UsingFred.TheProperties[j];
                            IFMainProperty NodeLol = Bobby.Nodes[j];
                            overlapped.Offset = (int)(Bobby.header.ItemsOffset + (i * Bobby.header.ItemLength)) + NodeLol.PropertyOffset;
                            if(NodeLol.PropCatagory == 1){
                               
                                if(someProperty.valueType == "s32"){
                                    WriteFile(hFile, &someProperty.InfoString, 32, &dwWritten, &overlapped);
                                }
                                else{
                                    WriteFile(hFile, &someProperty.TableString, 16, &dwWritten, &overlapped);  
                                }
                            }
                            else if(NodeLol.PropCatagory == 2){
                              
                               swapFloat(&someProperty.FloatLol);
                               WriteFile(hFile, &someProperty.FloatLol, 4, &dwWritten, &overlapped);
                            }
                            else{
                                toDWORD(someProperty.IntegerLol);
                                WriteFile(hFile, &someProperty.IntegerLol, 4, &dwWritten, &overlapped);
                            }
                        }
                    }
                    DWORD dwFileSize;
                    dwFileSize = GetFileSize(hFile, NULL);
                    if((dwFileSize % 32) != 0){
                        for(DWORD i=0; i<(32 - (dwFileSize % 32)); ++i){
                            u8 aDoot = 0x40;
                            WriteFile(hFile, &aDoot, 1, &dwWritten, NULL);
                        }
                    }
                    
                        bSuccess = TRUE;
        CloseHandle(hFile);
    }
    return bSuccess;
}
