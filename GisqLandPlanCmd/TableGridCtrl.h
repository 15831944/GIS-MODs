#pragma once
#include "bcgpgridctrl.h"
//#include "afxtempl.h" 
#define MAX_FIELD_COUNT                128
#define DEFAULT_COLUMN_WIDTH           80

using namespace std;
#include <map>
#include "..\include\Global.h"
#include "..\include\tinyxml.h"

struct ROWDATATYPE                   //����
{
	wchar_t     *FieldValue[MAX_FIELD_COUNT]; //�е��ֶ�����
	bool        bSelected;                    //���Ƿ�ѡ��
	long        objectid;                     //�е�ObjectID
};

class CSortedList 
{
public:
	CSortedList();

	void AddTail(ROWDATATYPE t);
	POSITION GetHeadPosition();
	ROWDATATYPE GetNext(POSITION & p);
	void RemoveAt(int nIndex);     
	void RemoveAll();
	void BuildArray(BOOL bRebuild = FALSE);
	void BuildSelectedArray(IFeatureSelectionPtr lpFeatureSelection = NULL);
	void Sort(int nColumnSorted,esriFieldType fieldType,BOOL bAsending,BOOL bAdd);
	ROWDATATYPE * GetSortAt(int n);
	ROWDATATYPE * GetAt(int n);
	//ROWDATATYPE * GetSelectedAt(int n) { return m_Selectedvector.GetAt(n); }
	int  GetCount();
	int  GetSelectedCount();
	void ShowSelectedOnly(BOOL bShowSelectedOnly);
	BOOL IsShowSelectedOnly();
	void SetColumnCount(int nCount);

protected:
	CList<ROWDATATYPE,ROWDATATYPE>        m_list;             
	CArray<ROWDATATYPE *, ROWDATATYPE *>  m_vector;
	CArray<ROWDATATYPE *, ROWDATATYPE *>  m_Selectedvector;

private:
	BOOL                                  m_bShowSelectedOnly;  //ֻ��ʾѡ���
	int                                   m_nColumnCount;
};

class CTableGridCtrl : public CBCGPGridCtrl
{
public:
	CTableGridCtrl();
	CTableGridCtrl(GisqLib::_DGisqFrameworkCOMPtr pFrameworkCOM,
		           ITablePtr pTable,
				   IQueryFilterPtr pQueryFilter,
				   BOOL bOnlyShowSelected);

	~CTableGridCtrl(void);

	void			  SetInitValue(GisqLib::_DGisqFrameworkCOMPtr pFrameworkCOM,
						ITablePtr pTable,
						IQueryFilterPtr pQueryFilter,
						BOOL bOnlyShowSelected);
	BOOL              FillTable(BOOL bFillAll = FALSE);  //���ؼ�¼
	BOOL              FillTable(ICursorPtr pCursor);     
	void              MoveToIndex(int nIndex);           //�ж�λ
	int               GetRowCount ();                    //�Ѽ��صļ�¼����
	//void                         SetRowCount (int nCount);
	void              Sort (BOOL bAscending = TRUE, BOOL bAdd = FALSE);  //����
	int               GetCurrentColumn();                //��ǰѡ����

	void              ClearAllSelectedRow(BOOL bClearFeatureSlectionSet = FALSE);
	void     	      SetSelectedRowByOID(long nOid, bool bSelected);

	int               GetSelectedRowCount();            //��ȡ��ǰѡ��

	CString m_strFtClassName;
	CString m_strOIDFieldName;
	CString m_strShapeFieldName;
	CComQIPtr<IField> m_spLenthField;
	CComQIPtr<IField> m_spAreaField;
	void utf8ToUnicode(LPCSTR src, CString *Result) ;
	void SetFtClassName(CString strFtClassName);
	void setColumHide(CString strOIDFieldName,CString strShapeFieldName,CComQIPtr<IField> spLenthField,CComQIPtr<IField> spAreaField);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	//void                         SetSelectedRowCount(int nCount);
	IFieldsPtr        GetOldFields();
	BOOL              IsBeingEdit();
	void              FilterSelectedItem(BOOL bFilterSelected);
	BOOL              IsOnlyShowSelected();
	void              SetVirtualRowsEx();
	
	void              InitControls(ITablePtr pTable = NULL, IQueryFilterPtr pQueryFilter = NULL);
	
	void              ResetLastSelected();
	BOOL              IsFillAllRecords();

	CSortedList                  m_List;
	COLORREF                     m_SelColor;            //ѡ����ɫ

protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
	
	BOOL EndEditItem (BOOL bUpdateData = TRUE);
	BOOL EditItem (CBCGPGridRow* pItem, LPPOINT lptClick = NULL);

private:
	void                         LoadTable(void);       
	BOOL                         InitColumns();
	BOOL                         IsColumnReadOnly(int nColumn);
	BOOL                         IsColumnCanSort(int nColumn);
	BOOL                         IsColumnCanStat(int nColumn);  //�Ƿ��ͳ��

	void                         PopupColumnMenu(int nColumn);
	void                         PopupRowMenu(int nRow);
	BOOL                         UpdatetValueByID(CBCGPGridItemID id);
	void                         ChangeValueTypeByFieldType( IField* ipField, _variant_t &varValue );

	//void                         DoSelect();
	void                         DoShiftSelect(CBCGPGridItemID idCurrent);
	void                         DoCtrlSelect();
	int                          m_nLastSelectedTop;          //��һ��ѡ��
	int                          m_nLastSelectedBottom;       //��һ��ѡ��
	
	GisqLib::_DGisqFrameworkCOMPtr m_pFrameworkCOM;
	BOOL                         m_bOnlyShowSelected;   //��ʾѡ���
	ITablePtr                    m_pTable;              
	IQueryFilterPtr              m_pQueryFilter;        //��ѯ����
	IFieldsPtr                   m_pFieldsClone;        //��¡

	ICursorPtr                   m_pCursor;             //��ǰ�ļ�¼

    int                          m_nCurrentColumn;      //��ǰ��
	BOOL                         m_bStartSelect;        //��ʼѡ��
	
	BOOL                         m_bIsFillAllRecords;

};


