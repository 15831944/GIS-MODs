/*********************************************
�ļ���:  LPJSYDGZQCXDlg
������:  luchencheng
��д����:2016-10-13
��Ҫ����:�����õع�������ѯ���ܴ���
�޸ļ�¼(who-when-why):
********************************************/
// LPJSYDGZQCXDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LPJSYDGZQCXDlg.h"


// CLPJSYDGZQCXDlg �Ի���

IMPLEMENT_DYNAMIC(CLPJSYDGZQCXDlg, CGSBCGDialog)

CLPJSYDGZQCXDlg::CLPJSYDGZQCXDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework, CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CLPJSYDGZQCXDlg::IDD, pParent)
	, m_strGZQLX(_T(""))
	, m_nTDYTQMJ_Min(0)
	, m_nTDYTQMJ_Max(0)
	, m_strUnit(_T("ƽ����"))
	, m_strResultNum(_T(""))
{
	m_ipFramework = ipFramework;
}

CLPJSYDGZQCXDlg::~CLPJSYDGZQCXDlg()
{
}

void CLPJSYDGZQCXDlg::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GZQLXCOMBO, m_cbxGZQLX);
	DDX_CBString(pDX, IDC_GZQLXCOMBO, m_strGZQLX);

	DDX_Text(pDX, IDC_TDYTQMJMAX, m_nTDYTQMJ_Max);
	DDX_Text(pDX, IDC_TDYTQMJMIN, m_nTDYTQMJ_Min);
	DDX_Control(pDX, IDC_TDYTQMJMAX, m_editTDYTQMJ_Max);
	DDX_Control(pDX, IDC_TDYTQMJMIN, m_editTDYTQMJ_Min);	

	DDX_Text(pDX, IDC_UNITTEXT41, m_strUnit);
	DDX_Text(pDX, IDC_UNITTEXT42, m_strUnit);

	DDX_Control(pDX, IDC_GRIDAREA_4, m_RectForGrid);
	DDX_Text(pDX, IDC_RESCOUNT_4, m_strResultNum);
}


BEGIN_MESSAGE_MAP(CLPJSYDGZQCXDlg, CGSBCGDialog)
	ON_BN_CLICKED(IDC_METER_RD_4, &CLPJSYDGZQCXDlg::OnBnClickedMeterRd4)
	ON_BN_CLICKED(IDC_MU_RD_4, &CLPJSYDGZQCXDlg::OnBnClickedMuRd4)
	ON_BN_CLICKED(IDC_HECTARE_RD_4, &CLPJSYDGZQCXDlg::OnBnClickedHectareRd4)
	ON_BN_CLICKED(IDC_CXBUTTON_4, &CLPJSYDGZQCXDlg::OnBnClickedCxbutton4)
END_MESSAGE_MAP()


// CLPJSYDGZQCXDlg ��Ϣ�������

BOOL CLPJSYDGZQCXDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	//��ʼ��ѡ�в�ѯ���Ϊƽ����
	((CButton *)GetDlgItem(IDC_METER_RD_4))->SetCheck(TRUE);
	//��ʼ���ı���ؼ�
	m_editTDYTQMJ_Min.SetValidChars(_T("1234567890."));//�����ַ��б�
	m_editTDYTQMJ_Max.SetValidChars(_T("1234567890."));//�����ַ��б�

	m_editTDYTQMJ_Min.SetWindowText(_T("0"));
	m_editTDYTQMJ_Max.SetWindowText(_T("0"));

	//��ʼ������������
	m_cbxGZQLX.AddString(_T("ȫ��"));
	m_cbxGZQLX.AddString(_T("�������õ���"));
	m_cbxGZQLX.AddString(_T("�����������õ���"));
	m_cbxGZQLX.AddString(_T("���ƽ����õ���"));
	m_cbxGZQLX.AddString(_T("��ֹ�����õ���"));
	m_cbxGZQLX.SelectString(0,_T("ȫ��"));

	UpdateData(FALSE);

	//��ʼ�����ؼ�
	InitGridCtrl();

	if (m_ipFramework == NULL) return FALSE;
	//��ȡ�����ռ�
	IWorkspacePtr ipWorkspace;
	CComQIPtr<GisqLib::IGSDataExchangeS> spGSDataExchangeS = m_ipFramework->GetDataExchangeS();
	BSTR bstrTag(_T("GHWS"));
	short index = spGSDataExchangeS->FindIndex(bstrTag);
	if (index >= 0)
	{
		CComQIPtr<GisqLib::IGSDataExchange> spGSDataExchange = spGSDataExchangeS->Item(index);
		CComVariant var = spGSDataExchange->Item(0);
		ipWorkspace = var.punkVal;
	}
	if (ipWorkspace == NULL) return FALSE;
	m_ipFtWorkspace = ipWorkspace;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CLPJSYDGZQCXDlg::InitGridCtrl()
{
	CRect rect;
	CBCGPGridColors GridTheme;

	if (m_wndResultGrid.GetSafeHwnd() == NULL)
	{	
		m_RectForGrid.GetClientRect(&rect);
		m_RectForGrid.MapWindowPoints (this, &rect);
		if (rect == NULL)
		{
			return FALSE;
		}

		m_wndResultGrid.Create (WS_CHILD |WS_VISIBLE, rect, this, 1);
		m_wndResultGrid.EnableMarkSortedColumn (FALSE);//���ñ�����ʼ��Ϊ��
		m_wndResultGrid.EnableRowHeader (TRUE);//��ʾ��ͷ
		m_wndResultGrid.EnableLineNumbers ();//��ʾ�к�
		m_wndResultGrid.SetReadOnly(TRUE);
		GridTheme = m_wndResultGrid.GetColorTheme();
		GridTheme.m_clrHorzLine = RGB (0, 0, 0);//����ˮƽ��������ɫ
		GridTheme.m_clrVertLine = RGB (0, 0, 0);//���ô�ֱ��������ɫ
		m_wndResultGrid.SetColorTheme(GridTheme);

		//���ڵ�һ�ε���ʱ�����һ���տ�
		const int nColumns = 6;
		//�����ͷ
		for (int nColumn = 0; nColumn < nColumns; nColumn++)
		{
			CString strColumn;
			strColumn.Format (_T("%c"), _T('A') + nColumn);

			m_wndResultGrid.InsertColumn (nColumn, strColumn, 80);
		}
		//�����ͷ
		for (int nRow = 0; nRow < 10; nRow++)
		{
			CBCGPGridRow* pRow = m_wndResultGrid.CreateRow (m_wndResultGrid.GetColumnCount ());
			m_wndResultGrid.AddRow (pRow, FALSE);
		}
	}else{
		//������ǵ�һ�ε��ã���ôɾ������֮ǰ������
		m_wndResultGrid.DeleteAllColumns();//ɾ��������
		m_wndResultGrid.RemoveAll();//ɾ��������
	}
	//ˢ����ʾ�����ý���
	m_wndResultGrid.AdjustLayout (); 
	m_wndResultGrid.SetFocus (); 

	return TRUE;
}

void CLPJSYDGZQCXDlg::OnBnClickedMeterRd4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_strUnit = _T("(ƽ����)");
	UpdateData(FALSE);
}

void CLPJSYDGZQCXDlg::OnBnClickedMuRd4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_strUnit = _T("(Ķ)");
	UpdateData(FALSE);
}

void CLPJSYDGZQCXDlg::OnBnClickedHectareRd4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_strUnit = _T("(����)");
	UpdateData(FALSE);
}

void CLPJSYDGZQCXDlg::OnBnClickedCxbutton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	//��֯��ѯ����
	CString strArea = _T("");
	CString strGZQLX = _T("");
	CString strWhere = _T("");

	if (m_strGZQLX != _T("ȫ��"))
	{
		if (m_strGZQLX == "�������õ���")
		{
			strGZQLX += "GZQLXDM='010'";
		}
		if (m_strGZQLX == "�����������õ���")
		{
			strGZQLX += "GZQLXDM='020'";
		}
		if (m_strGZQLX == "���ƽ����õ���")
		{
			strGZQLX += "GZQLXDM='030'";
		}
		if (m_strGZQLX == "��ֹ�����õ���")
		{
			strGZQLX += "GZQLXDM='040'";
		}
	}else
	{
		strGZQLX += "GZQLXDM='*'";
	}
	//��ѯ����,����Ϊƽ����
	double nAREA_Max,nAREA_Min;
	if (m_strUnit == _T("��"))
	{
		nAREA_Max = m_nTDYTQMJ_Max;
		nAREA_Min = m_nTDYTQMJ_Min;
	}else if (m_strUnit == _T("(Ķ)"))
	{
		nAREA_Max = m_nTDYTQMJ_Max * 666.67;
		nAREA_Min = m_nTDYTQMJ_Min * 666.67;

	}else if (m_strUnit == _T("(����)"))
	{
		nAREA_Max = m_nTDYTQMJ_Max * 10000.0;
		nAREA_Min = m_nTDYTQMJ_Min * 10000.0;
	}
	//�ų���ѯ������ֵ
	if (m_nTDYTQMJ_Min != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ >= %lf "), nAREA_Min);
		strArea += strTmp;
	}
	if (m_nTDYTQMJ_Max != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ <= %lf "), nAREA_Max);
		strArea += strTmp;
	}

	//�ϲ���ѯ����
	strWhere +=  strGZQLX + strArea;

	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_SubFields(_T("*"));
	ipQuery->put_WhereClause(_bstr_t(strWhere));
	InitGridCtrl();
	ShowResrult(ipQuery);
}

BOOL CLPJSYDGZQCXDlg::ShowResrult(IQueryFilterPtr ipQuery)
{
	IFeatureClassPtr ipFtClass;
	m_ipFtWorkspace->OpenFeatureClass(_T("JSYDGZQ"),&ipFtClass);
	if (ipFtClass == NULL) return FALSE;

	IFeatureCursorPtr ipFtCursor;
	ipFtClass->Search(ipQuery,VARIANT_FALSE,&ipFtCursor);

	if (ipFtCursor == NULL) return FALSE;

	int nResult = 0;
	IFeaturePtr ipFt;
	ipFtCursor->NextFeature(&ipFt);

	//�����ͷ
	if (ipFt != NULL)
	{
		IFieldsPtr ipFields;
		long nNumField;
		ipFt->get_Fields(&ipFields);
		if (ipFields == NULL)return FALSE;

		ipFields->get_FieldCount(&nNumField);
		for (int i = 0; i < nNumField -1; i++)
		{
			IFieldPtr ipField;
			BSTR bstrName;
			ipFields->get_Field(i,&ipField);
			ipField->get_Name(&bstrName);
			m_wndResultGrid.InsertColumn(i,bstrName,80);
		}
	}
	//�����������
	while(ipFt != NULL)
	{
		nResult ++;

		IFieldsPtr ipFields;
		long nNumField;
		ipFt->get_Fields(&ipFields);
		if (ipFields == NULL)return FALSE;

		ipFields->get_FieldCount(&nNumField);
		CBCGPGridRow *pGridRow = m_wndResultGrid.CreateRow(nNumField);
		for (int i = 0; i < nNumField -1; i++)
		{
			IFieldPtr ipField;
			VARIANT vtTmp;
			ipFields->get_Field(i,&ipField);
			ipFt->get_Value(i,&vtTmp);
			pGridRow->GetItem(i)->SetValue(vtTmp);
		}
		m_wndResultGrid.AddRow(pGridRow);

		ipFtCursor->NextFeature(&ipFt);
	}

	m_wndResultGrid.AdjustLayout();
	m_wndResultGrid.SetFocus();

	m_strResultNum.Format(_T("��%d������"),nResult);
	UpdateData(FALSE);

	return TRUE;
}
