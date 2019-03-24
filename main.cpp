#include "spreadsheet.h"
#include "interpreter.h"

int main(int argc, char* argv[]) {

	Interpreter c1(new Spreadsheet("tests/testinput1.txt"));

        c1.displaySpreadsheet();

	return 0;

}
