#include "spreadsheet.h"

Spreadsheet::Spreadsheet(const std::string& file): filename(file), originalMap() {
	read();
}
Spreadsheet::~Spreadsheet() {

}

void Spreadsheet::read() {

	std::ifstream inputFile(filename);

	if (inputFile) {
		std::string temp, type;
		int row = 1;
		char col = 'A';

		while (!inputFile.eof()) {
			std::getline(inputFile, type);
			std::istringstream ss(type);

			while (getline(ss, temp, '\t')) {
				originalMap[std::make_pair(row, col)] = temp;
				col++;
			}

			row++;
			col = 'A';
		}
	}
	else {
		std::cout << "Error: Could not open file\n";
		exit(0);
	}
}

void Spreadsheet::write(const spreadsheetMap& output) const {

	int j = 1;

	for (auto it = output.begin(); it != output.end(); it++) {

		if (j == it->first.first)
			std::cout << it->second << "\t";
		else {
			std::cout << "\n" << it->second << "\t";
			j++;
		}
	}

	std::cout << "\n\n";
}

const spreadsheetMap& Spreadsheet::getMap() const {
	return originalMap;
}