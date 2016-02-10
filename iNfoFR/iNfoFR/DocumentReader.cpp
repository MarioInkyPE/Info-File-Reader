#include "stdafx.h"
#include "DocumentReader.h"
#include <stdio.h>
#include <vector>
#include <string>

DocumentReader::DocumentReader()
{
}


DocumentReader::~DocumentReader()
{
}



BOOL DocumentReader::GetDocumentation(std::vector<DocumentReader::DocumentItem> &DocDest)
{
	FILE * DocumentFile;
	DocumentFile = fopen("InfoFileDocumentation.dat", "r");
	if (!DocumentFile == NULL) {
		std::vector<DocumentReader::DocumentItem> DocumentedFiles;
		char tempChar;	// Temporary Character to insert & check
		do {	// For whole file
			DocumentReader::DocumentItem tempDocItem;	// Temporary DocumentItem for adding later.
			int place = 0;	 //What value to overwrite
			do {	// For each line
				
				tempChar = fgetc(DocumentFile);
				if (tempChar == '(' && place <= 0)	// For first u32 ints
				{
					tempChar = fgetc(DocumentFile);
					std::string tempString = "";
					while (tempChar != ')' && tempChar != EOF && tempChar != '\n') {	//Get Numbers until hiting ')'
						if (tempChar >= '0' && tempChar <= '9') {
							tempString += tempChar;
						}
						tempChar = fgetc(DocumentFile);
					}
					tempDocItem.HashID = std::stoul(tempString);
					place++;
				}
				if (tempChar == '"' && place == 1) {	//For getting the name
					tempChar = fgetc(DocumentFile);
					std::string tempTCHAR = "";
					do { //Get Name string
						tempTCHAR.push_back(tempChar);
						tempChar = fgetc(DocumentFile);
					} while (tempChar != '"' && tempChar != EOF && tempChar != '\n');
					tempTCHAR.push_back('\0');
					tempDocItem.Name = tempTCHAR;
					place++;
				}
				if (tempChar == '"' && place == 2) {	//For getting the description
					tempChar = fgetc(DocumentFile);
					std::string tempTCHAR = "";
					while (tempChar != '"' && tempChar != EOF && tempChar != '\n') { //Get Description string
						tempTCHAR.push_back(tempChar);
						tempChar = fgetc(DocumentFile);
					}
					tempTCHAR.push_back('\0');
					_snprintf(tempDocItem.Description, 512, "%hs", tempTCHAR.c_str());
					place++;
				}
			} while (tempChar != '\n' && tempChar != EOF);
			DocumentedFiles.push_back(tempDocItem);

		} while (tempChar != EOF);
		DocDest = DocumentedFiles;
		fclose(DocumentFile);
		return TRUE;
	}
	return FALSE;
}

int DocumentReader::IDMatch(std::vector<DocumentReader::DocumentItem>& DocList, unsigned int IDHash)
{
	for (unsigned int i = 0; i < DocList.size(); ++i) {
		if (DocList[i].HashID == IDHash) {
			return i;
		}
	}
	return -1;
}

