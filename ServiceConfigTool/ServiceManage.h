#pragma once
#include <Windows.h>
#include "SimpleLogger.h"
#define VAS_MANAGER  L"vas-manager"

class CServiceManege
{
public:
	static CServiceManege*GetInstance();
	
	~CServiceManege();
	DWORD GetStartType(LPCTSTR lpszServiceName);
	BOOL  SetStartType(LPCTSTR lpszServiceName, DWORD dwStartType);
	DWORD GetStatus(LPCTSTR lpszServiceName); 

	int Start(LPCTSTR lpszServiceName,
		DWORD dwNumServiceArgs = 0,
		LPCTSTR *lpszServiceArgVectors = NULL);
	int Stop(LPCTSTR lpszServiceName);

private:
	CServiceManege();
};