/*********************************************
�ļ���:  LPGHDLTBCXCmd
������:  luchencheng
��д����:2016-10-09
��Ҫ����:�滮����ͼ�߲�ѯ���ܽӿ�
�޸ļ�¼(who-when-why):
********************************************/
// LPGHDLTBCXCmd.cpp : CLPGHDLTBCXCmd ��ʵ��

#include "stdafx.h"
#include "LPGHDLTBCXCmd.h"



// CLPGHDLTBCXCmd

STDMETHODIMP CLPGHDLTBCXCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd* hMainWnd = AfxGetMainWnd();

	HWND hwndImpDlg = NULL;
	hwndImpDlg = FindWindow(NULL,_T("�滮����ͼ�߲�ѯ"));
	//�������û�б���������������
	if (hwndImpDlg == NULL)
	{
		LPGHDLTBCXdlg = new CLPGHDLTBCXDlg(m_ipFramework,hMainWnd);
		LPGHDLTBCXdlg->Create(MAKEINTRESOURCE(IDD_LPGHDLTBCXDLG),hMainWnd);
		LPGHDLTBCXdlg->ShowWindow(SW_SHOW);
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
