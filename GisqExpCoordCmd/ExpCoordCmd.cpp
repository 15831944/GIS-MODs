// ExpCoordCmd.cpp : CExpCoordCmd 的实现

#include "stdafx.h"
#include "ExpCoordCmd.h"


// CExpCoordCmd

STDMETHODIMP CExpCoordCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//CWnd* hMainWnd = AfxGetMainWnd();
	CWnd *pParent;
	pParent = CWnd::FromHandle((HWND)m_ipFramework->GetParentHWnd());

	//控件函数


	HWND hwndExpDlg = NULL;
	hwndExpDlg = FindWindow(NULL,_T("界址点导出"));
	//如果窗口没有被创建，创建窗口
	if (hwndExpDlg == NULL)
	{
		ExportDlg = new CExpCoordDlg(m_ipFramework,pParent);
		ExportDlg->Create(MAKEINTRESOURCE(IDD_EXPCOORD_DLG),pParent);
		ExportDlg->ShowWindow(SW_SHOW);
	}else 
	{
		CWnd *ExpDlg;
		ExpDlg = CWnd::FromHandle(hwndExpDlg);
// 		ExpDlg->SendMessage(WM_DESTROY);
// 		ExportDlg = new CExpCoordDlg(m_ipFramework,hMainWnd);
// 		ExportDlg->Create(MAKEINTRESOURCE(IDD_EXPCOORD_DLG),hMainWnd);
// 		ExportDlg->ShowWindow(SW_SHOW);
		//如果窗口已经被创建过，且被打开，设为活动窗口，否则打开窗口
		if (ExpDlg->IsWindowVisible())
		{
			ExpDlg->SetActiveWindow();
		}else
		{
			ExpDlg->ShowWindow(SW_SHOW);
		}

	}


	return S_OK;
}