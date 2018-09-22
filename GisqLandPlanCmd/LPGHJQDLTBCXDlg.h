#pragma once

#include "Resource.h"
#include "afxwin.h"
#include "..\include\GlobalDefine.h"
// CLPGHJQDLTBCXDlg 对话框

class CLPGHJQDLTBCXDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPGHJQDLTBCXDlg)

public:
	CLPGHJQDLTBCXDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPGHJQDLTBCXDlg();

// 对话框数据
	enum { IDD = IDD_LPGHJQDLTBCXDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	GisqLib::_DGisqFrameworkCOMPtr	m_ipFramework; 
	IFeatureWorkspacePtr			m_ipFtWorkspace;
	CArray<CString,CString>			m_arrDLMC;
	CString							m_strParentDM;//标识最高级别的行政区
	CString							m_strParentMC;
	CArray<_tagDistrictItem *,_tagDistrictItem *> *m_parrDistrict;

	CString m_strUnit;//单位文本
	//查询条件
	double m_nTBDL_Min;
	double m_nTBDL_Max;
	double m_nTBDLMJ_Min;
	double m_nTBDLMJ_Max;
	CBCGPMaskEdit m_editTBMJ_Min;
	CBCGPMaskEdit m_editTBMJ_Max;
	CBCGPMaskEdit m_editTBDLMJ_Min;
	CBCGPMaskEdit m_editTBDLMJ_Max;

	CString m_strTown;
	CString m_strVillage;
	CString m_strGHDL;
	CBCGPComboBox m_cbxTown;
	CBCGPComboBox m_cbxVillage;
	CBCGPComboBox m_cbxGHDL;

	//用于显示结果的grid
	CBCGPStatic m_RectForGrid;
	CBCGPGridCtrl m_wndResultGrid;	

	//初始化
	BOOL InitTownComboBox();
	BOOL InitDlComboBox();
	BOOL InitGridCtrl();
	//获取行政区
	BOOL GetXzqDm();
	BOOL GetGHDLMC();
	void AddToArray(BSTR bstrDM,BSTR bstrMC);
	void SortArray(int left,int right);//快速排序

	BOOL FlashComboBox(_tagDistrictItem *tagParent);//刷新给定行政区的子combobox
	BOOL ShowResrult(IQueryFilterPtr ipQuery);
	CString VTtoCString(VARIANT m_variant);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMeterRd();
	afx_msg void OnBnClickedMuRd();
	afx_msg void OnBnClickedHectareRd();		
	afx_msg void OnCbnSelchangeXzcombo();
	afx_msg void OnBnClickedCxbutton();

	afx_msg void OnDestroy();
	CString m_strResultNum;
};
