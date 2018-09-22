#pragma once

#include "resource.h"
#include "afxwin.h"
#include "LPGridDockTabWnd.h"

//edit by lcc 2016.12.17 添加关闭tab页功能
class CBCGPTabWndEx : public CBCGPTabWnd
{
public:
	GisqLib::_DGisqFrameworkCOMPtr	m_pFrameworkCom;
	virtual void OnClickCloseButton(CWnd* pWndActive);
};
//edit by lcc 2016.12.17 end

// CGridDockWinDlg 对话框

class CLPGridDockWinDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CLPGridDockWinDlg)

public:
	CLPGridDockWinDlg(_DGisqFrameworkCOMPtr	ipFrameworkCom,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPGridDockWinDlg();

// 对话框数据
	enum { IDD = IDD_GRIDDOCKWIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	GisqLib::_DGisqFrameworkCOMPtr	m_ipFramework;
	CComQIPtr<IFeatureWorkspace> m_spFtWorkspace;
	CString m_strLayerName;
	CString m_strResCount;
	int m_nOIDFieldNameColumn;
	CString m_strOIDFieldName;
	CBCGPTabWndEx m_wndResultTab;
	//CBCGPTabbedControlBar;
	/*CStatic m_ResultTabRect;*/
	/*CStatic m_wndResCount;*/

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnDestroy();


	HRESULT SetQueryFilter(IUnknown* pQueryFilter, BSTR bstrFeatureClassName, BSTR bstrTabName);
	LRESULT OnSelectColumn(WPARAM, LPARAM lp);
	static CString VTtoCString(VARIANT m_variant);
	static void ZoomToScale(IMapControl2* ipMapControl,IFeature* ipFt);
	int m_nActiveTab;
	LRESULT OnChangeActiveTab(WPARAM wp, LPARAM lp);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


