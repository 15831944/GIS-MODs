// LPGHGXRstCmd.cpp : CLPGHGXRstCmd 的实现

#include "stdafx.h"
#include "LPGHGXRstCmd.h"


// CLPGHGXRstCmd

STDMETHODIMP CLPGHGXRstCmd::get_Caption (/*[out,retval]*/ BSTR * strCaption )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (strCaption == NULL)
	{
		return E_FAIL;
	}
	*strCaption=m_strCaption.AllocSysString();
	return S_OK;
}
STDMETHODIMP CLPGHGXRstCmd::get_ChildHWnd (/*[out,retval]*/ OLE_HANDLE * hWnd )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (hWnd == NULL)
	{
		return E_POINTER;
	}
	if (m_pGHGXCXRstDlg != NULL)
	{
		*hWnd = (OLE_HANDLE)m_pGHGXCXRstDlg->GetSafeHwnd();
		return S_OK;
	}
	return E_FAIL;
}
STDMETHODIMP CLPGHGXRstCmd::get_Name (/*[out,retval]*/ BSTR * strName )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (strName == NULL)
	{
		return E_FAIL;
	}
	*strName=m_strName.AllocSysString();
	return S_OK;
}
STDMETHODIMP CLPGHGXRstCmd::OnCreate (/*[in]*/ IDispatch * lpFrameworkCOM )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_pFrameworkCom = lpFrameworkCOM;
	BOOL bRet = FALSE;
	if (m_pGHGXCXRstDlg == NULL)
	{
		m_pGHGXCXRstDlg = new CLPGHGXCXRstDlg(m_pFrameworkCom);
		bRet = m_pGHGXCXRstDlg->Create(CLPGHGXCXRstDlg::IDD,NULL);
	}
	return S_OK;
}
STDMETHODIMP CLPGHGXRstCmd::OnDestroy ( )
{
 	if (m_pGHGXCXRstDlg != NULL)
 	{
 		m_pGHGXCXRstDlg->SendMessage(WM_DESTROY);
 		m_pGHGXCXRstDlg = NULL;
 	}
	return S_OK;

}
STDMETHODIMP CLPGHGXRstCmd::SearchFA(BSTR bstrQueryFilter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	m_pGHGXCXRstDlg->SearchFA(bstrQueryFilter);
	return S_OK;
}
