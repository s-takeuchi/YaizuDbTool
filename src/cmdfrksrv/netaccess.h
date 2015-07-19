#pragma once
#include <windows.h>
#include <time.h>
#include "..\Global.h"

class NetAccess
{
private:
	// This instance
	static NetAccess* ThisInstance;
	NetAccess();
	~NetAccess();

	// Service start time
	char ServiceStartTime[32];

public:
	// Get this instance
	static NetAccess* GetInstance();

	// Dispatch the request from client
	int Dispatcher(int, int);

};
