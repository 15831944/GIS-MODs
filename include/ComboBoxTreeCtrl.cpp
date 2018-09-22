// ComboBoxTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "ComboBoxTreeCtrl.h"


#define AFX_WNDCOMMCTL_TREEVIEW_REG     0x00080
#define AfxDeferRegisterClass(fClass) AfxEndDeferRegisterClass(fClass)
BOOL    AFXAPI AfxEndDeferRegisterClass(LONG fToRegister);
// CComboBoxTreeCtrl


IMPLEMENT_DYNAMIC(CComboBoxTreeCtrl, CComboBox)

CComboBoxTreeCtrl::CComboBoxTreeCtrl()
{

}

CComboBoxTreeCtrl::~CComboBoxTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CComboBoxTreeCtrl, CComboBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, &CComboBoxTreeCtrl::OnCbnEditupdate)
END_MESSAGE_MAP()



// CComboBoxTreeCtrl 消息处理程序

void  CComboBoxTreeCtrl::InitTree(int w,int h,UINT nid,COLORREF crMask)
{
	CRect rc(0, 0, 10, 10);
	UINT style =  WS_POPUP | WS_BORDER | TVS_DISABLEDRAGDROP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT;

	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_TREEVIEW_REG));
	CWnd* pWnd = &m_wndTreeList;
	CWnd* parentWnd =this;// GetParent();//?
	pWnd->CreateEx(0, WC_TREEVIEW, NULL, style, rc, parentWnd, 0, NULL);
	m_wndTreeList.CreateImageList(nid,crMask);
	m_wndTreeList.Init(this);

	GetClientRect(rc);
	if (w <= 0)
		w = rc.right;
	if (h <= 0)
		h = 100;
	SetDroppedWidth(w);
	SetDroppedHeight(h);
}
//绘制项
void CComboBoxTreeCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
	CDC dc;
	if(!dc.Attach(lpDrawItemStruct -> hDC ))
		return;

	CRect rc(lpDrawItemStruct->rcItem);
	CRect rc2;
	GetClientRect(rc2);
	CPoint p;
	p.x = 4;
	p.y = (rc2.bottom - m_iconWidth)/2 ;

	HTREEITEM hitem = m_wndTreeList.GetSelectedItem();
	if (hitem != NULL)
	{	
		DWORD dwValue   = (DWORD)m_wndTreeList.GetItemData(hitem);
		int nImage = -1;
		m_wndTreeList.GetItemImage(hitem,nImage,nImage);
		CString szNzme = m_wndTreeList.GetItemText(hitem);
		if (nImage != -1 )
		{	
			CImageList* imageList = m_wndTreeList.GetImageList(TVSIL_NORMAL);
			if (imageList)
				imageList->Draw(&dc,nImage, p, ILD_NORMAL);
			rc.top += 1;
			rc.bottom -= 1;
			rc.left += m_iconWidth + 2;
			CSize size = dc.GetTextExtent(szNzme);
			rc.right = rc.left + size.cx + 4;
			if(lpDrawItemStruct -> itemState & ODS_SELECTED)
			{	
				dc.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT) );
				dc.DrawFocusRect(rc);
				dc.SetTextColor((0x00FFFFFF & ~(GetSysColor(COLOR_WINDOWTEXT))));
			}
			else
				dc.SetTextColor( GetSysColor(COLOR_WINDOWTEXT) );
			rc.left += 2;
			dc.DrawText(szNzme, rc, DT_SINGLELINE | DT_VCENTER);
		}
	} 
	dc.Detach();
}

void CComboBoxTreeCtrl::MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/)
{

	// TODO:  添加您的代码以确定指定项的大小
}

BOOL CComboBoxTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (pMsg->message==WM_KEYDOWN && 
		(pMsg->wParam==VK_DOWN || pMsg->wParam==VK_RIGHT))
	{	
		//m_wndTreeList.SelectNextItem(TRUE);
		Invalidate();
		return TRUE;
	}
	else if (pMsg->message==WM_KEYDOWN && 
		(pMsg->wParam==VK_UP || pMsg->wParam==VK_LEFT))
	{	
		//m_wndTreeList.SelectNextItem(FALSE);
		Invalidate();
		return TRUE;
	}
	else if (pMsg->message==WM_SYSKEYDOWN && pMsg->wParam==VK_DOWN)
	{	
		DisplayTree();
		return TRUE;
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

void CComboBoxTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_wndTreeList.IsWindowVisible())
	{
		m_wndTreeList.ShowWindow(SW_HIDE);
	}
	else
		DisplayTree ();
	//GetParent()->SendMessage(WM_DROPDOWN);
	//CComboBox::OnLButtonDown(nFlags, point);
}

void CComboBoxTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    OnLButtonDown(nFlags, point);
	//CComboBox::OnLButtonDblClk(nFlags, point);
}

BOOL CComboBoxTreeCtrl::GetDroppedState() const
{
     return m_wndTreeList.IsWindowVisible();
}
void CComboBoxTreeCtrl::SetDroppedWidth(int width)
{
     m_droppedWidth=width;
}
void CComboBoxTreeCtrl::SetDroppedHeight(int height)
{   
	m_droppedHeight=height;
}
int  CComboBoxTreeCtrl::GetDroppedWidth()
{
     return m_droppedWidth;
}
int  CComboBoxTreeCtrl::GetDroppedHeight()
{  
	 return m_droppedHeight;
}
void CComboBoxTreeCtrl::DisplayTree()
{
	CRect rc;
	GetWindowRect(rc);
	rc.top = rc.bottom + 1;
	rc.right = GetDroppedWidth();
	rc.bottom = GetDroppedHeight();
	m_wndTreeList.Display(rc);
	m_wndTreeList.RedrawWindow();
}

BOOL CComboBoxTreeCtrl::SetSelectString(CString strValue,BOOL bAllMatch /*= TRUE*/)
{
    HTREEITEM  hRoot = m_wndTreeList.GetRootItem();
	while (hRoot)
	{
		CString strTxt;
		if (m_wndTreeList.GetItemText(hRoot)==strTxt)
		{
			m_wndTreeList.SelectItem(hRoot);
			DWORD dwType=m_wndTreeList.GetItemData(hRoot);			
			ResetContent();
			AddString(strValue);
			SetCurSel(0);
			SetItemData(0,dwType);
		}
		if(SetSelectString(hRoot,strValue,bAllMatch))
			return TRUE;
		hRoot=m_wndTreeList.GetNextItem(hRoot,TVGN_NEXT);
	}
	return FALSE;
}
BOOL CComboBoxTreeCtrl::SetSelectString(HTREEITEM item,CString strValue,BOOL bAllMatch /*= TRUE*/)
{
	HTREEITEM hChildeitem=m_wndTreeList.GetChildItem(item);
	CString strTxt;
	strTxt=m_wndTreeList.GetItemText(item);
	if (bAllMatch == TRUE ? strTxt.Compare(strValue)==0 : strTxt.Find(strValue,0) >=0 )
	{
		m_wndTreeList.SelectItem(item);
		DWORD dwType=m_wndTreeList.GetItemData(item);
		ResetContent();
		AddString( bAllMatch == TRUE ? strValue : strTxt);
		SetCurSel(0);
		SetItemData(0,dwType);
		//向下滚动
		SCROLLINFO si;
         m_wndTreeList.GetScrollInfo(SB_VERT,&si);
	     si.nPos -=1;
		m_wndTreeList.SetScrollInfo(SB_VERT,&si);
		return TRUE;
	}
	while(hChildeitem)
	{
		strTxt=m_wndTreeList.GetItemText(hChildeitem);
		if ( bAllMatch == TRUE ? strTxt.Compare(strValue)==0 : strTxt.Find(strValue,0) >=0 )
		{
			m_wndTreeList.SelectItem(hChildeitem);
			DWORD dwType=m_wndTreeList.GetItemData(hChildeitem);
			ResetContent();
			AddString(bAllMatch == TRUE ? strValue : strTxt);
			SetCurSel(0);
			SetItemData(0,dwType);
			//向下滚动
			SCROLLINFO si;
			m_wndTreeList.GetScrollInfo(SB_VERT,&si);
			si.nPos -=1;
			m_wndTreeList.SetScrollInfo(SB_VERT,&si);
			return TRUE;
		}
		HTREEITEM   chilideItem;
		if (m_wndTreeList.ItemHasChildren(hChildeitem))
		{
			chilideItem=m_wndTreeList.GetChildItem(hChildeitem);
			while(chilideItem)
			{
				SetSelectString(chilideItem,strValue,bAllMatch);
                chilideItem=m_wndTreeList.GetNextItem(chilideItem,TVGN_NEXT);
			}
		}
		hChildeitem=m_wndTreeList.GetNextItem(hChildeitem,TVGN_NEXT);
	}
	return FALSE;
}
BOOL CComboBoxTreeCtrl::SetSelectItem(HTREEITEM item,DWORD dwItemData)
{
   if (item!=NULL)
   {
	   m_wndTreeList.SelectItem(item);
	   //????????dwitemdata
	   m_wndTreeList.GetParent()->SendMessage(WM_SELECTITEM_CHANGE, (WPARAM)dwItemData);//?
   }
   return TRUE;
}
HTREEITEM CComboBoxTreeCtrl::GetSelectItem()
{
    return m_wndTreeList.GetSelectItem();
}
HTREEITEM CComboBoxTreeCtrl::AddItem(LPCTSTR itemName,int nImageIndex,DWORD dwItemData,HTREEITEM parentItem)
{
    HTREEITEM item= m_wndTreeList.InsertItem(itemName,nImageIndex,nImageIndex,parentItem);
	if (item!=NULL)
	{
		m_wndTreeList.SetItemData(item,dwItemData);
	}
	return item;
}
BOOL CComboBoxTreeCtrl::DeleteAllItems()
{
	m_wndTreeList.DeleteAllItems();
	return TRUE;
}
BOOL CComboBoxTreeCtrl::ExpandTree()
{
	HTREEITEM  hRoot = m_wndTreeList.GetRootItem();
	if (hRoot != NULL)  
		ExpandAllItem(hRoot);
	return TRUE;
}
void CComboBoxTreeCtrl::ExpandAllItem(HTREEITEM item)
{
    while(item)
	{
		m_wndTreeList.Expand(item,TVE_EXPAND);
		HTREEITEM  childItem=m_wndTreeList.GetChildItem(item);
		while(childItem)
		{
			ExpandAllItem(childItem);
			childItem=m_wndTreeList.GetNextSiblingItem(childItem);
		}
		item=m_wndTreeList.GetNextSiblingItem(item);
	}
}
void CComboBoxTreeCtrl::OnCbnEditupdate()
{
	// TODO: Add your control notification handler code here
}
