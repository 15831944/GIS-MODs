// LPGHGXCXCmd.cpp : CLPGHGXCXCmd ��ʵ��

#include "stdafx.h"
#include "LPGHGXCXCmd.h"


// CLPGHGXCXCmd
STDMETHODIMP CLPGHGXCXCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* hMainWnd = AfxGetMainWnd();

	HWND hwndImpDlg = NULL;
	hwndImpDlg = FindWindow(NULL,_T("�滮�޸Ĳ�ѯ"));
	//�������û�б���������������
	if (hwndImpDlg == NULL)
	{
		LPGHGXCXDlg = new CLPGHGXCXDlg(m_ipFramework,hMainWnd);
		LPGHGXCXDlg->Create(MAKEINTRESOURCE(IDD_GHGXCXDLG),hMainWnd);
		LPGHGXCXDlg->ShowWindow(SW_SHOW);
	}else 
	{
		CWnd *CXDlg;
		CXDlg = CWnd::FromHandle(hwndImpDlg);
		//��������Ѿ������������ұ��򿪣���Ϊ����ڣ�����򿪴���
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
