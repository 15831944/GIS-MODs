// LPJSYDGZQCXCmd.cpp : CLPJSYDGZQCXCmd ��ʵ��

#include "stdafx.h"
#include "LPJSYDGZQCXCmd.h"


// CLPJSYDGZQCXCmd

STDMETHODIMP CLPJSYDGZQCXCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* hMainWnd = AfxGetMainWnd();

	HWND hwndImpDlg = NULL;
	hwndImpDlg = FindWindow(NULL,_T("�����õع�������ѯ"));
	//�������û�б���������������
	if (hwndImpDlg == NULL)
	{
		LPJSYDGZQCXDlg = new CLPJSYDGZQCXDlg(m_ipFramework,hMainWnd);
		LPJSYDGZQCXDlg->Create(MAKEINTRESOURCE(IDD_LPJSYDGZQCXDLG),hMainWnd);
		LPJSYDGZQCXDlg->ShowWindow(SW_SHOW);
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