#pragma once

#include "Resource.h"
#include "afxwin.h"
#include "..\include\GlobalDefine.h"
// CLPGHJQDLTBCXDlg �Ի���

class CLPGHJQDLTBCXDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPGHJQDLTBCXDlg)

public:
	CLPGHJQDLTBCXDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLPGHJQDLTBCXDlg();

// �Ի�������
	enum { IDD = IDD_LPGHJQDLTBCXDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	GisqLib::_DGisqFrameworkCOMPtr	m_ipFramework; 
	IFeatureWorkspacePtr			m_ipFtWorkspace;
	CArray<CString,CString>			m_arrDLMC;
	CString							m_strParentDM;//��ʶ��߼����������
	CString							m_strParentMC;
	CArray<_tagDistrictItem *,_tagDistrictItem *> *m_parrDistrict;

	CString m_strUnit;//��λ�ı�
	//��ѯ����
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

	//������ʾ�����grid
	CBCGPStatic m_RectForGrid;
	CBCGPGridCtrl m_wndResultGrid;	

	//��ʼ��
	BOOL InitTownComboBox();
	BOOL InitDlComboBox();
	BOOL InitGridCtrl();
	//��ȡ������
	BOOL GetXzqDm();
	BOOL GetGHDLMC();
	void AddToArray(BSTR bstrDM,BSTR bstrMC);
	void SortArray(int left,int right);//��������

	BOOL FlashComboBox(_tagDistrictItem *tagParent);//ˢ�¸�������������combobox
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
