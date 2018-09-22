#pragma once

#include "Resource.h"
#include "..\include\Global.h"
#include "afxwin.h"
#include "..\include\tinyxml.h"
#include "LPGridDockWinDlg.h"
#include "TableGridCtrl.h"
// CLPGridDockTabWnd �Ի���

class CLPGridDockTabWnd : public CBCGPDialog
{
	DECLARE_DYNAMIC(CLPGridDockTabWnd)

public:
	CLPGridDockTabWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLPGridDockTabWnd();

// �Ի�������
	enum { IDD = IDD_GRIDDOCKTABWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	//edit by lcc 2016.12.15 ���˫������
	LRESULT OnDBClickColumn(WPARAM, LPARAM lp);
	//edit by lcc 2016.12.15 end
	//edit by lcc 2016.12.17 ����ַ�ת���������ڶ������ļ�
	void utf8ToUnicode(LPCSTR src, CString *Result);
	//edit by lcc 2016.12.17 end
	BOOL SetFocusLayer(CComQIPtr<ILayer> spParentLayer, CComQIPtr<IMap> spMap);
	void FlashShapes(CComQIPtr<IMapControl2> ipMapControl, CComQIPtr<IGeometry> ipGeo );
	void ClearLastActiveTabSel();//�����һ��Tab��ѡ��
	void SetResText(CString strValue);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
