// ImpCoordCmd.cpp : CImpCoordCmd ��ʵ��

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
	//�ؼ�����
// 	if (ImportDlg == NULL)
// 	{
// 		int n = 1;
// 	}else
// 	{
// int n = 0;
// 	}

	HWND hwndExpDlg = NULL;
	hwndExpDlg = FindWindow(NULL,_T("��ַ�㵼��"));
	//�������û�б���������������
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
		//��������Ѿ������������ұ��򿪣���Ϊ����ڣ�����򿪴���
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