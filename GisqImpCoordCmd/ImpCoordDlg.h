#pragma once
#include "..\include\Global.h"
#include "afxwin.h"

#define GisqCustomText _T("GISQLANDPLAN$$")

enum CoordType
{
	nulltype = 0,
	Country = 1,//国家、部坐标格式
	Province = 2,//省厅格式
	Custom = 3,//常规格式
	Country_P = 4,//报省部标
	GisqCustom = 5//系统导出的简单坐标
};

enum FileCodeType
{
	unkown = 0,
	utf8 = 1,
	unicode = 2,
	asni = 3
};

static BOOL CoordFileCheck(CString &strFilePath, CString &strErr)
{
	if (strFilePath.GetLength() == 0)
	{
		strErr = _T("请选择文件路径");
		return FALSE;
	}
	CFileFind filefind;
	if (filefind.FindFile(strFilePath) == FALSE)
	{
		strErr = _T("无法在输入的路径找到文件或文件被占用");
		return FALSE;
	}
	CFileStatus fCooordFileStatus;
	CFile::GetStatus(strFilePath,fCooordFileStatus);
	if (fCooordFileStatus.m_size > 5000000)
	{
		strErr = _T("文件过大，无法打开");
		return FALSE;
	}else if (fCooordFileStatus.m_size < 8)//要考虑文件头存在的情况
	{
		strErr = _T("所选文件为空");
		return FALSE;
	}
	CFile fCooordFile;
	BOOL bIsOK = FALSE;
	bIsOK = fCooordFile.Open(strFilePath,CFile::modeReadWrite,NULL);
	if (bIsOK == FALSE)
	{
		strErr = _T("无法打开选定的文件");
		return FALSE;
	}
	fCooordFile.Close();
	return TRUE;
}

class CBCGPOpenEdit : public CBCGPEdit
{
public:
	void OnBrowse()
	{
		CString strEditorText;
		GetWindowText (strEditorText);
		CFileDialog dlg(TRUE, NULL, NULL, NULL, _T("TXT(*.txt)|*.txt||"), GetParent ());
		//CFileDialog dlg(FALSE, NULL, strConfigFile,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TXT(*.txt)|*.txt||"), GetParent ());
		if (dlg.DoModal () == IDOK &&
			strEditorText != dlg.GetPathName ())
		{
			CString strFilePath =  dlg.GetPathName ();
			if (strFilePath.Find(_T(".txt")) < 0)
			{
				strFilePath += _T(".txt");
			}
			CString strErr;
			if (CoordFileCheck(strFilePath,strErr) == FALSE)
			{
				BCGPMessageBox(strErr,_T("错误"),MB_OK);
				SetWindowText(_T(""));
				SetModify(TRUE);
				OnAfterUpdate ();
			}else
			{
				SetWindowText (strFilePath);
				SetModify (TRUE);
				OnAfterUpdate ();
			}
		}
	}
};

// CImpCoordDlg 对话框

class CImpCoordDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CImpCoordDlg)

public:
	CImpCoordDlg(GisqLib::_DGisqFrameworkCOMPtr ipFramework,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImpCoordDlg();

// 对话框数据
	enum { IDD = IDD_IMPCOORD_DLG };

	CComQIPtr<GisqLib::_DGisqFrameworkCOM> m_ipFramework;
	//地图视图控件
	CComQIPtr<IMapControl2> m_ipMapCtrl;
	//临时数据库工作空间
	IWorkspacePtr m_ipWorkspace;
	IFeatureWorkspacePtr m_ipFtWorkspace;
	//类型标志
	CoordType m_eCoordType;
	esriGeometryType m_eFtType;
	//临时数据图层名
	CString m_sLayerName;

	CString m_strOpenFilePath;//保存路径
	int m_nXType;//其他坐标格式x位置
	int m_nYType;//其他坐标格式y位置
	CString m_sSymbol;//其他坐标格式分隔符
	//用于UI的启用和禁用
	BOOL m_IsSetCustomType;
	CEdit m_XEditCtrl;
	CEdit m_YEditCtrl;
	CEdit m_SymbolEditCtrl;

	CBCGPOpenEdit m_wndFilePath;//打开文件编辑框
	CString m_strShowCoordExample;//用于显示几行文本
	CString m_strShowFileType;//用于显示判断结果
	

	CArray<CString,CString> m_arstrCoord;//因为要先进行读取，并判断格式，所以移到这里做判断
	int m_nWorkspacesSpatialNum;
	int m_nSpatailNum;
	CComQIPtr<ISpatialReference> m_spSpatialRederence;//保存带号
	CComQIPtr<ISpatialReference> m_spWorkspaceSpatial;//保存系统带号
	CComQIPtr<IFeatureClass> m_spFeatureClass;
	CComQIPtr<ILayer> m_spFZLayer;

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnCancel();
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit1();	//当选择文件改变时进行预读取
	afx_msg void OnBnClickedCustomtype();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	void ShowFileLine();//显示文件片段

	FileCodeType CheckFile(CFile *pCoordFile);//获取给定文件指针的编码类型是Unicode还是asni
	BOOL ReadCoordFile(CFile &CoordFile,CArray<CString,CString> *arstrTmp);//按编码预处理文本，返回Cstring数组

	BOOL FindCustomXY(CString cs,double *x,double *y);//用于在自定义条件下找到xy
	BOOL FindCountryXY(CString cs, int *nPoint, int *nCircle, double *x,double *y);//格式化一行国标数据
	BOOL FindProvinceXY(CString cs, int *nPoint, int *nCircle, int *nDKH, int *nJZD, double *x,double *y);//格式化一行省标数据
	BOOL FindGisqCustomXY(CString cs,double *x,double *y);//用于在杰思科自定义条件下找到xy
	BOOL CheckCoord(double *x,double *y);//用于检查坐标带号

	int GetWorkspaceSpatial(CComQIPtr<ISpatialReference> &spSpatial);//获取系统带号
	int SpatialReferenceToInt(CComQIPtr<ISpatialReference> spSpatial);//从坐标接口转int
	int BigToSmall(int nBig);
	int SmallToBig(int nSmall);
	int GetSpatialNum(CString strCoord);//获取带号
	void CreateSpatial(int nProjCS);//根据给定的带号创建空间坐标

	BOOL ReadCoord(IFeatureClass *ipFtClass);//根据格式分配处理函数
	//根据不同的格式，调用不同的函数处理，可以通过传递对应的格式全部交由自定义函数处理
	BOOL ReadCountryCoord(CComQIPtr<IFeatureClass> spFtClass);
	BOOL ReadProvinceCoord(CComQIPtr<IFeatureClass> spFtClass);
	BOOL ReadCustomCoord(IFeatureClass *ipFtClass);
	BOOL ReadCountry_PCoord(CComQIPtr<IFeatureClass> spFtClass);
	BOOL ReadGisqCustomCoord(IFeatureClass *ipFtClass);

	HRESULT AddFields(CComQIPtr<IFeature> &spFt, CMapStringToString *mapFieldAndValue = NULL);//添加字段,除了文件名，其他的按“字段-值”对，写在map中
	HRESULT AddFieldsToFeature(CString &strInfo, CComQIPtr<IFeature> spFt);
	HRESULT SaveFeature(CComQIPtr<IGeometry> spGeometry, CComQIPtr<IFeature> &spFt);//将给定点集中的数据保存到给定要素中
	HRESULT PolygonCutHollow(CComQIPtr<IPointCollection> spPointCollection, CComQIPtr<IGeometry> spGeometry);//切割要素
	HRESULT GetRing(CComQIPtr<IPointCollection> spPointCollection, CComQIPtr<IRing> &spRing);//将给定的点集合转化为圈

	//闪烁图形
	void FlashShapes(CComQIPtr<IMapControl2> ipMapControl, CComQIPtr<IGeometry> ipGeo);
	//如果是ring，转化为polygon
	HRESULT ConvertLow2High(CComQIPtr<IGeometry> spLowGeometry,CComQIPtr<IGeometry> spHighGeometry);
	
	//用一个函数调用下面全部4个检查
	void CheckCoordType();
	//用4个函数分别判断文件是否符合格式，参数取全局的字符串数组
	//如果四个检查全没有通过，则判定为简单坐标，需要用户自己设置格式
	//部标判断标准：能找到“[属性描述]”“[地块坐标]”“0,0,D8-003,地块4,面,,,,@”，8个“，”
	//报省部标判断标准：能找到“[属性描述]”“[地块坐标]”“0,0,330106104,横桥村,D8-003,地块4,面,,,,@”，10个“，”
	BOOL CheckIsCountryTypeFile();//部标
	BOOL CheckIsCountry_PTypeFile();//报省部标
	BOOL CheckIsProvinceTypeFile();//省标
	BOOL CheckIsGisqCustomTypeFile();//系统导出简单坐标
	BOOL CheckCustomTypeFile();//检查简单坐标，坐标系
	//用于检查当前字符串中，给定字符串出现的次数
	//用于区分部标和报省部标
	int FindCharNum(CString &strTargetString, CString &strWhere);

	BOOL m_bHasError;//用于标记整个导入过程中是否存在错误
	CString m_strError;//如果有错误，错误写在这个字符串中
	afx_msg void OnEnChangeEditsymbol();
	afx_msg void OnEnChangeEditxtype();
	afx_msg void OnEnChangeEditytype();
};
