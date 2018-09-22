#pragma once
#include "..\include\Global.h"
#include "afxwin.h"

#define GisqCustomText _T("GISQLANDPLAN$$")

enum CoordType
{
	nulltype = 0,
	Country = 1,//���ҡ��������ʽ
	Province = 2,//ʡ����ʽ
	Custom = 3,//�����ʽ
	Country_P = 4,//��ʡ����
	GisqCustom = 5//ϵͳ�����ļ�����
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
		strErr = _T("��ѡ���ļ�·��");
		return FALSE;
	}
	CFileFind filefind;
	if (filefind.FindFile(strFilePath) == FALSE)
	{
		strErr = _T("�޷��������·���ҵ��ļ����ļ���ռ��");
		return FALSE;
	}
	CFileStatus fCooordFileStatus;
	CFile::GetStatus(strFilePath,fCooordFileStatus);
	if (fCooordFileStatus.m_size > 5000000)
	{
		strErr = _T("�ļ������޷���");
		return FALSE;
	}else if (fCooordFileStatus.m_size < 8)//Ҫ�����ļ�ͷ���ڵ����
	{
		strErr = _T("��ѡ�ļ�Ϊ��");
		return FALSE;
	}
	CFile fCooordFile;
	BOOL bIsOK = FALSE;
	bIsOK = fCooordFile.Open(strFilePath,CFile::modeReadWrite,NULL);
	if (bIsOK == FALSE)
	{
		strErr = _T("�޷���ѡ�����ļ�");
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
				BCGPMessageBox(strErr,_T("����"),MB_OK);
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

// CImpCoordDlg �Ի���

class CImpCoordDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CImpCoordDlg)

public:
	CImpCoordDlg(GisqLib::_DGisqFrameworkCOMPtr ipFramework,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImpCoordDlg();

// �Ի�������
	enum { IDD = IDD_IMPCOORD_DLG };

	CComQIPtr<GisqLib::_DGisqFrameworkCOM> m_ipFramework;
	//��ͼ��ͼ�ؼ�
	CComQIPtr<IMapControl2> m_ipMapCtrl;
	//��ʱ���ݿ⹤���ռ�
	IWorkspacePtr m_ipWorkspace;
	IFeatureWorkspacePtr m_ipFtWorkspace;
	//���ͱ�־
	CoordType m_eCoordType;
	esriGeometryType m_eFtType;
	//��ʱ����ͼ����
	CString m_sLayerName;

	CString m_strOpenFilePath;//����·��
	int m_nXType;//���������ʽxλ��
	int m_nYType;//���������ʽyλ��
	CString m_sSymbol;//���������ʽ�ָ���
	//����UI�����úͽ���
	BOOL m_IsSetCustomType;
	CEdit m_XEditCtrl;
	CEdit m_YEditCtrl;
	CEdit m_SymbolEditCtrl;

	CBCGPOpenEdit m_wndFilePath;//���ļ��༭��
	CString m_strShowCoordExample;//������ʾ�����ı�
	CString m_strShowFileType;//������ʾ�жϽ��
	

	CArray<CString,CString> m_arstrCoord;//��ΪҪ�Ƚ��ж�ȡ�����жϸ�ʽ�������Ƶ��������ж�
	int m_nWorkspacesSpatialNum;
	int m_nSpatailNum;
	CComQIPtr<ISpatialReference> m_spSpatialRederence;//�������
	CComQIPtr<ISpatialReference> m_spWorkspaceSpatial;//����ϵͳ����
	CComQIPtr<IFeatureClass> m_spFeatureClass;
	CComQIPtr<ILayer> m_spFZLayer;

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnCancel();
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit1();	//��ѡ���ļ��ı�ʱ����Ԥ��ȡ
	afx_msg void OnBnClickedCustomtype();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	void ShowFileLine();//��ʾ�ļ�Ƭ��

	FileCodeType CheckFile(CFile *pCoordFile);//��ȡ�����ļ�ָ��ı���������Unicode����asni
	BOOL ReadCoordFile(CFile &CoordFile,CArray<CString,CString> *arstrTmp);//������Ԥ�����ı�������Cstring����

	BOOL FindCustomXY(CString cs,double *x,double *y);//�������Զ����������ҵ�xy
	BOOL FindCountryXY(CString cs, int *nPoint, int *nCircle, double *x,double *y);//��ʽ��һ�й�������
	BOOL FindProvinceXY(CString cs, int *nPoint, int *nCircle, int *nDKH, int *nJZD, double *x,double *y);//��ʽ��һ��ʡ������
	BOOL FindGisqCustomXY(CString cs,double *x,double *y);//�����ڽ�˼���Զ����������ҵ�xy
	BOOL CheckCoord(double *x,double *y);//���ڼ���������

	int GetWorkspaceSpatial(CComQIPtr<ISpatialReference> &spSpatial);//��ȡϵͳ����
	int SpatialReferenceToInt(CComQIPtr<ISpatialReference> spSpatial);//������ӿ�תint
	int BigToSmall(int nBig);
	int SmallToBig(int nSmall);
	int GetSpatialNum(CString strCoord);//��ȡ����
	void CreateSpatial(int nProjCS);//���ݸ����Ĵ��Ŵ����ռ�����

	BOOL ReadCoord(IFeatureClass *ipFtClass);//���ݸ�ʽ���䴦����
	//���ݲ�ͬ�ĸ�ʽ�����ò�ͬ�ĺ�����������ͨ�����ݶ�Ӧ�ĸ�ʽȫ�������Զ��庯������
	BOOL ReadCountryCoord(CComQIPtr<IFeatureClass> spFtClass);
	BOOL ReadProvinceCoord(CComQIPtr<IFeatureClass> spFtClass);
	BOOL ReadCustomCoord(IFeatureClass *ipFtClass);
	BOOL ReadCountry_PCoord(CComQIPtr<IFeatureClass> spFtClass);
	BOOL ReadGisqCustomCoord(IFeatureClass *ipFtClass);

	HRESULT AddFields(CComQIPtr<IFeature> &spFt, CMapStringToString *mapFieldAndValue = NULL);//����ֶ�,�����ļ����������İ����ֶ�-ֵ���ԣ�д��map��
	HRESULT AddFieldsToFeature(CString &strInfo, CComQIPtr<IFeature> spFt);
	HRESULT SaveFeature(CComQIPtr<IGeometry> spGeometry, CComQIPtr<IFeature> &spFt);//�������㼯�е����ݱ��浽����Ҫ����
	HRESULT PolygonCutHollow(CComQIPtr<IPointCollection> spPointCollection, CComQIPtr<IGeometry> spGeometry);//�и�Ҫ��
	HRESULT GetRing(CComQIPtr<IPointCollection> spPointCollection, CComQIPtr<IRing> &spRing);//�������ĵ㼯��ת��ΪȦ

	//��˸ͼ��
	void FlashShapes(CComQIPtr<IMapControl2> ipMapControl, CComQIPtr<IGeometry> ipGeo);
	//�����ring��ת��Ϊpolygon
	HRESULT ConvertLow2High(CComQIPtr<IGeometry> spLowGeometry,CComQIPtr<IGeometry> spHighGeometry);
	
	//��һ��������������ȫ��4�����
	void CheckCoordType();
	//��4�������ֱ��ж��ļ��Ƿ���ϸ�ʽ������ȡȫ�ֵ��ַ�������
	//����ĸ����ȫû��ͨ�������ж�Ϊ�����꣬��Ҫ�û��Լ����ø�ʽ
	//�����жϱ�׼�����ҵ���[��������]����[�ؿ�����]����0,0,D8-003,�ؿ�4,��,,,,@����8��������
	//��ʡ�����жϱ�׼�����ҵ���[��������]����[�ؿ�����]����0,0,330106104,���Ŵ�,D8-003,�ؿ�4,��,,,,@����10��������
	BOOL CheckIsCountryTypeFile();//����
	BOOL CheckIsCountry_PTypeFile();//��ʡ����
	BOOL CheckIsProvinceTypeFile();//ʡ��
	BOOL CheckIsGisqCustomTypeFile();//ϵͳ����������
	BOOL CheckCustomTypeFile();//�������꣬����ϵ
	//���ڼ�鵱ǰ�ַ����У������ַ������ֵĴ���
	//�������ֲ���ͱ�ʡ����
	int FindCharNum(CString &strTargetString, CString &strWhere);

	BOOL m_bHasError;//���ڱ����������������Ƿ���ڴ���
	CString m_strError;//����д��󣬴���д������ַ�����
	afx_msg void OnEnChangeEditsymbol();
	afx_msg void OnEnChangeEditxtype();
	afx_msg void OnEnChangeEditytype();
};
