// ExpCoordCmd.cpp : CExpCoordCmd ��ʵ��

#include "stdafx.h"
#include "ExpCoordCmd.h"


// CExpCoordCmd

STDMETHODIMP CExpCoordCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//CWnd* hMainWnd = AfxGetMainWnd();
	CWnd *pParent;
	pParent = CWnd::FromHandle((HWND)m_ipFramework->GetParentHWnd());

	//�ؼ�����


	HWND hwndExpDlg = NULL;
	hwndExpDlg = FindWindow(NULL,_T("��ַ�㵼��"));
	//�������û�б���������������
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
		//��������Ѿ������������ұ��򿪣���Ϊ����ڣ�����򿪴���
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