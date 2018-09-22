/*********************************************
文件名:  LPGHDLTBCXCmd
创建者:  luchencheng
编写日期:2016-10-09
简要描述:规划地类图斑查询功能接口
修改记录(who-when-why):
********************************************/
// LPGHDLTBCXCmd.cpp : CLPGHDLTBCXCmd 的实现

#include "stdafx.h"
#include "LPGHDLTBCXCmd.h"



// CLPGHDLTBCXCmd

STDMETHODIMP CLPGHDLTBCXCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* hMainWnd = AfxGetMainWnd();

	HWND hwndImpDlg = NULL;
	hwndImpDlg = FindWindow(NULL,_T("规划地类图斑查询"));
	//如果窗口没有被创建，创建窗口
	if (hwndImpDlg == NULL)
	{
		LPGHDLTBCXdlg = new CLPGHDLTBCXDlg(m_ipFramework,hMainWnd);
		LPGHDLTBCXdlg->Create(MAKEINTRESOURCE(IDD_LPGHDLTBCXDLG),hMainWnd);
		LPGHDLTBCXdlg->ShowWindow(SW_SHOW);
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
