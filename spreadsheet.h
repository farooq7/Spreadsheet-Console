#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

typedef std::map<std::pair<int, char>, std::string> spreadsheetMap;
 
class Spreadsheet {

private:
	std::string filename;
	spreadsheetMap originalMap;

public:
	Spreadsheet(const std::string& file); 
	~Spreadsheet();

	void read();
	void write(const spreadsheetMap& output) const;
	const spreadsheetMap& getMap() const;
};

#endif