#include "ServiceManage.h"

CServiceManege * CServiceManege::GetInstance()
{
	static CServiceManege SerManege;
	return &SerManege;
}
CServiceManege::CServiceManege()
{
}
CServiceManege::~CServiceManege()
{
}

DWORD CServiceManege::GetStartType(LPCTSTR lpszServiceName)
{
	SC_HANDLE hManager = NULL;
	SC_HANDLE hService = NULL;
	DWORD dwBytesNeeded;
	TCHAR chBuffer[512];
	LPQUERY_SERVICE_CONFIG lpsConfig = (LPQUERY_SERVICE_CONFIG)chBuffer;

	if ((hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
	{
		LOG("Open SCManager failed,errno %ld.\n", GetLastError());
		return SERVICE_DISABLED;
	}
	if ((hService = OpenService(hManager, lpszServiceName, SERVICE_ALL_ACCESS)) == NULL)
	{
		CloseServiceHandle(hManager);
		LOG("Open service \"%s\" failed,errno %ld.\n",
			lpszServiceName, GetLastError());
		return SERVICE_DISABLED;
	}
	if (QueryServiceConfig(hService, lpsConfig, sizeof(chBuffer), &dwBytesNeeded))
	{
		return lpsConfig->dwStartType;
	}

	return SERVICE_DISABLED;
}

BOOL CServiceManege::SetStartType(LPCTSTR lpszServiceName, DWORD dwStartType)
{
	SC_HANDLE hManager = NULL;
	SC_HANDLE hService = NULL;
	DWORD dwBytesNeeded;
	TCHAR chBuffer[512];
	LPQUERY_SERVICE_CONFIG lpsConfig = (LPQUERY_SERVICE_CONFIG)chBuffer;

	if ((hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
	{
		LOG("Open SCManager failed,errno %ld.", GetLastError());
		return FALSE;
	}
	if ((hService = OpenService(hManager, lpszServiceName, SERVICE_ALL_ACCESS)) == NULL)
	{
		CloseServiceHandle(hManager);
		LOG("Open service \"%s\" failed,errno %ld.\n",
			lpszServiceName, GetLastError());
		return FALSE;
	}
	if (QueryServiceConfig(hService, lpsConfig, sizeof(chBuffer), &dwBytesNeeded))
	{
		if (lpsConfig->dwStartType == dwStartType)
		{
			return dwStartType;
		}
		else
		{
			lpsConfig->dwStartType = dwStartType;
			return ChangeServiceConfig(hService,
				SERVICE_NO_CHANGE,
				lpsConfig->dwStartType,
				SERVICE_NO_CHANGE,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL);
		}
	}
	return FALSE;
}

DWORD CServiceManege::GetStatus(LPCTSTR lpszServiceName)
{
	SC_HANDLE hManager = NULL;
	SC_HANDLE hService = NULL;
	DWORD dwStatus;
	SERVICE_STATUS sStatus;

	if ((hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
	{
		LOG("Open SCManager failed,errno %ld.", GetLastError());
		return FALSE;
	}
	if ((hService = OpenService(hManager, lpszServiceName, SERVICE_ALL_ACCESS)) == NULL)
	{
		CloseServiceHandle(hManager);
		LOG("Open service \"%s\" failed,errno %ld.\n",
			lpszServiceName, GetLastError());
		return FALSE;
	}
	if (QueryServiceStatus(hService, &sStatus))
	{
		dwStatus = sStatus.dwCurrentState;
	}
	else
	{
		dwStatus = 0xFFFFFFFF;
		LOG("Query service \"%s\" status failed,errno %ld.\n",
			lpszServiceName, GetLastError());
	}
	CloseServiceHandle(hService);
	CloseServiceHandle(hManager);

	return dwStatus;
}

int CServiceManege::Start(LPCTSTR lpszServiceName, DWORD dwNumServiceArgs, LPCTSTR * lpszServiceArgVectors)
{
	SC_HANDLE hManager = NULL;
	SC_HANDLE hService = NULL;
	BOOL bRet = 0;
	if ((hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
	{
		LOG("Open SCManager failed ,errno %ld.\n",GetLastError());
		bRet = GetLastError();
		return FALSE;
	}
	if ((hService = OpenService(hManager, lpszServiceName, SERVICE_ALL_ACCESS)) == NULL)
	{
		CloseServiceHandle(hManager);
		LOG("Open service \"%s\" failed,errno %ld.\n",
			lpszServiceName, GetLastError());
		bRet = GetLastError();
		return bRet;
	}
	if (StartService(hService, dwNumServiceArgs, lpszServiceArgVectors))
	{
		LOG("Start servcie \"%s\" success.\n", lpszServiceName);
	}
	else
	{
		if (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING)
		{
			LOG("An instance of the service \"%s\" is already running.\n",
				lpszServiceName);
		}
		else
		{
			bRet = GetLastError();
			LOG("Start service \"%s\" failed,errno %d.\n",
				lpszServiceName, GetLastError());
		}
	}
	CloseServiceHandle(hService);
	CloseServiceHandle(hManager);

	return bRet;
}

int CServiceManege::Stop(LPCTSTR lpszServiceName)
{
	SC_HANDLE hManager = NULL;
	SC_HANDLE hService = NULL;
	int bRet = 0;
	SERVICE_STATUS sStatus;

	if ((hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
	{
		bRet = GetLastError();
		LOG("Oepn SCManager failed,errno %ld.\n", GetLastError());
		return bRet;
	}
	if ((hService = OpenService(hManager, lpszServiceName, SERVICE_ALL_ACCESS)) == NULL)
	{
		bRet = GetLastError();
		CloseServiceHandle(hManager);
		LOG("Open service \"%s\" failed,errno %d.",
			lpszServiceName, GetLastError());
		return bRet;
	}
	if (!ControlService(hService, SERVICE_CONTROL_STOP, &sStatus))
	{
		LOG("Control service \"%s\" failed - %d.", lpszServiceName, GetLastError());
		bRet = GetLastError();
	}
	CloseServiceHandle(hService);
	CloseServiceHandle(hManager);

	return bRet;
}
