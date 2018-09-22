// CLPXZQWindowCmd.cpp : CLPXZQWindowCmd 的实现

#include "stdafx.h"
#include "LPXZQWindowCmd.h"


// CLPXZQWindowCmd


STDMETHODIMP CLPXZQWindowCmd::get_Caption (/*[out,retval]*/ BSTR * strCaption )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (strCaption == NULL)
	{
		return E_FAIL;
	}
	*strCaption=m_strCaption.AllocSysString();
	return S_OK;
}
STDMETHODIMP CLPXZQWindowCmd::get_ChildHWnd (/*[out,retval]*/ OLE_HANDLE * hWnd )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (hWnd == NULL)
	{
		return E_POINTER;
	}
	if (m_pXZQDiolog != NULL)
	{
		*hWnd = (OLE_HANDLE)m_pXZQDiolog->GetSafeHwnd();
		return S_OK;
	}
	return E_FAIL;
}
STDMETHODIMP CLPXZQWindowCmd::get_Name (/*[out,retval]*/ BSTR * strName )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (strName == NULL)
	{
		return E_FAIL;
	}
	*strName=m_strName.AllocSysString();
	return S_OK;
}
STDMETHODIMP CLPXZQWindowCmd::OnCreate (/*[in]*/ IDispatch * lpFrameworkCOM )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_pFrameworkCom = lpFrameworkCOM;
	if (m_pXZQDiolog == NULL)
	{
		m_pXZQDiolog = new CLPXZQWindowDlg(m_pFrameworkCom);
		m_pXZQDiolog->setParentCOMPtr(this);
		m_pXZQDiolog->Create(CLPXZQWindowDlg::IDD,NULL);
	}
	return S_OK;
}
STDMETHODIMP CLPXZQWindowCmd::OnDestroy ( )
{
	if (m_pXZQDiolog != NULL)
	{
		m_pXZQDiolog->SendMessage(WM_DESTROY);
		delete m_pXZQDiolog;
		m_pXZQDiolog = NULL;
	}
	return S_OK;

}

STDMETHODIMP CLPXZQWindowCmd::GetSelectedRegion(BSTR* strName, BSTR* strCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	if (m_pXZQDiolog != NULL && strName != NULL && strCode != NULL)
	{
		m_pXZQDiolog->GetSelectedRegion(strName,strCode);
		return S_OK;
	}else
	{
		return E_POINTER;
	}


}

STDMETHODIMP CLPXZQWindowCmd::ZoomToTopXZQ(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	m_pXZQDiolog->ZoomToTopXZQ();
	return S_OK;
}
