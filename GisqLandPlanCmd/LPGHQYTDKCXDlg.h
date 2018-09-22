/*********************************************
文件名:  LPGHQYTDKCXDlg
创建者:  luchencheng
编写日期:2016-10-013
简要描述:规划期用途地块查询功能窗口
修改记录(who-when-why):
********************************************/
#pragma once

#include "resource.h"
#include "..\include\GlobalDefine.h"
#include "afxwin.h"
// CLPGHQYTDKCXDlg 对话框

class CLPGHQYTDKCXDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPGHQYTDKCXDlg)

public:
	CLPGHQYTDKCXDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPGHQYTDKCXDlg();

// 对话框数据
	enum { IDD = IDD_LPGHQYTDKCXDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	GisqLib::_DGisqFrameworkCOMPtr	m_ipFramework;

public:
	IFeatureWorkspacePtr			m_ipFtWorkspace;
	CArray<CString,CString>			m_arrDLMC;
	CString							m_strParentDM;//标识最高级别的行政区
	CString							m_strParentMC;
	CArray<_tagDistrictItem *,_tagDistrictItem *> *m_parrDistrict;

	CString m_strUnit;//单位文本
	//查询范围
	CString m_strTown;
	CString m_strGHDL;
	CBCGPComboBox m_cbxTown;
	CBCGPComboBox m_cbxGHDL;

	double m_nAREA_Min;
	double m_nAREA_Max;
	CBCGPMaskEdit m_editAREA_Min;
	CBCGPMaskEdit m_editAREA_Max;

	//结果显示
	CStatic m_RectForGrid;
	CBCGPGridCtrl m_wndResultGrid;
	CString m_strResultNum;

	//初始化
	BOOL InitTownComboBox();
	BOOL InitDlComboBox();
	BOOL InitGridCtrl();
	//获取行政区
	BOOL GetXzqDm();
	BOOL GetGHDLMC();
	void AddToArray(BSTR bstrDM,BSTR bstrMC);
	void SortArray(int left,int right);//快速排序
	//结果显示
	BOOL ShowResrult(IQueryFilterPtr ipQuery);

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMeterRd3();
	afx_msg void OnBnClickedMuRd3();
	afx_msg void OnBnClickedHectareRd3();
	afx_msg void OnBnClickedCxbutton3();
};
