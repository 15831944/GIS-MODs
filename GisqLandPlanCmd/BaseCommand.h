/*********************************************
文件名:  DmpBaseCommand
创建者:  lidazhang
编写日期:2014-12-08
简要描述:杰思科图形框架命令接口的实现类）（作为其他相应接口的基类）
修改记录(who-when-why):
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
	// IGSCommandEx  方法实现
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

	// IDispatch  接口实现
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
	GisqLib::_DGisqFrameworkCOMPtr	m_ipFramework;    //主框架
	ICommandPtr						m_ipCommand;      //ESRI命令
	HBITMAP							    m_hBitmap;        //图标
	CString									m_strCaption;    //标题
	CString									m_strCategory;   //分类
	CString									m_strName;       //名称
	CString									m_strTooltip;    //工具栏提示
	CString									m_strMessage;    //状态栏
	CString									m_strHelpfile;   //帮助文档
	LONG									m_lHelpID;        //帮助ID
	UINT									    m_bitmapID;       //图片ID
};

#else

#endif