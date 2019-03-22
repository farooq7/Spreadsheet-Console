#include "interpreter.h"

Interpreter::Interpreter(Spreadsheet* spreadSheetObj) : spreadSheet(spreadSheetObj), finalCellMap(spreadSheet->getMap()), state(NUMBER) {

}

Interpreter::~Interpreter() {
	delete spreadSheet;
}

spreadsheetMap& Interpreter::evaluateSpreadsheet() {

	for (auto it = finalCellMap.begin(); it != finalCellMap.end(); it++) {
		
		if (!isNumber(it->second)) {

			if (it->second == "#ERROR" || it->second == "#NAN")
				continue;

			evaluateCell(it);
		}
	}

	return finalCellMap;
}

bool Interpreter::evaluateCell(spreadsheetMap::iterator& it) {

	std::stack<int> values;
	std::stack<char> operators;

	for (int i = 0; i < it->second.length(); i++) {

		if (it->second[i] == ' ' || it->second[i] == '=')
			continue;
		else if (it->second[i] == '(')
			operators.push(it->second[i]);
		else if (isdigit(it->second[i])) {
			int number = 0;

			while (i < it->second.length() && isdigit(it->second[i])) {
				number = (number * 10) + (it->second[i] - '0');
				i++;
			}

			i--;
			values.push(number);
		}
		else if (it->second[i] == '+' || it->second[i] == '-' || it->second[i] == '*' || it->second[i] == '/') {

			while (!operators.empty() && pemdas(operators.top()) >= pemdas(it->second[i]))
				calculate(values, operators);

			operators.push(it->second[i]);
		}
		else if (it->second[i] == ')') {

			while (!operators.empty() && operators.top() != '(')
				calculate(values, operators);

			operators.pop();
		}
		else {
			std::string cell = "";

			while (i < it->second.length() && (it->second[i] != '+' && it->second[i] != '-' && it->second[i] != '*' && it->second[i] != '/' && it->second[i] != ')')) {
				cell += it->second[i];
				i++;
			}

			i--;

			std::pair<STATE, spreadsheetMap::iterator> cellState = checkState(cell);

			switch (cellState.first) {

				case NUMBER:
					values.push(stoi(cellState.second->second));
					break;

				case EXPRESSION:
					if (evaluateCell(cellState.second))
						values.push(stoi(cellState.second->second));
					else if (state == EMPTY) {
						it->second = "#NAN";
						return false;
					}
					else if (state == ERROR) {
						it->second = "#ERROR";
						return false;
					}
					break;

				case EMPTY:
					it->second = "#NAN";
					state = EMPTY;
					return false;

				case ERROR:
					it->second = "#ERROR";
					state = ERROR;
					return false;
			}
		}
	}

	while (!operators.empty())
		calculate(values, operators);

	it->second = std::to_string(values.top());
	return true;
}

std::pair<Interpreter::STATE, spreadsheetMap::iterator> Interpreter::checkState(std::string& cellString) {

	static std::set<std::string> cells;

	auto itr = finalCellMap.find(std::make_pair(cellString[1] - '0', cellString[0]));

	if (itr != finalCellMap.end() && !itr->second.empty()) {

		if (!cells.insert(cellString).second) {
			state = ERROR;
			return make_pair(state, itr);
		}
		else {

			if (isNumber(itr->second)) {
				state = NUMBER;
				cells.erase(cellString);
				return make_pair(state, itr);
			}

			if (itr->second[0] == '=') {
				state = EXPRESSION;
				return make_pair(state, itr);
			}
		}
	}
	else {
		state = EMPTY;
		return make_pair(state, itr);
	}
}

void Interpreter::calculate(std::stack<int>& values, std::stack<char>& operators) const {

	int val2 = values.top();
	values.pop();

	int val1 = values.top();
	values.pop();

	char op = operators.top();
	operators.pop();

	values.push(arithmetic(val1, val2, op));
}

int Interpreter::arithmetic(int operand1, int operand2, char oper) const {

	switch (oper) {

		case '+': 
			return operand1 + operand2;

		case '-': 
			return operand1 - operand2;

		case '*': 
			return operand1 * operand2;

		case '/': 
			return operand1 / operand2;
	}
}

bool Interpreter::isNumber(const std::string& s) const {

	for (int i = 0; i < s.length(); i++) {

		if (!isdigit(s[i]))
			return false;
	}

	return true;
}

int Interpreter::pemdas(char oper) const {

	if (oper == '+' || oper == '-')
		return 1;
	if (oper == '*' || oper == '/')
		return 2;
	
	return 0;
}

void Interpreter::displaySpreadsheet() {
	
	evaluateSpreadsheet();

	spreadSheet->write(finalCellMap);
}