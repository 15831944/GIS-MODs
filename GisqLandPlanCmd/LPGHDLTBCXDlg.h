/*********************************************
文件名:  LPGHDLTBCXDlg
创建者:  luchencheng
编写日期:2016-10-09
简要描述:规划地类图斑查询功能窗口
修改记录(who-when-why):
********************************************/
#pragma once

#include "Resource.h"

#include "..\include\GlobalDefine.h"
#include "afxwin.h"
// CLPGHDLTBCXDlg 对话框

class CLPGHDLTBCXDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPGHDLTBCXDlg)

public:
	CLPGHDLTBCXDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPGHDLTBCXDlg();

// 对话框数据
	enum { IDD = IDD_LPGHDLTBCXDLG };

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
 	//查询条件
 	double m_nGHDL_Min;
 	double m_nGHDL_Max;
 	CBCGPMaskEdit m_editGHDLMJ_Min;
 	CBCGPMaskEdit m_editGHDLMJ_Max;
 
 	CString m_strTown;
 	CString m_strVillage;
 	CString m_strGHDL;
 	CBCGPComboBox m_cbxTown;
 	CBCGPComboBox m_cbxVillage;
 	CBCGPComboBox m_cbxGHDL;
 
 	//用于显示结果的grid
 	CBCGPStatic m_RectForGrid;
 	CBCGPGridCtrl m_wndResultGrid;	
	CString m_strResultNum;

	//初始化
	BOOL InitTownComboBox();
	BOOL InitDlComboBox();
	BOOL InitGridCtrl();

	BOOL GetXzqDm();
	BOOL GetGHDLMC();
	void AddToArray(BSTR bstrDM,BSTR bstrMC);
	void SortArray(int left,int right);//快速排序

	BOOL FlashComboBox(_tagDistrictItem *tagParent);//刷新给定行政区的子combobox
	BOOL ShowResrult(IQueryFilterPtr ipQuery);
	CString VTtoCString(VARIANT m_variant);

	//事件响应
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedMeterRd2();
	afx_msg void OnBnClickedMuRd2();
	afx_msg void OnBnClickedHectareRd2();
	afx_msg void OnBnClickedCxbutton2();
	afx_msg void OnCbnSelchangeXzcombo2();
	//下面是关闭窗口时释放窗口资源
	afx_msg void OnClose();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnDestroy();
};
