/*********************************************
�ļ���:  DmpBaseCommand
������:  lidazhang
��д����:2014-12-08
��Ҫ����:��˼��ͼ�ο������ӿڵ�ʵ���ࣩ����Ϊ������Ӧ�ӿڵĻ��ࣩ
�޸ļ�¼(who-when-why):
********************************************/
#ifndef     DMPBASECOMMAND_H_
#define    DMPBASECOMMAND_H_
class CBaseCommand:
	public IGSCommandEx
{
public:
	CBaseCommand(UINT nBitmapID = 0, 
		CString strCaption = NULL,     
		CString strCategory = NULL,
		CString strTooltip = NULL,
		CString strName = NULL,
		CString strMessage = NULL,
		CString strHelpfile = NULL);
	virtual ~CBaseCommand(void);
public:
	// IGSCommandEx  ����ʵ��
	STDMETHOD (get_Enabled) ( VARIANT_BOOL * Enabled );
	STDMETHOD (get_Checked) ( VARIANT_BOOL * Checked );
	STDMETHOD (get_Name)    ( BSTR * Name );
	STDMETHOD (get_Caption) ( BSTR * Caption );
	STDMETHOD (get_Tooltip) ( BSTR * Tooltip );
	STDMETHOD (get_Message) ( BSTR * Message );
	STDMETHOD (get_HelpFile)( BSTR * HelpFile );
	STDMETHOD (get_HelpContextID) ( long * HelpContextID );
	STDMETHOD (get_Bitmap)  ( OLE_HANDLE * Bitmap );
	STDMETHOD (get_Category)( BSTR * Category );
	STDMETHOD (OnCreate)    ( IDispatch * hook );
	STDMETHOD (OnClick)     ( );
	STDMETHOD (get_CommandValueList)  ( IDispatch * * lpCommandValueList );
	STDMETHOD (get_CommandType)       ( SHORT * nCommandType );
	STDMETHOD (OnItemSelectedChanged) ( VARIANT varCurrentValue );
	STDMETHOD (get_DefaultValue)      ( VARIANT * varDefaultValue );
	STDMETHOD (get_ContextMenuEnable) (VARIANT_BOOL * Enabled );

	// IDispatch  �ӿ�ʵ��
	HRESULT STDMETHODCALLTYPE GetTypeInfoCount( UINT *pctinfo)
	{
		if (pctinfo == NULL)
		{
			return E_POINTER;
		}
		  
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE GetTypeInfo( UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
	{
		
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
		REFIID riid,
		LPOLESTR *rgszNames,
		UINT cNames,
		LCID lcid,
		DISPID *rgDispId)
	{
		
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE Invoke( 
		DISPID dispIdMember,
		REFIID riid,
		LCID lcid,
		WORD wFlags,
		DISPPARAMS *pDispParams,
		VARIANT *pVarResult,
		EXCEPINFO *pExcepInfo,
		UINT *puArgErr)
	{
		
		return S_OK;
	}

protected:
	GisqLib::_DGisqFrameworkCOMPtr	m_ipFramework;    //�����
	ICommandPtr						m_ipCommand;      //ESRI����
	HBITMAP							    m_hBitmap;        //ͼ��
	CString									m_strCaption;    //����
	CString									m_strCategory;   //����
	CString									m_strName;       //����
	CString									m_strTooltip;    //��������ʾ
	CString									m_strMessage;    //״̬��
	CString									m_strHelpfile;   //�����ĵ�
	LONG									m_lHelpID;        //����ID
	UINT									    m_bitmapID;       //ͼƬID
};

#else

#endif