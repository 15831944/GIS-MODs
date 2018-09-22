// LPExportDataCmd.cpp : CLPExportDataCmd ��ʵ��

#include "stdafx.h"
#include "LPExportDataCmd.h"


// CLPExportDataCmd

STDMETHODIMP CLPExportDataCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWnd *pParent;
	pParent = CWnd::FromHandle((HWND)m_ipFramework->GetParentHWnd());

	HWND hwndImpDlg = NULL;
	hwndImpDlg = FindWindow(NULL,_T("���ݷַ�����"));
	//�������û�б���������������
	if (hwndImpDlg == NULL)
	{
		m_pExpDataDlg = new CLPExportDataDlg(m_ipFramework,pParent);
		m_pExpDataDlg->Create(MAKEINTRESOURCE(IDD_LPEXPORTDATADLG),pParent);
		m_pExpDataDlg->ShowWindow(SW_SHOW);
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