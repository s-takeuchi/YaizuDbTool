#include "../../../YaizuComLib/src/stkpl/StkPl.h"

int main(int argc, char *argv[])
{
	if (argc != 3) {
		wchar_t CmdName[64] = L"";
		wchar_t* PathToCmd = StkPlCreateWideCharFromUtf8(argv[0]);
		StkPlGetFileNameFromFullPath(PathToCmd, CmdName, 64);
		delete[] PathToCmd;
		StkPlWPrintf(L"Usage: %ls odbc_connection_string test_scenario\n", CmdName);
		StkPlPrintf("test_scenario:\n");
		StkPlPrintf("    GENERAL ... 3 tables, 8 columns and 150 records in each table.\n");
		StkPlExit(0);
	}
	
	return 0;
}
