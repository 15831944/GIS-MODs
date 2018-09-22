// LPGHJQDLTBCXCmd.cpp : CLPGHJQDLTBCXCmd 的实现

#include "stdafx.h"
#include "LPGHJQDLTBCXCmd.h"



// CLPGHJQDLTBCXCmd
STDMETHODIMP CLPGHJQDLTBCXCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* hMainWnd = AfxGetMainWnd();

	HWND hwndImpDlg = NULL;
	hwndImpDlg = FindWindow(NULL,_T("规划基期地类图斑查询"));
	//如果窗口没有被创建，创建窗口
	if (hwndImpDlg == NULL)
	{
		LPGHJQDLTBCXdlg = new CLPGHJQDLTBCXDlg(m_ipFramework,hMainWnd);
		LPGHJQDLTBCXdlg->Create(MAKEINTRESOURCE(IDD_GHJQDLTBCXDLG),hMainWnd);
		LPGHJQDLTBCXdlg->ShowWindow(SW_SHOW);
	}else 
	{
		CWnd *CXDlg;
		CXDlg = CWnd::FromHandle(hwndImpDlg);
		//如果窗口已经被创建过，且被打开，设为活动窗口，否则打开窗口
		if (CXDlg->IsWindowVisible())
		{
			CXDlg->SetActiveWindow();
		}else
		{
			CXDlg->ShowWindow(SW_SHOW);
		}

	}


	return S_OK;
}
