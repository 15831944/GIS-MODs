#pragma once

#include "resource.h"
#include "afxwin.h"
#include "..\include\ComboBoxTreeCtrl.h"
#include "LPXZQDataTree.h"
#include "..\include\tinyxml.h"


// class CAreaSetCtrl
// {
// public:
// 	CStatic *m_wndAreaName;
// 	CStatic *m_wndMinAreaUnit;
// 	CStatic *m_wndMaxAreaUnit;
// 	CBCGPMaskEdit *m_wndMinArea;
// 	CBCGPMaskEdit *m_wndMaxArea;
// 	static UINT uIDEdit;
// 	CString m_strMJFieldName;
// 	CFont *m_font;
// public:
// 	CAreaSetCtrl(int nCenter, int ntop, CString strFieldAliaName,CString strFieldName, CWnd *pParent, CFont *AreaCtrlFont, CString strUnit);//传入对话框中间坐标和创建的顶部位置
// 	~CAreaSetCtrl();
// 	void setUnitText(CString strUnit,CWnd *pParent);
// 	void GetSQLText(BSTR *SQLText);
// };

// CLPAttrSearchDlg 对话框

class CLPAttrSearchDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPAttrSearchDlg)

public:
	CLPAttrSearchDlg(GisqLib::_DGisqFrameworkCOMPtr pFrameworkCom, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPAttrSearchDlg();

// 对话框数据
	enum { IDD = IDD_LPATTRSEARCHDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strParent;
	//记录当前行政区工作空间
	CComQIPtr<IFeatureWorkspace> m_ipFtWorkspace;
	//记录框架接口
	CComQIPtr<GisqLib::_DGisqFrameworkCOM> m_pFrameworkCom;
	//单位文本
	CString m_strUnit;
	//CArray<CAreaSetCtrl*,CAreaSetCtrl*> m_AreaSetArray;
	//用于记录别名和字段对
	CMapStringToString m_mapAreaName;
	//记录当前查询的要素集名称
	CString m_strFeatureClassName;
	//记录当前要素集对应的地类字典表名称
	CString m_strDLDMFieldName;

	//选择查询要素集的下拉菜单
	CBCGPComboBox m_wndTargetType;
	//面积类型对应的矩形框
	CStatic m_rectAreaName;
	//如果只有“面积”，用静态文本
	CStatic *m_pStaticAreaName;
	//如果有“面积”和“净面积”两个选项，创建combobox
	CBCGPComboBox *m_pComboboxAreaName;
	//行政区下拉树
	CComboBoxTreeCtrl m_wndXZQTree;
	//地类类型下拉树
	CComboBoxTreeCtrl m_wndDLMC;
	//初始化行政区树的最低级别，对应的宏定义在globledefine中
	int m_nXZQLevel;


	//地块面积最大值和最小值的编辑框
	// 	double m_dbAreaSizeMin;
	// 	double m_dbAreaSizeMax;
	CBCGPMaskEdit m_editAreaSizeMin;
	CBCGPMaskEdit m_editAreaSizeMax;
	CString m_strAreaSizeMin;
	CString m_strAreaSizeMax;

	//地类类型下拉菜单前的文本内容
	CString m_strDLLX;

	//记录当前选中的行政区
	//在切换查询类型，重置行政区树后，重新选中当前行政区
	//如果当前行政区不初始化，选中上级行政区
	CString m_strSelXZQDM;
	CString m_strSelXZQParent;
	HTREEITEM m_pSelXZQitem;

	//初始化对话框
	virtual BOOL OnInitDialog();
	//初始化行政区树
	BOOL InitXZQComboTree(int nIndex);
	//初始化地类类型树
	BOOL InitDLMCComboTree(CString DictID);
	//递归的插入地类类型树的节点
	void InsertDLMCTreeItems(TiXmlElement *CodeNamesElement, HTREEITEM pParentItem);
	//字符编码转换
	void utf8ToUnicode(LPCSTR src, CString *Result);
	//对于所有下拉树菜单进行统一的初始化，如果不初始化，在点击时会崩溃
	void InitComboTree();
	//创建行政区前的图片列表（现在没用
	void InitImageList(CImageList** ppImageList);
	//递归的插入行政区节点
	void AddDistrictTreeItem(HTREEITEM parentItem = NULL,CComQIPtr<ILPXZQDataItem> spDataItem = (IUnknown*)NULL, int nStopLevel = 7);
	//更换查询目标要素集时刷新“面积”控件
	void ReFleshAreaNameCtrl();
	//void AddAreaEditCtrl(CString strFieldAliaName, CString strFieldName);
	//void ResetWnd();

	LRESULT OnCbnSelchangeCbxDlmc( WPARAM wParam, LPARAM lParam);
	HRESULT SelectXZQbyCode(CString &strCode, CString &strParentCode, int nLevel);


	//在查询目标要素集切换消息处理函数
	//主要确保仍然选中上次查询的目标行政区
	afx_msg void OnCbnSelchangeCoxtargettype();
	//对话框大小改变响应函数
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//单击“平方米”单选框时改变单位文本
	afx_msg void OnBnClickedUnitMeter();
	//单击“亩”单选框时改变单位文本
	afx_msg void OnBnClickedUnitMu();
	//单击“公顷”单选框时改变单位文本
	afx_msg void OnBnClickedUnitHectare();
	//afx_msg void OnBnClickedOk();
	//查询按钮消息处理，包括拼接SQL语句和调用结果窗口
	//这里还有一个操作是改变窗口焦点，防止因为焦点导致的再次点击某个按钮失效
	afx_msg void OnBnClickedGhysSearch();
	//关闭消息响应函数，处理窗口销毁
	afx_msg void OnClose();
protected:
	//窗口最后销毁的函数
	virtual void PostNcDestroy();
	//取消按钮响应函数，处理窗口销毁
	virtual void OnCancel();
};

