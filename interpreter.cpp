#include "interpreter.h"

Interpreter::Interpreter(Spreadsheet* spreadSheetObj) : spreadSheet(spreadSheetObj), finalCellMap(spreadSheet->getMap()), visitedCells(), state(NUMBER) {

}

Interpreter::~Interpreter() {
	delete spreadSheet;
}

const spreadsheetMap& Interpreter::evaluateSpreadsheet() {

	for (auto it = finalCellMap.begin(); it != finalCellMap.end(); it++) 
		if (it->second[0] == '=')
			evaluateCell(it);

	return finalCellMap;
}

bool Interpreter::evaluateCell(spreadsheetMap::iterator& it) {

	std::stack<char> operators;
	std::stack<std::string> operands;

	for (unsigned int i = 0; i < it->second.length(); i++) {

		if (it->second[i] == ' ' || it->second[i] == '=')
			continue;
		else if (it->second[i] == '(')
			operators.push(it->second[i]);
		else if (it->second[i] == '+' || it->second[i] == '-' || it->second[i] == '*' || it->second[i] == '/') {

			while (!operators.empty() && pemdas(operators.top()) >= pemdas(it->second[i]))
				calculate(operands, operators);

			operators.push(it->second[i]);
		}
		else if (it->second[i] == ')') {

			while (!operators.empty() && operators.top() != '(')
				calculate(operands, operators);

			operators.pop();
		}
		else {
			std::string cell = "";

			while (i < it->second.length() && (it->second[i] != '+' && it->second[i] != '-' && it->second[i] != '*' && it->second[i] != '/' && it->second[i] != ')')) {
				cell += it->second[i];
				i++;
			}

			i--;

			if(isNumber(cell))
				operands.push(cell);
			else {
				std::pair<STATE, spreadsheetMap::iterator> cellState = checkState(cell);

				switch (cellState.first) {

				case NUMBER:
					operands.push(cellState.second->second);
					break;

				case EXPRESSION:
					if (evaluateCell(cellState.second))
						operands.push(cellState.second->second);
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

				case STRING:
					operands.push(cellState.second->second);
					break;
				}
			}
		}
	}

	while (!operators.empty())
		calculate(operands, operators);

	it->second = operands.top();
	return true;
}

const std::pair<Interpreter::STATE, spreadsheetMap::iterator> Interpreter::checkState(std::string& cellString) {

	auto itr = finalCellMap.find(std::make_pair(cellString[1] - '0', cellString[0]));
	
	if (itr != finalCellMap.end() && !itr->second.empty()) {

		if (!visitedCells.insert(cellString).second) {
			state = ERROR;
			return make_pair(state, itr);
		}
		else {
			
			if (isNumber(itr->second)) {
				state = NUMBER;
				visitedCells.erase(cellString);
				return make_pair(state, itr);
			}
			else if (itr->second[0] == '=') {
				state = EXPRESSION;
				return make_pair(state, itr);
			}
			else if (itr->second[0] == '#') {
				state = ERROR;
				return make_pair(state, itr);
			}
			else {
				state = STRING;
				return make_pair(state, itr);
			}
		}
	}

	state = EMPTY;
	return make_pair(state, itr);
}

void Interpreter::calculate(std::stack<std::string>& operands, std::stack<char>& operators) const {

	std::string operand2 = operands.top();
	operands.pop();

	std::string operand1 = operands.top();
	operands.pop();
	
	char op = operators.top();
	operators.pop();

	if (isNumber(operand1) && isNumber(operand2))
	{
		switch (op) {

		case '+':
			operands.push(std::to_string(stoi(operand1) + stoi(operand2)));
			break;

		case '-':
			operands.push(std::to_string(stoi(operand1) - stoi(operand2)));
			break;

		case '*':
			operands.push(std::to_string(stoi(operand1) * stoi(operand2)));
			break;

		case '/':
			operands.push(std::to_string(stoi(operand1) / stoi(operand2)));
			break;

		default:
			operands.push("#ERROR");
			break;
		}
	}
	else {
		if (op == '+')
			operands.push(operand1 + operand2);
		else
			operands.push("#ERROR");
	}
}

bool Interpreter::isNumber(const std::string& s) const {

	for (unsigned int i = 0; i < s.length(); i++) {

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