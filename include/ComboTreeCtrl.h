/*********************************************
文件名:  ComboTreeCtrl
创建者:  lidazhang
编写日期:2014-12-22
简要描述:下拉树控件
修改记录(who-when-why):
********************************************/
#ifndef   AFX_COMBOTREECTRL_H
#define   AFX_COMBOTREECTRL_H
class CComboBoxTreeCtrl;
class  CComboTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CComboTreeCtrl)

public:
	CComboTreeCtrl();
	virtual ~CComboTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);


public:
	//外部访问
    BOOL   Init(CComboBoxTreeCtrl* parent); 
	BOOL   CreateImageList(UINT nid=0,COLORREF crMask=RGB(255,255,255));
	void   SelectItem(HTREEITEM item);
	HTREEITEM   SelectPrevVisibleItem();
	HTREEITEM   SelectNextVisibleItem();
	HTREEITEM   GetSelectItem();
	BOOL        DeleteAllItems();
	void        Display(CRect rect);

protected:
	//私有方法
	HTREEITEM           m_selectedItem;
	CImageList	        m_imageList;
	CComboBoxTreeCtrl*  m_parentComboBox;
    void        SendMessageNotiFy();  //向父窗口发送 层层转发
    void        Hide();               //隐藏窗口
	HTREEITEM   FindData(HTREEITEM item,DWORD dwData);
 
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDestroy();
};

#endif