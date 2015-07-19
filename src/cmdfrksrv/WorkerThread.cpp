#include <windows.h>
#include <tchar.h>
#include "..\..\..\YaizuComLib\src\stkthread\stkthread.h"
#include "netaccess.h"

int SessionMonitorThreadMain(int Id)
{
	return 0;
}

int StartWorkerThread()
{
	SetProcAfterLastStkThreadStops(NULL);
	SetProcBeforeFirstStkThreadStarts(NULL);
	AddStkThread(0, _T("SessionMonitor"), _T("SessionMonitor"), NULL, NULL, SessionMonitorThreadMain, NULL, NULL);
	SetStkThreadInterval(0, 1000);
	int Ids[MAX_NUM_OF_STKTHREADS];
	Ids[0] = 0;
	StartSpecifiedStkThreads(Ids, 1);
	return 0;
}

int StopWorkerThread()
{
	int Ids[MAX_NUM_OF_STKTHREADS];
	Ids[0] = 0;
	StopSpecifiedStkThreads(Ids, 1);
	DeleteStkThread(0);
	return 0;
}
