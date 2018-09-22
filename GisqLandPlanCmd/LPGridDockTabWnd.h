#pragma once

#include "Resource.h"
#include "..\include\Global.h"
#include "afxwin.h"
#include "..\include\tinyxml.h"
#include "LPGridDockWinDlg.h"
#include "TableGridCtrl.h"
// CLPGridDockTabWnd 对话框

class CLPGridDockTabWnd : public CBCGPDialog
{
	DECLARE_DYNAMIC(CLPGridDockTabWnd)

public:
	CLPGridDockTabWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPGridDockTabWnd();

// 对话框数据
	enum { IDD = IDD_GRIDDOCKTABWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	GisqLib::_DGisqFrameworkCOMPtr	m_ipFramework;
	CComQIPtr<IFeatureWorkspace> m_spFtWorkspace;
	CString m_strLayerName;
	long m_nOIDFieldNameColumn;
	CString m_strOIDFieldName;
	//CBCGPGridCtrl m_wndResultGrid;
	CTableGridCtrl m_wndResultGrid;
	CStatic m_wndResCount;
	CString m_strResCount;

	LRESULT OnSelectColumn(WPARAM, LPARAM lp);
	void ZoomToScale(IMapControl2* ipMapControl,IFeature* ipFt);
	HRESULT SetQueryFilter(IUnknown* pQueryFilter, BSTR bstrFeatureClassName, BSTR bstrTabName);
	void SetWorkspace(CComQIPtr<IFeatureWorkspace> spFtWorkspace);
	void SetFramework(GisqLib::_DGisqFrameworkCOMPtr ipFramework);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//edit by lcc 2016.12.15 添加双击缩放
	LRESULT OnDBClickColumn(WPARAM, LPARAM lp);
	//edit by lcc 2016.12.15 end
	//edit by lcc 2016.12.17 添加字符转换函数用于读配置文件
	void utf8ToUnicode(LPCSTR src, CString *Result);
	//edit by lcc 2016.12.17 end
	BOOL SetFocusLayer(CComQIPtr<ILayer> spParentLayer, CComQIPtr<IMap> spMap);
	void FlashShapes(CComQIPtr<IMapControl2> ipMapControl, CComQIPtr<IGeometry> ipGeo );
	void ClearLastActiveTabSel();//清除上一个Tab的选择
	void SetResText(CString strValue);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
