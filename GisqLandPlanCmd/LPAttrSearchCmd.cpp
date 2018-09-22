// LPAttrSearchCmd.cpp : CLPAttrSearchCmd ��ʵ��

#include "stdafx.h"
#include "LPAttrSearchCmd.h"


// CLPAttrSearchCmd

STDMETHODIMP CLPAttrSearchCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//CWnd* hMainWnd = AfxGetMainWnd();
	CWnd *pParent;
	pParent = CWnd::FromHandle((HWND)m_ipFramework->GetParentHWnd());

	HWND hwndImpDlg = NULL;
	hwndImpDlg = FindWindow(NULL,_T("�滮Ҫ�ز�ѯ"));
	//�������û�б���������������
	if (hwndImpDlg == NULL)
	{
		m_AttrSearchDlg = new CLPAttrSearchDlg(m_ipFramework,pParent);
		m_AttrSearchDlg->Create(MAKEINTRESOURCE(IDD_LPATTRSEARCHDLG),pParent);
		m_AttrSearchDlg->ShowWindow(SW_SHOW);
	}else 
	{
		CWnd *CXDlg;
		CXDlg = CWnd::FromHandle(hwndImpDlg);
// 		CXDlg->SendMessage(WM_DESTROY);
// 		m_AttrSearchDlg = new CLPAttrSearchDlg(m_ipFramework,hMainWnd);
// 		m_AttrSearchDlg->Create(MAKEINTRESOURCE(IDD_LPATTRSEARCHDLG),hMainWnd);
// 		m_AttrSearchDlg->ShowWindow(SW_SHOW);
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