// LPGHGXCXCmd.cpp : CLPGHGXCXCmd 的实现

#include "stdafx.h"
#include "LPGHGXCXCmd.h"


// CLPGHGXCXCmd
STDMETHODIMP CLPGHGXCXCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* hMainWnd = AfxGetMainWnd();

	HWND hwndImpDlg = NULL;
	hwndImpDlg = FindWindow(NULL,_T("规划修改查询"));
	//如果窗口没有被创建，创建窗口
	if (hwndImpDlg == NULL)
	{
		LPGHGXCXDlg = new CLPGHGXCXDlg(m_ipFramework,hMainWnd);
		LPGHGXCXDlg->Create(MAKEINTRESOURCE(IDD_GHGXCXDLG),hMainWnd);
		LPGHGXCXDlg->ShowWindow(SW_SHOW);
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
