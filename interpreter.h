#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "spreadsheet.h"
#include <stack>
#include <set>

class Interpreter {

private:
	Spreadsheet *spreadSheet;
	spreadsheetMap finalCellMap;
	std::set<std::string> visitedCells;
	enum STATE { NUMBER, STRING, EXPRESSION, EMPTY, ERROR } state;
	
public:
	Interpreter(Spreadsheet* spreadSheetObj); 
	~Interpreter();

	const spreadsheetMap& evaluateSpreadsheet();
	bool evaluateCell(spreadsheetMap::iterator& it);
	const std::pair<STATE, spreadsheetMap::iterator> checkState(std::string& cellString);

	void calculate(std::stack<std::string>& values, std::stack<char>& operators) const;
	bool isNumber(const std::string& s) const;
	int pemdas(char op) const;
	void displaySpreadsheet();
};

#endif