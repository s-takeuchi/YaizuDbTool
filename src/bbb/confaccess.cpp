#include <windows.h>
#include <shlwapi.h>
#include "confaccess.h"
#include "..\Global.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkGeneric.h"

ConfAccess* ConfAccess::ThisInstance;

ConfAccess::ConfAccess()
{
}

ConfAccess::~ConfAccess()
{
}

// Get this instance
ConfAccess* ConfAccess::GetInstance()
{
	static int Init = 1;
	if (Init == 1) {
		ThisInstance = new ConfAccess();
		Init = 0;
	}
	return ThisInstance;
}

int ConfAccess::GetHostnameAndPortFromFile(TCHAR Hostname[Global::CMDFREAK_SERVER_NAME], int* PortNum)
{
	// Default value is set.
	lstrcpy(Hostname, _T("localhost"));
	*PortNum = 2001;

	// Load properties from the specified file
	StkGeneric* StkGen = StkGeneric::GetInstance();
	if (StkGen->GetProperties(_T("bbb.conf")) != 0) {
		return -1;
	}
	char Hname[256];
	int Pnum;
	if (StkGen->GetPropertyStr("accesshost", Hname) == 0) {
		wsprintf(Hostname, _T("%S"), Hname);
	}
	if (StkGen->GetPropertyInt("accessport", &Pnum) == 0) {
		*PortNum = Pnum;
	}
	return 0;
}
