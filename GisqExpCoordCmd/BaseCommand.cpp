#include "StdAfx.h"
#include "BaseCommand.h"

CBaseCommand::CBaseCommand(
								 UINT nBitmapID,
								 CString strCaption,
								 CString strCategory,
								 CString strTooltip,
								 CString strName,
								 CString strMessage,
								 CString strHelpfile)
{
	m_bitmapID    = nBitmapID;
	m_strCaption   = strCaption;
	m_strCategory = strCategory;
	m_strName     =strName;
	m_strTooltip   = strTooltip;
	m_strMessage   = strMessage;
	m_strHelpfile  = strHelpfile;
	m_ipCommand    = NULL;
	m_hBitmap      = NULL;
}

CBaseCommand::~CBaseCommand(void)
{
	if (m_hBitmap)
		::DeleteObject(m_hBitmap);
}

// IGSCommandEx
//////////////////////////////////////////////////////////////////////////
STDMETHODIMP CBaseCommand::get_Enabled ( VARIANT_BOOL * Enabled )
{
	if (Enabled == NULL)
		return E_POINTER;
	*Enabled = VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP CBaseCommand::get_Checked ( VARIANT_BOOL * Checked )
{
	if (Checked == NULL)
		return E_POINTER;
	return S_OK;
}

STDMETHODIMP CBaseCommand::get_Name ( BSTR * Name )
{
	if (Name == NULL)
		return E_POINTER;
	*Name = m_strName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CBaseCommand::get_Caption ( BSTR * Caption )
{
	if (Caption == NULL)
		return E_POINTER;
	*Caption = m_strCaption.AllocSysString();
	return S_OK;
}

STDMETHODIMP CBaseCommand::get_Tooltip ( BSTR * Tooltip )
{
	if (Tooltip == NULL)
		return E_POINTER;
	*Tooltip = m_strTooltip.AllocSysString();
	return S_OK;
}

STDMETHODIMP CBaseCommand::get_Message ( BSTR * Message )
{
	if (Message == NULL)
		return E_POINTER;
	*Message = m_strMessage.AllocSysString();
	return S_OK;
}

STDMETHODIMP CBaseCommand::get_HelpFile ( BSTR * HelpFile )
{
	if (HelpFile == NULL)
		return E_POINTER;
	return E_NOTIMPL;
}

STDMETHODIMP CBaseCommand::get_HelpContextID ( long * HelpContextID )
{
	if (HelpContextID == NULL)
		return E_POINTER;
	return E_NOTIMPL;
}

STDMETHODIMP CBaseCommand::get_Bitmap ( OLE_HANDLE * Bitmap )
{
	if (Bitmap == NULL)
		return E_POINTER;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ( m_hBitmap==NULL && m_bitmapID>0)
	{
		m_hBitmap = (HBITMAP) ::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(m_bitmapID),
			IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	}
	*Bitmap = (OLE_HANDLE) m_hBitmap;
	return S_OK;
}

STDMETHODIMP CBaseCommand::get_Category ( BSTR * Category )
{
	if (Category == NULL)
		return E_POINTER;
	*Category = m_strCategory.AllocSysString();
	return S_OK;
}

STDMETHODIMP CBaseCommand::OnCreate ( IDispatch * hook )
{
	m_ipFramework = hook;

	return S_OK;
}

STDMETHODIMP CBaseCommand::OnClick ( )
{
	return S_OK;
}

STDMETHODIMP CBaseCommand::get_CommandValueList ( IDispatch * * lpCommandValueList )
{
	get_CommandValueList(lpCommandValueList);
	return S_OK;
}

STDMETHODIMP CBaseCommand::get_CommandType ( SHORT * nCommandType )
{
	*nCommandType = CT_Button;
	return S_OK;
}

STDMETHODIMP CBaseCommand::OnItemSelectedChanged ( VARIANT varCurrentValue )
{
	 
	return S_OK;
}

STDMETHODIMP CBaseCommand::get_DefaultValue ( VARIANT * varDefaultValue )
{
	if (varDefaultValue == NULL)
		return E_POINTER;
	return S_OK;
}

STDMETHODIMP CBaseCommand::get_ContextMenuEnable ( VARIANT_BOOL * Enabled )
{

	if (Enabled == NULL)
		return E_POINTER;
	*Enabled = VARIANT_TRUE;
	return S_OK;
}
