#pragma once
#include "./wtl/atlapp.h"
#include "./wtl/atlctrls.h"
#include "./wtl/atluser.h"

using namespace DuiLib;

class TreeViewWnd;

class TreeViewUI : public DuiLib::CControlUI
{
	friend class TreeViewWnd;
public:
	TreeViewUI();
	virtual ~TreeViewUI();
	virtual LPCTSTR GetClass() const;
	void Init();
	virtual void SetPos(RECT rc);

	// operate tree view
	CTreeItem InsertChildItem(LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	CTreeItem GetRootItem();
	CTreeItem GetChildItem(HTREEITEM hItem);
	CTreeItem GetNextItem(HTREEITEM hItem);
	CTreeItem GetNextSiblingItem(HTREEITEM hItem);
	CTreeItem GetParentItem(HTREEITEM hItem);
	CTreeItem GetPrevSiblingItem(HTREEITEM hItem);
	CTreeItem GetSelectedItem();
	void GetItemText(HTREEITEM hItem, LPTSTR lptstr, int iLength);
	unsigned int GetCount();
	void ExpandItem(HTREEITEM hItem);

	bool GetIcon();
	void AddIcon(HICON icon);
	void SetItemStateImage(CTreeItem hItem, unsigned int iImage = 0, unsigned int iSelectedImage = 1);
	/*
		iBar: SB_HORZ, SB_VERT
		iPos: 0-100
		bRedraw: TRUE or FALSE
	*/
	void SetScrollPos(int iBar, int iPos, BOOL bRedraw = TRUE);
protected:
	TreeViewWnd* pTreeViewWnd;
	bool isIcon;
};