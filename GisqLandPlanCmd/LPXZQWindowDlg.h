/*********************************************
文件名:  LPXZQWindowDlg
创建者:  lidazhang
编写日期:2016-09-08
简要描述:行政区浮动窗口类,加载行政区结构
修改记录(who-when-why):
luchencheng 2016-9-20 功能初始化版本
********************************************/
// CLPXZQWindowDlg 对话框
#ifndef   _LPXZQ_WINDOW_DLG_H_
#define  _LPXZQ_WINDOW_DLG_H_

#include "..\include\GlobalDefine.h"
#include "Resource.h"
#include "LPXZQDataTree.h"
#include "..\include\Global.h"
//#include "LPXZQWindowCmd.h"
class CLPXZQWindowDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPXZQWindowDlg)

public:
	CLPXZQWindowDlg(_DGisqFrameworkCOMPtr	ipFrameworkCom, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPXZQWindowDlg();

// 对话框数据
	enum { IDD = IDD_LPXZQWINDOWDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//add by lcc 2016.9.19
	GisqLib::_DGisqFrameworkCOMPtr	m_pFrameworkCom;
	CImageList				m_imageList;
	CTreeCtrl m_wndXZQTree;
	CArray<_tagDistrictItem *,_tagDistrictItem *> *m_parrDistrict;
	CString m_strParent;
	IFeatureWorkspacePtr m_ipFtWorkSpace;
	IFeatureClassPtr m_ipFtClass;
//change by lcc 2016-11-14 将全局变量由bstr变更为cstring	
// 	BSTR *m_bstrName;
// 	BSTR *m_bstrCode;
	CString m_strName;
	CString m_strCode;
	//add by lcc 2016.12.23
	CLPXZQWindowCmd *m_pParentCOM;
	CMapStringToString *m_pmapXZQ;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkXzqTree(NMHDR *pNMHDR, LRESULT *pResult);

	BOOL GetXzqDm(IFeatureWorkspace *ipFtWorkSpace);
	void AddToArray(BSTR bstrDM,BSTR bstrMC);
	void SortArray(int left,int right);//快速排序
	void AddDistrictTreeItem(HTREEITEM parentItem = NULL,CString parentId =_T(""));
	BOOL InitXzqTree(IFeatureWorkspace *ipFtWork = NULL);	
	void ZoomToXZQ(IMapControl2Ptr ipMapControl,CString strCode,CString strParentCode);
	void GetSelectedRegion(BSTR* strName = NULL,BSTR* strCode = NULL);
	//add by lcc 2016.9.20
	afx_msg void OnNMClickXzqTree(NMHDR *pNMHDR, LRESULT *pResult);
	//add by lcc 2016.9.27
	void FlashShapes(IMapControl2Ptr ipMapControl = NULL, IGeometryPtr ipGeo = NULL );

	BOOL InitXzqTree2(CComQIPtr<IFeatureWorkspace> ipFtWork = (IUnknown*)NULL);
	void InitImageList(CImageList** ppImageList);
	void AddDistrictTreeItem2(HTREEITEM parentItem = NULL,CComQIPtr<ILPXZQDataItem> spDataItem = (IUnknown*)NULL);
	//add by lcc 2016.12.23
	void setParentCOMPtr(CLPXZQWindowCmd *pParent);
	void ZoomToTopXZQ();
};


#endif