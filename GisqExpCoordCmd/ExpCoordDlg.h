#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CGSBCGDialog。"
#endif 
#include "Resource.h"
#include "..\include\Global.h"
#include "afxwin.h"
enum Selection
{
	SelectThisLayer = 1,//导出当前图层
	SelectionInAllLayer = 2,//导出所有图层中选定的要素
	SelectionInThisLayer = 3//导出当前图层选定的要素
};
enum CoordType
{
	nulltype = 0,
	Country = 1,//国家、部坐标格式
	Province = 2,//省厅格式
	Custom = 3,//常规格式
	Country_P = 4//报省部标
};

//重写CBCGPEdit的按钮，实现保存对话框
class CBCGPSaveEdit : public CBCGPEdit
{
public:
	void OnBrowse()
	{
		CString strEditorText;
		GetWindowText (strEditorText);
		TCHAR szDesktopFullPath[MAX_PATH];
		DWORD dBufferSize = sizeof(szDesktopFullPath)/sizeof(CHAR);
		SHGetSpecialFolderPath(NULL,szDesktopFullPath,CSIDL_DESKTOPDIRECTORY,NULL);
		CString strDesktopPath(szDesktopFullPath);
		CString strConfigFile = strDesktopPath + _T("\\地块界址点.txt");
		CFileDialog dlg(FALSE, NULL, strConfigFile, NULL, _T("TXT(*.txt)|*.txt||"), GetParent ());
		//CFileDialog dlg(FALSE, NULL, strConfigFile,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TXT(*.txt)|*.txt||"), GetParent ());
		if (dlg.DoModal () == IDOK &&
			strEditorText != dlg.GetPathName ())
		{
			CString strFilePath =  dlg.GetPathName ();
			if (strFilePath.Find(_T(".txt")) < 0)
			{
				strFilePath += _T(".txt");
			}
			SetWindowText (strFilePath);
			SetModify (TRUE);
			OnAfterUpdate ();
		}
	}
};

// CExpCoordDlg 对话框

class CExpCoordDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CExpCoordDlg)

public:
	CExpCoordDlg(GisqLib::_DGisqFrameworkCOMPtr ipFramework, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExpCoordDlg();

// 对话框数据
	enum { IDD = IDD_EXPCOORD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()

	CComQIPtr<GisqLib::_DGisqFrameworkCOM> m_ipFramework;
	CComQIPtr<IFeatureWorkspace> m_ipFtWorkspace;
	CComQIPtr<ISpatialReference> m_spSpatialRederence;
	CComQIPtr<IMapControl2> m_ipMapCtrl;
	CComQIPtr<IFeatureClass> m_ipXZQXFtClass;
	CComQIPtr<IEnumFeature> m_ipEnumFt;
	Selection m_eSelect;//选集范围
	CoordType m_eCoordType;//坐标类型
	CString m_sExpPath;//保存路径
	CFile m_fSaveFile;//保存用文件指针
	int m_nExpPrecision;//保存精度
	CComboBox m_LayerSelect;//图层选集
	int m_nProvinceFtIndex;//用于省标多要素标记
	int m_nProvincePointIndex;//用于省标多要素点号标记

	afx_msg void OnBnClickedSelectioninlayer();
	afx_msg void OnBnClickedSelectlayer();
	afx_msg void OnBnClickedRdbcountry();
	afx_msg void OnBnClickedRdbprovince();
	afx_msg void OnBnClickedRdbconstom();
	
	BOOL ExportingCoord(IFeatureLayer *ipFtLayer);//用于格式化导出选中图层
	BOOL ExportingCoord(CArray<CComQIPtr<IFeature>, CComQIPtr<IFeature>> *arFt);//用于格式化导出选中图层中选中的要素 

	BOOL ExportHead();//打印文件头（如果选择了部坐标），因为坐标格式为全局所以不用传入参数
	BOOL ExportFeatureFields(CComQIPtr<IFeature> spFt);//打印给定要素的书属性（仅部坐标）
	BOOL ExportFeatureFieldsForCountry_P(CComQIPtr<IFeature> spFt);//打印给定要素的书属性（报省部标）

	//统一接口，每次调用输出一个要素时使用。参数：要输出的要素，这是第几个要输出的要素
	BOOL ExportingFeature(CComQIPtr<IFeature> ipFt);

	BOOL CountryTypeExporting(CComQIPtr<IFeature> ipFt);//用部坐标格式化输出一个要素
	BOOL ProvinceTypeExporting(CComQIPtr<IFeature> ipFt);//用省厅坐标格式化输出一个要素
	BOOL CustomTypeExporting(CComQIPtr<IFeature> ipFt);//用常规坐标格式化输出一个要素
	BOOL Country_pTypeExproting(CComQIPtr<IFeature> ipFt);//用报省部标格式化输出一个要素

	BOOL CheckSpatial(IGeometry* spGeo);//校对图形投影，根据所在行政区重新投影到正确的行政区
	BOOL GetGeoXZQSpatial(CComQIPtr<IGeometry> spGeo);//获取图形所在行政区的投影
	void CreateSpatial(int nProjCS);

	//封装数据检查函数，将所有的数据问题移到这里进行检查
	//数据预检查所有的消息框都在这里弹出，其他地方尽量不弹消息框
	BOOL CheckData(CComQIPtr<IFeatureLayer> spFtLayer);
	BOOL CheckData(CArray<CComQIPtr<IFeature>, CComQIPtr<IFeature>> *arFt);

public:
	CBCGPSaveEdit m_wndSavePathEdit;
	CBCGPMaskEdit m_wndExpPrecision;
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	afx_msg void OnBnClickedRdbcountryP();
};
