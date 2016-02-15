#include <vector>
#include <string>
#include "readFile.h"
#pragma once
class SearchItem
{
public:
	SearchItem();
	~SearchItem();
	BOOL FindAllProperties(std::string keyword, std::vector<int>& results, IFWhole fileToSearch);

};

