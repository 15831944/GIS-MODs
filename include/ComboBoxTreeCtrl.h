/*********************************************
�ļ���:  ComboBoxTreeCtrl
������:  lidazhang
��д����:2014-12-22
��Ҫ����:�������ؼ�
�޸ļ�¼(who-when-why):
********************************************/
#ifndef   AFX_COMBOBOXTREECTRL_H
#define   AFX_COMBOBOXTREECTRL_H
#include "ComboTreeCtrl.h"
//�¼���Ϣ
#define WM_SELECTITEM_CHANGE	       WM_USER + 102
#define	WM_DROPDOWN				       WM_USER + 103
// CComboBoxTreeCtrl
class  CComboBoxTreeCtrl : public CBCGPComboBox
{
	DECLARE_DYNAMIC(CComboBoxTreeCtrl)

public:
	CComboBoxTreeCtrl();
	virtual ~CComboBoxTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

//�ⲿ���÷���
public:
	//����ѡ����
	BOOL      SetSelectItem(HTREEITEM item,DWORD dwItemData);
	HTREEITEM GetSelectItem();
	HTREEITEM AddItem(LPCTSTR itemName,int nImageIndex,DWORD dwItemData,HTREEITEM parentItem);
	BOOL      DeleteAllItems();
	BOOL      ExpandTree();
    void        InitTree(int w,int h,UINT nid=0,COLORREF crMask=RGB(255,255,255));
    BOOL     SetSelectString(CString strValue,BOOL bAllMatch = TRUE);
	BOOL     SetSelectString(HTREEITEM item,CString strValue,BOOL bAllMatch = TRUE);

private:
	//�ڲ���������
	BOOL   GetDroppedState() const;
	void     SetDroppedWidth(int width);
	void     SetDroppedHeight(int height);
	int       GetDroppedWidth();
	int       GetDroppedHeight();
	void     DisplayTree();
    void     ExpandAllItem(HTREEITEM item);


protected:
	//��Ա����
	int      m_iconWidth;
	int      m_droppedHeight;
	int      m_droppedWidth;

public:
	CComboTreeCtrl   m_wndTreeList;
	afx_msg void OnCbnEditupdate();
};
#endif