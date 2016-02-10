#include <vector>
#pragma once
class DocumentReader
{
public:
	DocumentReader();
	~DocumentReader();
	
	
	//For Setting up Documented Names and information about a Property ID.
	struct DocumentItem {
		unsigned int HashID;
		std::string Name;
		char Description[512];
	};

	//To get the documentation.
	BOOL GetDocumentation(std::vector<DocumentReader::DocumentItem> &DocDest);

	//Match an ID given.
	int IDMatch(std::vector<DocumentReader::DocumentItem> &DocList, unsigned int IDHash);
};

