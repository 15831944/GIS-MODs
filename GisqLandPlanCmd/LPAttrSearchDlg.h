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
// 	CAreaSetCtrl(int nCenter, int ntop, CString strFieldAliaName,CString strFieldName, CWnd *pParent, CFont *AreaCtrlFont, CString strUnit);//����Ի����м�����ʹ����Ķ���λ��
// 	~CAreaSetCtrl();
// 	void setUnitText(CString strUnit,CWnd *pParent);
// 	void GetSQLText(BSTR *SQLText);
// };

// CLPAttrSearchDlg �Ի���

class CLPAttrSearchDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPAttrSearchDlg)

public:
	CLPAttrSearchDlg(GisqLib::_DGisqFrameworkCOMPtr pFrameworkCom, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLPAttrSearchDlg();

// �Ի�������
	enum { IDD = IDD_LPATTRSEARCHDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strParent;
	//��¼��ǰ�����������ռ�
	CComQIPtr<IFeatureWorkspace> m_ipFtWorkspace;
	//��¼��ܽӿ�
	CComQIPtr<GisqLib::_DGisqFrameworkCOM> m_pFrameworkCom;
	//��λ�ı�
	CString m_strUnit;
	//CArray<CAreaSetCtrl*,CAreaSetCtrl*> m_AreaSetArray;
	//���ڼ�¼�������ֶζ�
	CMapStringToString m_mapAreaName;
	//��¼��ǰ��ѯ��Ҫ�ؼ�����
	CString m_strFeatureClassName;
	//��¼��ǰҪ�ؼ���Ӧ�ĵ����ֵ������
	CString m_strDLDMFieldName;

	//ѡ���ѯҪ�ؼ��������˵�
	CBCGPComboBox m_wndTargetType;
	//������Ͷ�Ӧ�ľ��ο�
	CStatic m_rectAreaName;
	//���ֻ�С���������þ�̬�ı�
	CStatic *m_pStaticAreaName;
	//����С�������͡������������ѡ�����combobox
	CBCGPComboBox *m_pComboboxAreaName;
	//������������
	CComboBoxTreeCtrl m_wndXZQTree;
	//��������������
	CComboBoxTreeCtrl m_wndDLMC;
	//��ʼ��������������ͼ��𣬶�Ӧ�ĺ궨����globledefine��
	int m_nXZQLevel;


	//�ؿ�������ֵ����Сֵ�ı༭��
	// 	double m_dbAreaSizeMin;
	// 	double m_dbAreaSizeMax;
	CBCGPMaskEdit m_editAreaSizeMin;
	CBCGPMaskEdit m_editAreaSizeMax;
	CString m_strAreaSizeMin;
	CString m_strAreaSizeMax;

	//�������������˵�ǰ���ı�����
	CString m_strDLLX;

	//��¼��ǰѡ�е�������
	//���л���ѯ���ͣ�������������������ѡ�е�ǰ������
	//�����ǰ����������ʼ����ѡ���ϼ�������
	CString m_strSelXZQDM;
	CString m_strSelXZQParent;
	HTREEITEM m_pSelXZQitem;

	//��ʼ���Ի���
	virtual BOOL OnInitDialog();
	//��ʼ����������
	BOOL InitXZQComboTree(int nIndex);
	//��ʼ������������
	BOOL InitDLMCComboTree(CString DictID);
	//�ݹ�Ĳ�������������Ľڵ�
	void InsertDLMCTreeItems(TiXmlElement *CodeNamesElement, HTREEITEM pParentItem);
	//�ַ�����ת��
	void utf8ToUnicode(LPCSTR src, CString *Result);
	//���������������˵�����ͳһ�ĳ�ʼ�����������ʼ�����ڵ��ʱ�����
	void InitComboTree();
	//����������ǰ��ͼƬ�б�����û��
	void InitImageList(CImageList** ppImageList);
	//�ݹ�Ĳ����������ڵ�
	void AddDistrictTreeItem(HTREEITEM parentItem = NULL,CComQIPtr<ILPXZQDataItem> spDataItem = (IUnknown*)NULL, int nStopLevel = 7);
	//������ѯĿ��Ҫ�ؼ�ʱˢ�¡�������ؼ�
	void ReFleshAreaNameCtrl();
	//void AddAreaEditCtrl(CString strFieldAliaName, CString strFieldName);
	//void ResetWnd();

	LRESULT OnCbnSelchangeCbxDlmc( WPARAM wParam, LPARAM lParam);
	HRESULT SelectXZQbyCode(CString &strCode, CString &strParentCode, int nLevel);


	//�ڲ�ѯĿ��Ҫ�ؼ��л���Ϣ������
	//��Ҫȷ����Ȼѡ���ϴβ�ѯ��Ŀ��������
	afx_msg void OnCbnSelchangeCoxtargettype();
	//�Ի����С�ı���Ӧ����
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//������ƽ���ס���ѡ��ʱ�ı䵥λ�ı�
	afx_msg void OnBnClickedUnitMeter();
	//������Ķ����ѡ��ʱ�ı䵥λ�ı�
	afx_msg void OnBnClickedUnitMu();
	//���������ꡱ��ѡ��ʱ�ı䵥λ�ı�
	afx_msg void OnBnClickedUnitHectare();
	//afx_msg void OnBnClickedOk();
	//��ѯ��ť��Ϣ��������ƴ��SQL���͵��ý������
	//���ﻹ��һ�������Ǹı䴰�ڽ��㣬��ֹ��Ϊ���㵼�µ��ٴε��ĳ����ťʧЧ
	afx_msg void OnBnClickedGhysSearch();
	//�ر���Ϣ��Ӧ����������������
	afx_msg void OnClose();
protected:
	//����������ٵĺ���
	virtual void PostNcDestroy();
	//ȡ����ť��Ӧ����������������
	virtual void OnCancel();
};

