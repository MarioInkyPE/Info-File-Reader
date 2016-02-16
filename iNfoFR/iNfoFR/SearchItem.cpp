#include "stdafx.h"
#include "SearchItem.h"


SearchItem::SearchItem()
{
}


SearchItem::~SearchItem()
{
}

BOOL isFloat(std::string floatString) {
	for (int i = 0; i < floatString.length(); ++i) {
		if (!((floatString[i] >= '0' && floatString[i] <= '9') || floatString[i] == '.' || (floatString[i] == '-' && i == 0))) {
			return FALSE;
		}
	}
	return TRUE;
}

BOOL isInt(std::string intString) {
	for (int i = 0; i < intString.length(); ++i) {
		if (!(intString[i] >= '0' && intString[i] <= '9')) {
			return FALSE;
		}
	}
	return TRUE;
}

BOOL SearchItem::FindAllProperties(std::string keyword, std::vector<int> &results, IFWhole fileToSearch)
{
	BOOL isGood = FALSE;
	for (int i = 0; i < fileToSearch.header.NumberOfItems; ++i) {
		BOOL toAdd = FALSE;
		for (int j = 0; j < fileToSearch.header.NumberOfProperties; ++j) {
			IFPropertyValue tempPropLook = fileToSearch.ItemsLolz[i].TheProperties[j];

			if (tempPropLook.valueType == "s32") {
				std::size_t results2 = std::string(tempPropLook.InfoString).find(keyword);
				if (results2 != std::string::npos) {
					toAdd = TRUE;
				}
			}
			else if (tempPropLook.valueType == "s16") {
				std::size_t results2 = std::string(tempPropLook.TableString).find(keyword);
				if (results2 != std::string::npos) {
					toAdd = TRUE;
				}
			}
			else if (tempPropLook.valueType == "f4") {
				if (isFloat(keyword)) {
					if (std::stof(keyword) == tempPropLook.FloatLol) {
						toAdd = TRUE;
					}
				}
			}
			else if (tempPropLook.valueType == "i4") {
				if (isInt(keyword)) {
					if (std::stoul(keyword) == tempPropLook.IntegerLol) {
						toAdd = TRUE;
					}
				}
			}
		}
		if (toAdd == TRUE) {
			isGood = TRUE;
			results.push_back(i);
		}
		
	}
	return isGood;
}

BOOL SearchItem::FindGroupProperties(std::string keyword, std::vector<int>& results, IFWhole fileToSearch, std::string groupType)
{
	BOOL isGood = FALSE;
	for (int i = 0; i < fileToSearch.header.NumberOfItems; ++i) {
		BOOL toAdd = FALSE;
		for (int j = 0; j < fileToSearch.header.NumberOfProperties; ++j) {
			IFPropertyValue tempPropLook = fileToSearch.ItemsLolz[i].TheProperties[j];

			if (tempPropLook.valueType == "s32" && groupType == "s32") {
				std::size_t results2 = std::string(tempPropLook.InfoString).find(keyword);
				if (results2 != std::string::npos) {
					toAdd = TRUE;
				}
			}
			else if (tempPropLook.valueType == "s16" && groupType == "s16") {
				std::size_t results2 = std::string(tempPropLook.TableString).find(keyword);
				if (results2 != std::string::npos) {
					toAdd = TRUE;
				}
			}
			else if (tempPropLook.valueType == "f4" && groupType == "f4") {
				if (isFloat(keyword)) {
					if (std::stof(keyword) == tempPropLook.FloatLol) {
						toAdd = TRUE;
					}
				}
			}
			else if (tempPropLook.valueType == "i4" && groupType == "i4") {
				if (isInt(keyword)) {
					if (std::stoul(keyword) == tempPropLook.IntegerLol) {
						toAdd = TRUE;
					}
				}
			}
		}
		if (toAdd == TRUE) {
			isGood = TRUE;
			results.push_back(i);
		}

	}
	return isGood;
}

BOOL SearchItem::FindSingleProperty(std::string keyword, std::vector<int>& results, IFWhole fileToSearch, u32 propID)
{
	BOOL isGood = FALSE;
	for (int i = 0; i < fileToSearch.header.NumberOfItems; ++i) {
		BOOL toAdd = FALSE;
		for (int j = 0; j < fileToSearch.header.NumberOfProperties; ++j) {
			IFPropertyValue tempPropLook = fileToSearch.ItemsLolz[i].TheProperties[j];
			if (tempPropLook.ValueID == propID) {
				if (tempPropLook.valueType == "s32") {
					std::size_t results2 = std::string(tempPropLook.InfoString).find(keyword);
					if (results2 != std::string::npos) {
						toAdd = TRUE;
					}
				}
				else if (tempPropLook.valueType == "s16") {
					std::size_t results2 = std::string(tempPropLook.TableString).find(keyword);
					if (results2 != std::string::npos) {
						toAdd = TRUE;
					}
				}
				else if (tempPropLook.valueType == "f4") {
					if (isFloat(keyword)) {
						if (std::stof(keyword) == tempPropLook.FloatLol) {
							toAdd = TRUE;
						}
					}
				}
				else if (tempPropLook.valueType == "i4") {
					if (isInt(keyword)) {
						if (std::stoul(keyword) == tempPropLook.IntegerLol) {
							toAdd = TRUE;
						}
					}
				}
			}
		}
		if (toAdd == TRUE) {
			isGood = TRUE;
			results.push_back(i);
		}

	}
	return isGood;
}
