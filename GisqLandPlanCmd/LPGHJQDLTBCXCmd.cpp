// LPGHJQDLTBCXCmd.cpp : CLPGHJQDLTBCXCmd ��ʵ��

#include "stdafx.h"
#include "LPGHJQDLTBCXCmd.h"



// CLPGHJQDLTBCXCmd
STDMETHODIMP CLPGHJQDLTBCXCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* hMainWnd = AfxGetMainWnd();

	HWND hwndImpDlg = NULL;
	hwndImpDlg = FindWindow(NULL,_T("�滮���ڵ���ͼ�߲�ѯ"));
	//�������û�б���������������
	if (hwndImpDlg == NULL)
	{
		LPGHJQDLTBCXdlg = new CLPGHJQDLTBCXDlg(m_ipFramework,hMainWnd);
		LPGHJQDLTBCXdlg->Create(MAKEINTRESOURCE(IDD_GHJQDLTBCXDLG),hMainWnd);
		LPGHJQDLTBCXdlg->ShowWindow(SW_SHOW);
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
