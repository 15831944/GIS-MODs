#pragma once
#include "bcgpgridctrl.h"
//#include "afxtempl.h" 
#define MAX_FIELD_COUNT                128
#define DEFAULT_COLUMN_WIDTH           80

using namespace std;
#include <map>
#include "..\include\Global.h"
#include "..\include\tinyxml.h"

struct ROWDATATYPE                   //数据
{
	wchar_t     *FieldValue[MAX_FIELD_COUNT]; //行的字段内容
	bool        bSelected;                    //行是否被选中
	long        objectid;                     //行的ObjectID
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
	BOOL                                  m_bShowSelectedOnly;  //只显示选择的
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
	BOOL              FillTable(BOOL bFillAll = FALSE);  //加载记录
	BOOL              FillTable(ICursorPtr pCursor);     
	void              MoveToIndex(int nIndex);           //行定位
	int               GetRowCount ();                    //已加载的记录总数
	//void                         SetRowCount (int nCount);
	void              Sort (BOOL bAscending = TRUE, BOOL bAdd = FALSE);  //排序
	int               GetCurrentColumn();                //当前选择列

	void              ClearAllSelectedRow(BOOL bClearFeatureSlectionSet = FALSE);
	void     	      SetSelectedRowByOID(long nOid, bool bSelected);

	int               GetSelectedRowCount();            //获取当前选中

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
	COLORREF                     m_SelColor;            //选择颜色

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
	BOOL                         IsColumnCanStat(int nColumn);  //是否可统计

	void                         PopupColumnMenu(int nColumn);
	void                         PopupRowMenu(int nRow);
	BOOL                         UpdatetValueByID(CBCGPGridItemID id);
	void                         ChangeValueTypeByFieldType( IField* ipField, _variant_t &varValue );

	//void                         DoSelect();
	void                         DoShiftSelect(CBCGPGridItemID idCurrent);
	void                         DoCtrlSelect();
	int                          m_nLastSelectedTop;          //上一次选择
	int                          m_nLastSelectedBottom;       //上一次选择
	
	GisqLib::_DGisqFrameworkCOMPtr m_pFrameworkCOM;
	BOOL                         m_bOnlyShowSelected;   //显示选择的
	ITablePtr                    m_pTable;              
	IQueryFilterPtr              m_pQueryFilter;        //查询条件
	IFieldsPtr                   m_pFieldsClone;        //克隆

	ICursorPtr                   m_pCursor;             //当前的记录

    int                          m_nCurrentColumn;      //当前行
	BOOL                         m_bStartSelect;        //开始选择
	
	BOOL                         m_bIsFillAllRecords;

};


