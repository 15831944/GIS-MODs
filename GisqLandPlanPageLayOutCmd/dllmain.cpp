// dllmain.cpp : DllMain 的实现。

#include "stdafx.h"
#include "resource.h"
#include "GisqLandPlanPageLayOutCmd_i.h"
#include "dllmain.h"

CGisqLandPlanPageLayOutCmdModule _AtlModule;

class CGisqLandPlanPageLayOutCmdApp : public CWinApp
{
public:

// 重写
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CGisqLandPlanPageLayOutCmdApp, CWinApp)
END_MESSAGE_MAP()

CGisqLandPlanPageLayOutCmdApp theApp;

BOOL CGisqLandPlanPageLayOutCmdApp::InitInstance()
{
	return CWinApp::InitInstance();
}

int CGisqLandPlanPageLayOutCmdApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
