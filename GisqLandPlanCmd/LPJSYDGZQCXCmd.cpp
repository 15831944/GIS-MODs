// LPJSYDGZQCXCmd.cpp : CLPJSYDGZQCXCmd 的实现

#include "stdafx.h"
#include "LPJSYDGZQCXCmd.h"


// CLPJSYDGZQCXCmd

STDMETHODIMP CLPJSYDGZQCXCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* hMainWnd = AfxGetMainWnd();

	HWND hwndImpDlg = NULL;
	hwndImpDlg = FindWindow(NULL,_T("建设用地管制区查询"));
	//如果窗口没有被创建，创建窗口
	if (hwndImpDlg == NULL)
	{
		LPJSYDGZQCXDlg = new CLPJSYDGZQCXDlg(m_ipFramework,hMainWnd);
		LPJSYDGZQCXDlg->Create(MAKEINTRESOURCE(IDD_LPJSYDGZQCXDLG),hMainWnd);
		LPJSYDGZQCXDlg->ShowWindow(SW_SHOW);
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