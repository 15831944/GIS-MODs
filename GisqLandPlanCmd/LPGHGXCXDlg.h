#pragma once
#include "afxwin.h"
#include "LPXZQDataTree.h"
#include "..\include\ComboBoxTreeCtrl.h"
#include "LPGHGXCXRstDlg.h"
#include "..\include\GlobalDefine.h"
// CLPGHGXCXDlg 对话框

class CLPGHGXCXDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CLPGHGXCXDlg)

public:
	CLPGHGXCXDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPGHGXCXDlg();

// 对话框数据
	enum { IDD = IDD_GHGXCXDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	CString m_strMCBH;
	CComboBoxTreeCtrl m_wndXZQCombo;
	CBCGPComboBox m_wndFALXCombo;
	DATE m_dateFromTime;
	DATE m_dateToTime;
	CBCGPMaskEdit m_editFromTime;
	CBCGPMaskEdit m_editToTime;
	CBCGPDateTimeCtrl m_wndFromTime;
	CBCGPDateTimeCtrl m_wndToTime;
	CBCGPComboBox m_wndMJLXCombo;
	CBCGPComboBox m_wndZBLXCombo;
	CBCGPMaskEdit m_wndMJMin;
	CBCGPMaskEdit m_wndMJMax;
	CBCGPMaskEdit m_wndZBMin;
	CBCGPMaskEdit m_wndZBMax;
	CString m_strMJMin;
	CString m_strMJMax;
	CString m_strZBMin;
	CString m_strZBMax;

	CComQIPtr<IFeatureWorkspace> m_ipFtWorkspace;
	CComQIPtr<GisqLib::_DGisqFrameworkCOM> m_spFramework;
	CString m_strParent;
	CMapStringToString m_mapFALX;

	virtual BOOL OnInitDialog();
	//初始化行政区树
	BOOL InitXZQComboTree(int nIndex);
	void AddDistrictTreeItem(HTREEITEM parentItem = NULL,CComQIPtr<ILPXZQDataItem> spDataItem = (IUnknown*)NULL, int nStopLevel = 7);
	//初始化类型列表
	void InitComboBoxs();
	void utf8ToUnicode(LPCSTR src, CString *Result);
	//初始化时间控件
	void InitTimeCtrl();
	afx_msg void OnBnClickedOk();
	void GetTimeSet(CString &strFromTimeSQL,CString &strToTimeSQL);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CString m_strFromTime;
	CString m_strToTime;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
