// LPGHQYTDKCXCmd.cpp : CLPGHQYTDKCXCmd ��ʵ��

#include "stdafx.h"
#include "LPGHQYTDKCXCmd.h"


// CLPGHQYTDKCXCmd

STDMETHODIMP CLPGHQYTDKCXCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* hMainWnd = AfxGetMainWnd();

	HWND hwndImpDlg = NULL;
	hwndImpDlg = FindWindow(NULL,_T("�滮����;�ؿ��ѯ"));
	//�������û�б���������������
	if (hwndImpDlg == NULL)
	{
		LPGHQYTDKCXDlg = new CLPGHQYTDKCXDlg(m_ipFramework,hMainWnd);
		LPGHQYTDKCXDlg->Create(MAKEINTRESOURCE(IDD_LPGHQYTDKCXDLG),hMainWnd);
		LPGHQYTDKCXDlg->ShowWindow(SW_SHOW);
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
