// dllmain.cpp : DllMain 的实现。

#include "stdafx.h"
#include "resource.h"
#include "GisqLandPlanCmd_i.h"
#include "dllmain.h"

CGisqLandPlanCmdModule _AtlModule;

class CGisqLandPlanCmdApp : public CWinApp
{
public:

// 重写
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CGisqLandPlanCmdApp, CWinApp)
END_MESSAGE_MAP()

CGisqLandPlanCmdApp theApp;

BOOL CGisqLandPlanCmdApp::InitInstance()
{
	return CWinApp::InitInstance();
}

int CGisqLandPlanCmdApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
