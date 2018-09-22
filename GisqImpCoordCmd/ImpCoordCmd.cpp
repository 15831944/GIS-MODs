// ImpCoordCmd.cpp : CImpCoordCmd 的实现

#include "stdafx.h"
#include "ImpCoordCmd.h"


// CImpCoordCmd
/*CImpCoordDlg *CImpCoordCmd::ImportDlg = NULL;*/
STDMETHODIMP CImpCoordCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//CWnd* hMainWnd = AfxGetMainWnd();
	CWnd *pParent;
	pParent = CWnd::FromHandle((HWND)m_ipFramework->GetParentHWnd());
	//控件函数
// 	if (ImportDlg == NULL)
// 	{
// 		int n = 1;
// 	}else
// 	{
// int n = 0;
// 	}

	HWND hwndExpDlg = NULL;
	hwndExpDlg = FindWindow(NULL,_T("界址点导入"));
	//如果窗口没有被创建，创建窗口
	if (hwndExpDlg == NULL)
	{
		ImportDlg = new CImpCoordDlg(m_ipFramework,pParent);
		ImportDlg->Create(MAKEINTRESOURCE(IDD_IMPCOORD_DLG),pParent);
		ImportDlg->ShowWindow(SW_SHOW);
	}else 
	{
		CWnd *ImpDlg;
		ImpDlg = CWnd::FromHandle(hwndExpDlg);
		//ImpDlg->SendMessage(WM_DESTROY);
// 		ImportDlg = (CImpCoordDlg *)ImpDlg;
// 		delete ImportDlg;
// 		ImportDlg = new CImpCoordDlg(m_ipFramework,hMainWnd);
// 		ImportDlg->Create(MAKEINTRESOURCE(IDD_IMPCOORD_DLG),hMainWnd);
// 		ImportDlg->ShowWindow(SW_SHOW);
		//如果窗口已经被创建过，且被打开，设为活动窗口，否则打开窗口
		if (ImpDlg->IsWindowVisible())
		{
			ImpDlg->SetActiveWindow();
		}else
		{
			ImpDlg->ShowWindow(SW_SHOW);
		}

	}
	return S_OK;
}