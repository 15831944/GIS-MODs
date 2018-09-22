#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "../include/tinyxml.h"
#include "BCGPTipListBox.h"
#include "..\include\Global.h"
#include "LPXZQDataTree.h"

//面积及指标总计类，用于地块和项目层
class CSumInfo
{
public:
	CSumInfo()
	{
		dbSumZMJ = dbSumNYDMJ = dbSumGDJMJ = dbSumJSYDMJ = dbSumWLYDMJ = dbSumJBNTJMJ = 0.0;
		dbSumXZJSYD1 = dbSumXZJSYD2 = dbSumXZJSYD3 = dbSumXZJSYD4 = 0.0;
		dbSumJBNT1 = dbSumJBNT2 = dbSumJBNT3 = dbSumJBNT4 = 0.0;
	}
	void ClearInfo()
	{
		dbSumZMJ = dbSumNYDMJ = dbSumGDJMJ = dbSumJSYDMJ = dbSumWLYDMJ = dbSumJBNTJMJ = 0.0;
		dbSumXZJSYD1 = dbSumXZJSYD2 = dbSumXZJSYD3 = dbSumXZJSYD4 = 0.0;
		dbSumJBNT1 = dbSumJBNT2 = dbSumJBNT3 = dbSumJBNT4 = 0.0;
	}
	//总面积，基本农田面积，耕地净面积，建设用地面积，未利用地面积，基本农田净面积
	double dbSumZMJ,dbSumNYDMJ,dbSumGDJMJ,dbSumJSYDMJ,dbSumWLYDMJ,dbSumJBNTJMJ;
	//新增建设用地省市县乡四级指标
	double dbSumXZJSYD1,dbSumXZJSYD2,dbSumXZJSYD3,dbSumXZJSYD4;
	//基本农田省市县乡四级指标
	double dbSumJBNT1,dbSumJBNT2,dbSumJBNT3,dbSumJBNT4;
};
//地块信息类
class CDKInfoClass
{
public:
	CDKInfoClass()
	{
		strDKBSM = strXZDM = strCUNDM = strCUNMC = strXZMC = _T("");
	}
	void ClearInfo()
	{
		strDKBSM = strXZDM = strCUNDM = strCUNMC = strXZMC = _T("");
	}
	CString strDKBSM;
	CString strXZDM;
	CString strXZMC;
	CString strCUNDM;
	CString strCUNMC;
};
//项目信息类
class CXMInfoClass
{
public:
	CXMInfoClass()
	{
		strXMBSM = _T("");
		strXMMC = _T("");
		strXMLX = _T("");
		dbZMJ = dbNYDMJ = dbGDJMJ = dbJSYDMJ = dbWLYDMJ = dbJBNTJMJ = dbXGDKMJ = 0.0;
		dbJSYDZBsheng = dbJSYDZBshi = dbJSYDZBxian = dbJSYDZBxiang = 0.0;
		dbJBNTZBsheng = dbJBNTZBshi = dbJBNTZBxian = dbJBNTZBxiang = 0.0;
	}
	void ClearInfo()
	{
		strXMBSM = _T("");
		strXMMC = _T("");
		strXMLX = _T("");
		dbZMJ = dbNYDMJ = dbGDJMJ = dbJSYDMJ = dbWLYDMJ = dbJBNTJMJ = dbXGDKMJ = 0.0;
		dbJSYDZBsheng = dbJSYDZBshi = dbJSYDZBxian = dbJSYDZBxiang = 0.0;
		dbJBNTZBsheng = dbJBNTZBshi = dbJBNTZBxian = dbJBNTZBxiang = 0.0;
		for (int i = 0; i < m_arrDKInfo.GetCount(); i++)
		{
			if (m_arrDKInfo.GetAt(i) != NULL)
			{
				delete m_arrDKInfo.GetAt(i);
				m_arrDKInfo.GetAt(i) = NULL;
			}
		}
		m_arrDKInfo.RemoveAll();
		m_DKSumInfo.ClearInfo();
	}
	//项目标识码
	CString strXMBSM;
	//项目名称
	CString strXMMC;
	//项目类型
	CString strXMLX;
	//总面积,农用地面积，耕地净面积，建设用地面积，未利用地面积，基本农田净面积，修改地块面积
	double dbZMJ,dbNYDMJ,dbGDJMJ,dbJSYDMJ,dbWLYDMJ,dbJBNTJMJ,dbXGDKMJ;
	//新增建设用地、基本农田，省市县乡四级指标
	double dbJSYDZBsheng,dbJSYDZBshi,dbJSYDZBxian,dbJSYDZBxiang;
	double dbJBNTZBsheng,dbJBNTZBshi,dbJBNTZBxian,dbJBNTZBxiang;
	//地块信息数组
	CArray<CDKInfoClass*,CDKInfoClass*> m_arrDKInfo;
	//地块信息总计类
	CSumInfo m_DKSumInfo;
};

//方案信息类
//基本农田的耕地面积就是基本农田面积
class CFAInfoClass
{
public:
	CFAInfoClass()
	{
		strFABSM = _T("");
		strSLH = _T("");
		strFALX = _T("");
		strFAMC = _T("");
		strSPJG = _T("");
		strSPSJ = _T("");
		strGXSJ = _T("");
		strGXZT = _T("");
		strGHMC = _T("");
		strSJXMMC = _T("");
		strXMC = _T("");
		strXDM = _T("");
		m_mapstrSJXZMC.RemoveAll();
		nXMCount = nXZCount = nDKCount = 0;
		dbZMJ = dbNYDMJ = dbGDJMJ = dbJSYDMJ = dbWLYDMJ = dbJBNTJMJ = dbXMZMJ = 0.0;
		dbJSYDZBsheng = dbJSYDZBshi = dbJSYDZBxian = dbJSYDZBxiang = 0.0;
		dbJBNTZBsheng = dbJBNTZBshi = dbJBNTZBxian = dbJBNTZBxiang = 0.0;
	}
	//同初始化，清空遗留的信息
	void ClearInfo()
	{
		strFABSM = _T("");
		strSLH = _T("");
		strFALX = _T("");
		strFAMC = _T("");
		strSPJG = _T("");
		strSPSJ = _T("");
		strGXSJ = _T("");
		strGXZT = _T("");
		strGHMC = _T("");
		strSJXMMC = _T("");
		strXMC = _T("");
		strXDM = _T("");
		m_mapstrSJXZMC.RemoveAll();
		nXMCount = nXZCount = nDKCount = 0;
		dbZMJ = dbNYDMJ = dbGDJMJ = dbJSYDMJ = dbWLYDMJ = dbJBNTJMJ = dbXMZMJ = 0.0;
		dbJSYDZBsheng = dbJSYDZBshi = dbJSYDZBxian = dbJSYDZBxiang = 0.0;
		dbJBNTZBsheng = dbJBNTZBshi = dbJBNTZBxian = dbJBNTZBxiang = 0.0;
		for (int i = 0; i < m_arrXMInfo.GetCount(); i++)
		{
			if (m_arrXMInfo.GetAt(i) != NULL)
			{
				(m_arrXMInfo.GetAt(i))->ClearInfo();
				delete m_arrXMInfo.GetAt(i);
				m_arrXMInfo.GetAt(i) = NULL;
			}
		}
		m_arrXMInfo.RemoveAll();
		m_XMSumInfo.ClearInfo();
	}
	//项目标识码
	CString strFABSM;
	//受理号
	CString strSLH;
	//方案类型
	CString strFALX;
	//方案名称
	CString strFAMC;
	//审批机构
	CString strSPJG;
	//审批时间
	CString strSPSJ;
	//更新时间
	CString strGXSJ;
	//更新状态
	CString strGXZT;

	//涉及规划名称
	CString strGHMC;
	//涉及项目名称
	CString strSJXMMC;
	//县代码
	CString strXDM;
	//县名称
	CString strXMC;
	//涉及乡镇名称
	CMapStringToString m_mapstrSJXZMC;
	//涉及项目数量
	int nXMCount;
	//涉及乡镇数量
	int nXZCount;
	//涉及地块个数
	int nDKCount;

	//总面积
	double dbZMJ;
	//农用地面积
	double dbNYDMJ;
	//耕地净面积
	double dbGDJMJ;
	//建设用地面积
	double dbJSYDMJ;
	//未利用地面积
	double dbWLYDMJ;
	//基本农田面积
	double dbJBNTJMJ;
	//项目总面积
	double dbXMZMJ;
	//新增建设用地、基本农田，省市县乡四级指标
	double dbJSYDZBsheng,dbJSYDZBshi,dbJSYDZBxian,dbJSYDZBxiang;
	double dbJBNTZBsheng,dbJBNTZBshi,dbJBNTZBxian,dbJBNTZBxiang;

	//涉及项目数组
	CArray<CXMInfoClass*,CXMInfoClass*> m_arrXMInfo;
	//项目总计类 
	CSumInfo m_XMSumInfo;
};
//重载网格控件自定义行高
class CCXResBCGGridCtrl : public CBCGPGridCtrl
{
public:
	virtual void SetRowHeight ()
	{
		CBCGPGridCtrl::SetRowHeight();
		m_nRowHeight += 10;
	}
};
// CLPGHGXCXRstDlg 对话框

class CLPGHGXCXRstDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CLPGHGXCXRstDlg)

public:
	CLPGHGXCXRstDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework,CWnd* pParent = NULL);   // 标准构造函数
	CLPGHGXCXRstDlg(CComQIPtr<GisqLib::_DGisqFrameworkCOM> ipFramework,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPGHGXCXRstDlg();

// 对话框数据
	enum { IDD = IDD_GHGXCXRSTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//方案查询结果表
	CBCGPTipListBox m_wndFARstList;
	//记录当前选中的方案，防止重复刷新
	int m_nFASelectIndex;
	//结果tab控件
	CBCGPTabWnd *m_pwndResTabWnd;
	//结果方案信息页
	CCXResBCGGridCtrl *m_pwndFAInfoGrid;
	//结果项目地块页
	CBCGPGridCtrl *m_pwndFADKGrid;
	//方案信息类
	CFAInfoClass m_csFAInfo;
	//框架
	CComQIPtr<GisqLib::_DGisqFrameworkCOM> m_spFramework;
	//更新状态Map
	CMapStringToString m_strmapGXZT;
	//方案类型Map
	CMapStringToString m_strmapFALX;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLbnSelchangeFalist();
	//根据给定条件查询方案名称和标识码，并插入listbox
	HRESULT SearchFA(CComBSTR strQrueyQueryFilter);
	//连接到数据库
	HRESULT ConnToOracle(_ConnectionPtr &ipADOConn);
	HRESULT getConnString(CComBSTR &bstrConnSet,CComBSTR &bstrName,CComBSTR &bstrPasswd);
	//获取方案信息
	HRESULT GetFAInfoByUID(_ConnectionPtr &ipADOConn, CString &strFABSM);
	//显示方案信息
	HRESULT ShowFAInfo();
	//获取查询条件
	void GetSQLstring(CString &strFABSM, CString &strSQL);
	//初始化各个字典
	void InitMaps();
	void utf8ToUnicode(LPCSTR src, CString *Result);
};
