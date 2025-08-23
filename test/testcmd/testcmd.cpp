#include "../../../YaizuComLib/src/stkpl/StkPl.h"

int main(int argc, char *argv[])
{
	StkPlPrintf("Test Command Line Arguments\n");
	StkPlPrintf("Number of arguments: %d\n", argc);
	for (int i = 0; i < argc; i++) {
		StkPlPrintf("Argument %d: %s\n", i, argv[i]);
	}
	return 0;
}
