#pragma once
#include <windows.h>
#include <tchar.h>
#include "..\Global.h"

class ConfAccess
{
private:
	// This instance
	static ConfAccess* ThisInstance;
	ConfAccess();
	~ConfAccess();

public:
	// Get this instance
	static ConfAccess* GetInstance();

	// GetHost name and port number from configuration file.
	int GetHostnameAndPortFromFile(TCHAR[Global::CMDFREAK_SERVER_NAME], int* PortNum);
};
