#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "spreadsheet.h"
#include <stack>
#include <set>

class Interpreter {

private:
	Spreadsheet *spreadSheet;
	spreadsheetMap finalCellMap;
	enum STATE { NUMBER, STRING, EXPRESSION, EMPTY, ERROR } state;

public:
	Interpreter(Spreadsheet* spreadSheetObj); 
	~Interpreter();

	spreadsheetMap& evaluateSpreadsheet();
	bool evaluateCell(spreadsheetMap::iterator& it);
	std::pair<STATE, spreadsheetMap::iterator> checkState(std::string& cellString);

	void calculate(std::stack<int>& values, std::stack<char>& operators) const;
	int arithmetic(int a, int b, char op) const;
	bool isNumber(const std::string& s) const;
	int pemdas(char op) const;
	void displaySpreadsheet();
};

#endif