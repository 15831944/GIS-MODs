#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CGSBCGDialog��"
#endif 
#include "Resource.h"
#include "..\include\Global.h"
#include "afxwin.h"
enum Selection
{
	SelectThisLayer = 1,//������ǰͼ��
	SelectionInAllLayer = 2,//��������ͼ����ѡ����Ҫ��
	SelectionInThisLayer = 3//������ǰͼ��ѡ����Ҫ��
};
enum CoordType
{
	nulltype = 0,
	Country = 1,//���ҡ��������ʽ
	Province = 2,//ʡ����ʽ
	Custom = 3,//�����ʽ
	Country_P = 4//��ʡ����
};

//��дCBCGPEdit�İ�ť��ʵ�ֱ���Ի���
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
		CString strConfigFile = strDesktopPath + _T("\\�ؿ��ַ��.txt");
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

// CExpCoordDlg �Ի���

class CExpCoordDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CExpCoordDlg)

public:
	CExpCoordDlg(GisqLib::_DGisqFrameworkCOMPtr ipFramework, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExpCoordDlg();

// �Ի�������
	enum { IDD = IDD_EXPCOORD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
	Selection m_eSelect;//ѡ����Χ
	CoordType m_eCoordType;//��������
	CString m_sExpPath;//����·��
	CFile m_fSaveFile;//�������ļ�ָ��
	int m_nExpPrecision;//���澫��
	CComboBox m_LayerSelect;//ͼ��ѡ��
	int m_nProvinceFtIndex;//����ʡ���Ҫ�ر��
	int m_nProvincePointIndex;//����ʡ���Ҫ�ص�ű��

	afx_msg void OnBnClickedSelectioninlayer();
	afx_msg void OnBnClickedSelectlayer();
	afx_msg void OnBnClickedRdbcountry();
	afx_msg void OnBnClickedRdbprovince();
	afx_msg void OnBnClickedRdbconstom();
	
	BOOL ExportingCoord(IFeatureLayer *ipFtLayer);//���ڸ�ʽ������ѡ��ͼ��
	BOOL ExportingCoord(CArray<CComQIPtr<IFeature>, CComQIPtr<IFeature>> *arFt);//���ڸ�ʽ������ѡ��ͼ����ѡ�е�Ҫ�� 

	BOOL ExportHead();//��ӡ�ļ�ͷ�����ѡ���˲����꣩����Ϊ�����ʽΪȫ�����Բ��ô������
	BOOL ExportFeatureFields(CComQIPtr<IFeature> spFt);//��ӡ����Ҫ�ص������ԣ��������꣩
	BOOL ExportFeatureFieldsForCountry_P(CComQIPtr<IFeature> spFt);//��ӡ����Ҫ�ص������ԣ���ʡ���꣩

	//ͳһ�ӿڣ�ÿ�ε������һ��Ҫ��ʱʹ�á�������Ҫ�����Ҫ�أ����ǵڼ���Ҫ�����Ҫ��
	BOOL ExportingFeature(CComQIPtr<IFeature> ipFt);

	BOOL CountryTypeExporting(CComQIPtr<IFeature> ipFt);//�ò������ʽ�����һ��Ҫ��
	BOOL ProvinceTypeExporting(CComQIPtr<IFeature> ipFt);//��ʡ�������ʽ�����һ��Ҫ��
	BOOL CustomTypeExporting(CComQIPtr<IFeature> ipFt);//�ó��������ʽ�����һ��Ҫ��
	BOOL Country_pTypeExproting(CComQIPtr<IFeature> ipFt);//�ñ�ʡ�����ʽ�����һ��Ҫ��

	BOOL CheckSpatial(IGeometry* spGeo);//У��ͼ��ͶӰ��������������������ͶӰ����ȷ��������
	BOOL GetGeoXZQSpatial(CComQIPtr<IGeometry> spGeo);//��ȡͼ��������������ͶӰ
	void CreateSpatial(int nProjCS);

	//��װ���ݼ�麯���������е����������Ƶ�������м��
	//����Ԥ������е���Ϣ�������ﵯ���������ط�����������Ϣ��
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
