// dllmain.cpp : DllMain ��ʵ�֡�

#include "stdafx.h"
#include "resource.h"
#include "GisqLandPlanCmd_i.h"
#include "dllmain.h"

CGisqLandPlanCmdModule _AtlModule;

class CGisqLandPlanCmdApp : public CWinApp
{
public:

// ��д
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
