// CExtListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MemExtDC.h"
#include "ExtListCtrl.h"


#define ADD_MESSAGE_HEADER WM_APP + 1

/*

CListCtrl class already have the following functions which can be used :- 

	DeleteItem(Index) = Delete created item from list and call re adjust
	DeleteAllItems() = Delete all the items from the list

	GetItemCount() = Get the total number of items present in list

	SetItemText(iRow,iCol,string) = To set the individual row and column cell data

*/


// CExtListCtrl

IMPLEMENT_DYNAMIC(CExtListCtrl, CWnd)

CExtListCtrl::CExtListCtrl()
{
	_lbBrushTransparent.lbStyle = BS_NULL;
	_BrushTransparent.CreateBrushIndirect(&_lbBrushTransparent);
	
	memset(&_lfDefaultFont, 0, sizeof(LOGFONT));
	_lfDefaultFont.lfHeight = kDefaultItemTextHeight;
	_lfDefaultFont.lfWeight = FW_REGULAR;
	_lfDefaultFont.lfQuality = CLEARTYPE_NATURAL_QUALITY;
	wcscpy_s(_lfDefaultFont.lfFaceName, L"Arial");


	//Default Value
	m_objDefaultColAtt.m_bAutoFitText = FALSE;
	m_objDefaultColAtt.m_cBkColor = RGB(255,255,255);
	m_objDefaultColAtt.m_cTextColor = RGB(0,0,0);
	m_objDefaultColAtt.m_iColumnIndex = 0;
	m_objDefaultColAtt.m_iColumnWidth = kDefaultColumnWidth;
	m_objDefaultColAtt.m_iOriginalWidth = kDefaultColumnWidth;
	m_objDefaultColAtt.m_iRowIndex = 0;
	m_objDefaultColAtt.m_iTempWidth = kDefaultColumnWidth;
	m_objDefaultColAtt.m_sColumnHeading = L"";
	m_objDefaultColAtt.m_uiColumnFormat = LVCFMT_LEFT;
	m_objDefaultColAtt.m_uiItemFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	
	//Font
	m_objDefaultColAtt.m_lfcHeight= kDefaultItemTextHeight;
	m_objDefaultColAtt.m_lfcWidth=0;
	m_objDefaultColAtt.m_lfcEscapement=0;
	m_objDefaultColAtt.m_lfcOrientation=0;
	m_objDefaultColAtt.m_lfcWeight= FW_REGULAR;
	m_objDefaultColAtt.m_lfbItalic=0;//or 255
	m_objDefaultColAtt.m_lfbUnderline=0;
	m_objDefaultColAtt.m_lfbStrikeOut=0;
	m_objDefaultColAtt.m_lfiCharSet=0;
	m_objDefaultColAtt.m_lfiOutPrecision=0;
	m_objDefaultColAtt.m_lfiClipPrecision=0;
	m_objDefaultColAtt.m_lfiQuality= CLEARTYPE_NATURAL_QUALITY;
	m_objDefaultColAtt.m_lfiPitchAndFamily=0;
	m_objDefaultColAtt.m_lfpszFaceNam=L"Calibri";
	
	memset(&m_objDefaultColAtt.m_lfItemFont, 0, sizeof(LOGFONT));
	memcpy(&m_objDefaultColAtt.m_lfItemFont,&_lfDefaultFont,sizeof(LOGFONT));

	_bShowRowItemBorder = TRUE;
	_cBorderColorRowItem = RGB(255,0,0);
	_iItemHeight = kDefaultItemHeight;
	_bEnableAdjustHeight = FALSE;
	_rControlAreaRect.SetRectEmpty();
	_iPrevCtrlCX = _iPrevCtrlCX = 0;
	

	_bHeaderShow = TRUE;
	_bShowHVScrollBars = TRUE;
	_bEnableAdjustLastColumn = FALSE;
	_cItemsBackgroundColor = RGB(255,255,255);
	_rLastRectColumn.SetRectEmpty();

	_bShowRowAlternateColor = FALSE;
	_cRowFirstColor = RGB(255,255,255);
	_cRowSecondColor = RGB(255,255,255);
	_cVectorAlternateColorRows.clear();

	_bHighlightOnNewEntry = FALSE;
	_cRowHightlightForNewEntry = RGB(255,255,255);
	_iLockLastEntryItem = -1;
}

CExtListCtrl::~CExtListCtrl()
{
	if (_objHeaderCtrl.GetSafeHwnd()!=NULL) {
		_objHeaderCtrl.DestroyWindow();
	}
}

//used when default Insert column is used
void CExtListCtrl::fillColumnAttributes() {
	
	int nColumnsCount = 0;
	nColumnsCount = getColumnsCount();

	if (nColumnsCount>0 && ((int)m_vectorColumnAttList.size()<=0)) {

		HDITEM hditemLocal;
		SecureZeroMemory(&hditemLocal, sizeof(HDITEM));
		LV_COLUMN lvcLocal;
		::ZeroMemory(&lvcLocal, sizeof(lvcLocal));
		lvcLocal.mask = LVCF_WIDTH | LVCF_FMT;

		m_vectorColumnAttList.clear();

		//Fill the vector
		for (int j = 0;j < nColumnsCount;j++) {
			GetColumn(j, &lvcLocal);

			TCHAR buffer[MAX_PATH] = { 0 };
			hditemLocal.mask = HDI_TEXT;
			hditemLocal.pszText = buffer;
			hditemLocal.cchTextMax = MAX_PATH;
			GetHeaderCtrl()->GetItem(j, &hditemLocal);

			m_objColumnAttItem.m_iRowIndex = 0;
			m_objColumnAttItem.m_iColumnIndex = j;
			m_objColumnAttItem.m_cBkColor = RGB(255, 255, 255);
			m_objColumnAttItem.m_cTextColor = RGB(0, 0, 0);
			m_objColumnAttItem.m_sColumnHeading = CString(buffer);
			memset(&m_objColumnAttItem.m_lfItemFont, 0, sizeof(LOGFONT));
			memcpy(&m_objColumnAttItem.m_lfItemFont, &_lfDefaultFont, sizeof(LOGFONT));
			m_objColumnAttItem.m_bAutoFitText = FALSE;
			m_objColumnAttItem.m_uiColumnFormat = lvcLocal.fmt;
			m_objColumnAttItem.m_uiItemFormat = DT_VCENTER | DT_LEFT | DT_SINGLELINE;
			m_objColumnAttItem.m_iOriginalWidth = lvcLocal.cx;
			m_objColumnAttItem.m_iColumnWidth = lvcLocal.cx;
			m_objColumnAttItem.m_iTempWidth = 0;
			m_vectorColumnAttList.push_back(m_objColumnAttItem);

		}

	}


}

void CExtListCtrl::reInitializeControl() {

	//Intialization code goes here for custom control

}


BEGIN_MESSAGE_MAP(CExtListCtrl, CWnd)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_SIZE()
	//	ON_WM_ACTIVATE()
	ON_WM_NCCALCSIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(ADD_MESSAGE_HEADER, OnAddHeader)
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CListViewStatic message handlers

//Note : - This will work only when custom list is not dynamically created
void CExtListCtrl::PreSubclassWindow()
{
	
	//For dynamic list control we do post message so that it is processed after the header control is fully intialized otherwise gives null value
	PostMessage(ADD_MESSAGE_HEADER,0,0);
	
	CListCtrl::PreSubclassWindow();
}

void CExtListCtrl::onUserInterfaceChange() {
	CRect rc;
	GetWindowRect(&rc);
	_rControlAreaRect.CopyRect(rc);

	if (_bHeaderShow) {
		if (getColumnsCount()>0) {
			CRect rLocal;
			GetHeaderCtrl()->GetItemRect(getColumnsCount() - 1, &rLocal);
			_rLastRectColumn.CopyRect(rLocal);
			if (_bEnableAdjustLastColumn) {
				_objHeaderCtrl.setInternalMetrices((int)rLocal.Height(), (int)_rControlAreaRect.Width(), getColumnsCount(), _bEnableAdjustLastColumn);
			}
			_rControlAreaRect.DeflateRect(0, (int)(rLocal.Height()), 0, 0);
		}	
	}
	

	//For The Measure Item to work
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}


//-------------------------------------------- Column Header Members---------------------------------------------------//

CExtListHeader::CExtListHeader()
{
	_iHeightValueToStop = 0;
	_iWidthValueToStop = 0;
	_bEnableLastColumnAdjust = FALSE;
	_iNumberOfColumnsCount = 0;
	_lbBrushTransparent.lbStyle = BS_NULL;
	_cBrushTransparent.CreateBrushIndirect(&_lbBrushTransparent);
	_bShowHeaderSep =FALSE;
	_cHeaderSep =RGB(255,255,255);
	_cHeaderBg=RGB(0,0,0);
	_cHeaderTextClr= RGB(255, 255, 255);
}


CExtListHeader::~CExtListHeader()
{
}

void CExtListHeader::setInternalMetrices(int iInputValue1, int iInputValue2, int iInputValue3, BOOL bInputColumnAdjust) {

	_iWidthValueToStop = iInputValue2;
	_bEnableLastColumnAdjust = bInputColumnAdjust;
	_iHeightValueToStop = iInputValue1;
	_iNumberOfColumnsCount = iInputValue3;

}

void CExtListHeader::setHeaderAttributes(COLORREF cInputBg, COLORREF cInputText, BOOL bInputShowSep, COLORREF cInputSepClr) {

	_cHeaderSep = cInputSepClr;
	_cHeaderBg = cInputBg;
	_bShowHeaderSep = bInputShowSep;
	_cHeaderTextClr = cInputText;
	Invalidate(TRUE);

}

LONG CExtListHeader::getCalculatedTextHeight(CDC* pInputDC, int iInputHeight) {
	TEXTMETRIC tmLocal;
	HFONT hFontOld = (HFONT)SelectObject(pInputDC->m_hDC, GetFont()->GetSafeHandle());
	GetTextMetrics(pInputDC->m_hDC, &tmLocal);
	tmLocal.tmHeight = iInputHeight - (tmLocal.tmExternalLeading + 1);
	return tmLocal.tmHeight;
}

BEGIN_MESSAGE_MAP(CExtListHeader, CHeaderCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CExtListHeader::OnNMCustomDraw)
END_MESSAGE_MAP()


void CExtListHeader::OnNMCustomDraw(NMHDR *pInputNMHDR, LRESULT *pInputResult)
{
	LPNMCUSTOMDRAW pLocalNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pInputNMHDR);

	CDC* pLocalDC;
	pLocalDC = NULL;
	*pInputResult = CDRF_DODEFAULT;
	switch (pLocalNMCD->dwDrawStage)
	{
		case CDDS_PREPAINT: {
			CRect rLocalMain;
			pLocalDC = CDC::FromHandle(pLocalNMCD->hdc);
			GetClientRect(&rLocalMain);
			pLocalDC->FillSolidRect(&rLocalMain, _cHeaderBg);
			*pInputResult = CDRF_NOTIFYITEMDRAW;
		}
		break;
		case CDDS_ITEMPREPAINT:
		{
			HDITEM hdLocalItem;
			TCHAR Localbuffer[MAX_PATH] = { 0 };
			SecureZeroMemory(&hdLocalItem, sizeof(HDITEM));
			hdLocalItem.mask = HDI_TEXT;
			hdLocalItem.pszText = Localbuffer;
			hdLocalItem.cchTextMax = MAX_PATH;
			GetItem((int)pLocalNMCD->dwItemSpec, &hdLocalItem);
			pLocalDC = CDC::FromHandle(pLocalNMCD->hdc);
			pLocalDC->SetTextColor(_cHeaderTextClr);
			pLocalDC->SetBkColor(_cHeaderBg);
			CString strLocal(Localbuffer);

			//------------------ Apply Font for the Header Text -------------------//

			CFont *pOldLocalFont = NULL;
			CFont localColumnTitle;
			CFont *LocalFont = pLocalDC->GetCurrentFont();
			if (LocalFont)
			{
				LOGFONT lfLocalColumnTitle;
				LocalFont->GetLogFont(&lfLocalColumnTitle);
				lfLocalColumnTitle.lfWeight = FW_REGULAR;
				lfLocalColumnTitle.lfQuality = CLEARTYPE_NATURAL_QUALITY;
				lfLocalColumnTitle.lfHeight = getCalculatedTextHeight(pLocalDC, (int)CRect(pLocalNMCD->rc).Height());
				wcscpy_s(lfLocalColumnTitle.lfFaceName, L"Arial");

				localColumnTitle.CreateFontIndirect(&lfLocalColumnTitle);
				pOldLocalFont = pLocalDC->SelectObject(&localColumnTitle);
			}

			if (pOldLocalFont)
				pLocalDC->SelectObject(pOldLocalFont);


			pLocalDC->SelectObject(&localColumnTitle);
			CRect rLocalTextRect;
			rLocalTextRect.CopyRect(CRect(pLocalNMCD->rc));
			rLocalTextRect.DeflateRect(5, 0, 0, 0);
			pLocalDC->DrawText(strLocal, rLocalTextRect, DT_VCENTER | DT_LEFT);

			//Seperator
			if (_iNumberOfColumnsCount>0) {
				if (_bShowHeaderSep && ((_iNumberOfColumnsCount - 1)>((int)pLocalNMCD->dwItemSpec) ) ) {

					CPen *pOldLocalPen = NULL;

					CRect rLocalSep;
					rLocalSep.CopyRect(CRect(pLocalNMCD->rc));
					rLocalSep.DeflateRect(0, 0, 1, 0);

					CPen nNewLocalPen(PS_SOLID, 1, _cHeaderSep);

					pOldLocalPen = pLocalDC->SelectObject(&nNewLocalPen);
					pLocalDC->SelectObject(&_cBrushTransparent);

					//Draw Lines
					CPoint pLocalStart;
					CPoint pLocalEnd;

					//Right Border
					pLocalStart.x = (LONG)rLocalSep.right;
					pLocalStart.y = (LONG)rLocalSep.top;
					pLocalEnd.x = (LONG)rLocalSep.right;
					pLocalEnd.y = (LONG)rLocalSep.bottom;
					pLocalDC->MoveTo(pLocalStart);
					pLocalDC->LineTo(pLocalEnd);

					if (pOldLocalPen)
						pLocalDC->SelectObject(pOldLocalPen);
				}
			}
			*pInputResult = CDRF_SKIPDEFAULT;
		}
		break;
		default: {
			// it wasn't a notification that was interesting to us.
			*pInputResult = CDRF_DODEFAULT;
		}
		break;
	}
	if (pLocalDC != NULL) {
		::ReleaseDC(this->GetSafeHwnd(), pLocalDC->m_hDC);
	}
}


//----------------------------------------------------------------------------------------------------------------------//


void CExtListCtrl::DrawItem(LPDRAWITEMSTRUCT lpInputItem)
{

	TCHAR  lpBuffer[256];
	
	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = lpInputItem->itemID;
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));
	LV_COLUMN lvc, lvcprev;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;

	CDC* pDC;
	pDC = CDC::FromHandle(lpInputItem->hDC);
	int nCol=0;
	int nRow = 0;
	nRow = lpInputItem->itemID;
	CRect rRowRect;
	int iLocalBorderShift = 1;
	
	for (nCol = 0; GetColumn(nCol, &lvc); nCol++)
	{
		// Get the text 
		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpInputItem->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));

		GetSubItemRect(nRow,nCol, LVIR_BOUNDS, rRowRect);

		//Draw the separator
		if (_bShowRowItemBorder) {
			CPen *pOldPen = NULL;

			rRowRect.DeflateRect(0, 0, iLocalBorderShift, iLocalBorderShift);
			CRect rTemp;
			rTemp.CopyRect(rRowRect);

			CPen nNewPen(PS_SOLID, iLocalBorderShift, _cBorderColorRowItem);

			pOldPen = pDC->SelectObject(&nNewPen);
			pDC->SelectObject(&_BrushTransparent);

			//Draw Lines
			CPoint pStart;
			CPoint pEnd;

			//Right
			if (nCol<(getColumnsCount()-1)) {
				pStart.x = (LONG)rTemp.right;
				pStart.y = (LONG)rTemp.top;
				pEnd.x = (LONG)rTemp.right;
				if (nRow>=(GetItemCount() - 1)) {
					pEnd.y = (LONG)rTemp.bottom+5;
				}
				else {
					pEnd.y = (LONG)rTemp.bottom;
				}
				pDC->MoveTo(pStart);
				pDC->LineTo(pEnd);
			}

			//Bottom
			if (nRow<(GetItemCount()-1)) {
				pStart.x = (LONG)rTemp.right;
				pStart.y = (LONG)rTemp.bottom;
				pEnd.x = (LONG)rTemp.left;
				pEnd.y = (LONG)rTemp.bottom;
				pDC->MoveTo(pStart);
				pDC->LineTo(pEnd);
			}
			
			if (pOldPen)
				pDC->SelectObject(pOldPen);
		}

		//Apply the font
		if ((int)m_vectorColumnAttList.size()>0) {
			
			CFont *pOldFont = NULL;
			CFont fNewFont;
			// check if bold specified for subitem
			CFont *font = pDC->GetCurrentFont();
			if (font)
			{
				LOGFONT lf;
				font->GetLogFont(&lf);
				memcpy(&lf, &m_vectorColumnAttList[nCol].m_lfItemFont, sizeof(LOGFONT));

				if (m_vectorColumnAttList[nCol].m_bAutoFitText) {
					lf.lfHeight = getCalculateTxtHeight(pDC, _iItemHeight);
				}

				fNewFont.CreateFontIndirect(&lf);
				pOldFont = pDC->SelectObject(&fNewFont);
			}

			pDC->SetTextColor(m_vectorColumnAttList[nCol].m_cTextColor);

			BOOL bHoldText = FALSE;
			CRect rLocal;
			if (nRow >= (GetItemCount() - 1)) {			
				rLocal.CopyRect(rRowRect);
				rLocal.InflateRect(0,0,0,5);		
			}
			else {
				rLocal.CopyRect(rRowRect);
			}

			if (_iLockLastEntryItem >= 0) {
				if (_iLockLastEntryItem == nRow) {
					bHoldText = TRUE;
				}
			}

			COLORREF cLocalV;
			if (_bShowRowAlternateColor) {
				if (nRow<=((int)_cVectorAlternateColorRows.size()-1)) {
					cLocalV = _cVectorAlternateColorRows[nRow];
				}
			}else{
				cLocalV = m_vectorColumnAttList[nCol].m_cBkColor;
			}
			
			pDC->FillSolidRect(&rLocal, cLocalV);

			if (bHoldText == FALSE) {
				DrawText(pDC->m_hDC, lpBuffer, CString(lpBuffer).GetLength(), &rRowRect, m_vectorColumnAttList[nCol].m_uiItemFormat);
			}

			//Release old Font
			if (pOldFont != NULL) {
				pDC->SelectObject(pOldFont);
			}

		}
		else {
			pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));//For System Font

			pDC->FillSolidRect(&rRowRect, RGB(255, 0, 0));

			UINT  uFormat = DT_LEFT;
			DrawText(pDC->m_hDC, lpBuffer, CString(lpBuffer).GetLength(), &rRowRect, uFormat);

			pDC->SelectStockObject(SYSTEM_FONT);//For Sytem Font
		}
	}
}

LONG CExtListCtrl::getCalculateTxtHeight(CDC* pInputDC,int iInputBoxHeight) {
	TEXTMETRIC tm;
	HFONT hFontOld = (HFONT)SelectObject(pInputDC->m_hDC, GetFont()->GetSafeHandle());
	GetTextMetrics(pInputDC->m_hDC, &tm);
	tm.tmHeight = iInputBoxHeight - (tm.tmExternalLeading + 1);
	return tm.tmHeight;
}

void CExtListCtrl::recalculateRowHeight() {
	
	if (!_rControlAreaRect.IsRectEmpty()) {
		
		int iLocal_H = (int)(_rControlAreaRect.Height());
		if (GetItemCount()>0) {
			float fTemp = ((float)iLocal_H/(float)GetItemCount());
			int iSingleHeight = (int)(fTemp);
			_iItemHeight = iSingleHeight;
		}

	}

}

void CExtListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpItem)
{
	
	if (_bEnableAdjustHeight) {
		recalculateRowHeight();
	}

	lpItem->itemHeight = _iItemHeight;
	
	//Full width the last column
	reAdjustColumns();
}


int CExtListCtrl::getColumnsCount() {
	if (GetHeaderCtrl()!=NULL) {
		if (GetHeaderCtrl()->GetSafeHwnd() != NULL) {
			return GetHeaderCtrl()->GetItemCount();
		}
		return 0;
	}
	return 0;
}


void CExtListCtrl::reAdjustColumns() {

	int nColumnCount = getColumnsCount();
	if (nColumnCount <= 0) {
		return;
	}
	
	if (_bEnableAdjustLastColumn) {
		int iTotalColumnWidth = 0;
		int iActualWantedWidth = 0;
		int iLastColumnReq = 0;

		int i = (nColumnCount - 1);
		int iWidth = -1;
		int iReduce = -1;
		int iLocalV = 0;
		CRect rLocalArea;
		GetClientRect(&rLocalArea);
		do {
			iReduce++;

			iTotalColumnWidth = 0;
			if (nColumnCount>1) {
				for (int k = 0;k < (nColumnCount - 1);k++) {
					if (nColumnCount == (int)m_vectorColumnAttList.size()) {
						iActualWantedWidth += m_vectorColumnAttList[k].m_iOriginalWidth;
						if ((m_vectorColumnAttList[k].m_iColumnWidth - iReduce)>0) {
							m_vectorColumnAttList[k].m_iTempWidth = iLocalV = (m_vectorColumnAttList[k].m_iColumnWidth - iReduce);
						}
						else {
							m_vectorColumnAttList[k].m_iTempWidth = iLocalV = 0;
						}

						iTotalColumnWidth += iLocalV;
					}
				}
			}


			iWidth = (int)rLocalArea.Width() - iTotalColumnWidth;

		} while (/*iWidth<0 || */iWidth<1);

		SetColumnWidth(i, iWidth);

		//Success
		iLastColumnReq = ((int)rLocalArea.Width() - iActualWantedWidth);
		iActualWantedWidth += m_vectorColumnAttList[nColumnCount - 1].m_iOriginalWidth;
		for (int k = 0;k < (nColumnCount - 1);k++) {
			if (nColumnCount == (int)m_vectorColumnAttList.size()) {
				if (((int)rLocalArea.Width() >= iActualWantedWidth) && (iReduce <= 0)) {
					m_vectorColumnAttList[k].m_iColumnWidth = m_vectorColumnAttList[k].m_iOriginalWidth;
				}
				else {
					if (iReduce>0) {
						m_vectorColumnAttList[k].m_iColumnWidth = m_vectorColumnAttList[k].m_iTempWidth;
					}
				}
				SetColumnWidth(k, m_vectorColumnAttList[k].m_iColumnWidth);
			}
		}

		//Adjust Last column if any other column is shrinked
		if (iLastColumnReq>iWidth) {
			SetColumnWidth(i, iLastColumnReq);
		}

	}

}


BOOL CExtListCtrl::GetSubItemRect(int nInputItem, int nSubInputItem, int nInputArea, CRect& RectInput) {
	ASSERT(nInputItem >= 0);
	ASSERT(nInputItem < GetItemCount());
	if ((nInputItem < 0) || nInputItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubInputItem >= 0);
	ASSERT(nSubInputItem < getColumnsCount());
	if ((nSubInputItem < 0) || nSubInputItem >= getColumnsCount())
		return FALSE;

	BOOL bLocalRC = CListCtrl::GetSubItemRect(nInputItem, nSubInputItem, nInputArea, RectInput);

	/* if nSubInputItem == 0, 
			the rect by CListCtrl::GetSubItemRect is the full row hence use left edge of 2nd subitem */

	if (nSubInputItem == 0)
	{
		if (getColumnsCount() > 1)
		{
			CRect rectLocal_1;
			bLocalRC = GetSubItemRect(nInputItem, 1, LVIR_BOUNDS, rectLocal_1);
			RectInput.right = rectLocal_1.left;
		}
	}

	//Further +++
	if (nSubInputItem == 0)
	{
		if (getColumnsCount() > 1)
		{
			CRect rectLocal_1;
			// in case second column width = 0
			for (int i = 1; i < getColumnsCount(); i++)
			{
				bLocalRC = GetSubItemRect(nInputItem, i, LVIR_BOUNDS, rectLocal_1);
				if (rectLocal_1.Width() > 0)
				{
					RectInput.right = rectLocal_1.left;
					break;
				}
			}
		}
	}

	return bLocalRC;
}

BOOL CExtListCtrl::customDeleteColumn(int nColInput) {
	
	if (nColInput >= getColumnsCount() || ((int)m_vectorColumnAttList.size()<= nColInput)  || nColInput<0) {
		return FALSE;
	}

	DeleteColumn(nColInput);
	memset(&m_vectorColumnAttList[nColInput].m_lfItemFont, 0, sizeof(LOGFONT));
	m_vectorColumnAttList.erase(m_vectorColumnAttList.begin()+ nColInput);

	onUserInterfaceChange();

	return TRUE;
}

BOOL CExtListCtrl::customAddColumn(int nInputCol) {
	
	if (nInputCol<0) {
		return FALSE;
	}

	int iCurrentCount = getColumnsCount();

	if (iCurrentCount >= kMaxColumnCountLimit) {
		return FALSE;
	}

	m_objColumnAttItem = m_objDefaultColAtt;

	InsertColumn(nInputCol, m_objColumnAttItem.m_sColumnHeading, m_objColumnAttItem.m_uiColumnFormat, m_objColumnAttItem.m_iColumnWidth);///Fixed as Column is hidden
	
	m_vectorColumnAttList.push_back(m_objColumnAttItem);

	onUserInterfaceChange();

	return TRUE;

}

BOOL CExtListCtrl::setItemsHeight(int iInputHeight, BOOL bInputAutoFullHeight) {
	
	if (iInputHeight<kMinItemHeight || iInputHeight>kMaxItemHeight) {
		return FALSE;
	}

	if (_iItemHeight == iInputHeight || _bEnableAdjustHeight == bInputAutoFullHeight) {
		return FALSE;
	}

	_iItemHeight = iInputHeight;
	_bEnableAdjustHeight = bInputAutoFullHeight;
	onUserInterfaceChange();
	return TRUE;
}

void CExtListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);

	if ( (_iPrevCtrlCX != cx) || (_iPrevCtrlCX != cy) ) {
		_iPrevCtrlCX = cx;
		_iPrevCtrlCX = cy;
		onUserInterfaceChange();
	}
	
}

BOOL CExtListCtrl::bulkAddItems(int iInput, BOOL bFillInputWithEmptyText) {
	if (iInput < 0 || iInput>kMaxRowsCountLimit) {
		return FALSE;
	}
	if (getColumnsCount()<=0) {
		return FALSE;
	}

	DeleteAllItems();

	CString sTextLocal, sLocalData;
	for (int i = 0;i < iInput;i++) {
		sTextLocal.Format(L"%d", i);
		InsertItem(LVIF_TEXT | LVIF_STATE, i, sTextLocal, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0, 0);

		for (int k = 0;k < getColumnsCount();k++) {
			if (bFillInputWithEmptyText) {
				sLocalData = L"";
			}
			else {
				sLocalData.Format(L"Data (%d,%d)", i, k);
			}
			
			SetItemText(i, k, sLocalData);
		}
	}

	moveRowAlternateColors();

	return TRUE;
}


BOOL CExtListCtrl::modifyBulkAddItems(int iInput, BOOL bFillInputWithEmptyText, BOOL bInputTop) {
	
	int iCount = GetItemCount();

	if (iInput < 0 || iInput>kMaxRowsCountLimit || iCount== iInput) {
		return FALSE;
	}

	CString sLocalData;
	
	if (iInput>iCount) {//Add More
		int iHowMuchToAdd = (iInput - iCount);

		for (int i = 0;i < iHowMuchToAdd;i++) {
			if (bInputTop) {

				InsertItem(LVIF_TEXT | LVIF_STATE, i, L"", LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0, 0);

				for (int k = 0;k < getColumnsCount();k++) {
					if (bFillInputWithEmptyText) {
						sLocalData = L"";
					}
					else {
						sLocalData.Format(L"Data (%d,%d)", i, k);
					}

					SetItemText(i, k, sLocalData);
				}

			}
			else {
				
				InsertItem(LVIF_TEXT | LVIF_STATE, iCount+i, L"", LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0, 0);

				for (int k = 0;k < getColumnsCount();k++) {
					if (bFillInputWithEmptyText) {
						sLocalData = L"";
					}
					else {
						sLocalData.Format(L"Data (%d,%d)", iCount + i, k);
					}

					SetItemText(iCount + i, k, sLocalData);
				}

			}
		}
		
	}
	else {//Delete

		if (iCount<=0) {
			return FALSE;
		}

		int iHowMuchToDelete = (iCount - iInput);
		iCount--;
		for (int i = 0;i < iHowMuchToDelete;i++) {
			
			if (bInputTop) {
				DeleteItem(0);
			}
			else {
				DeleteItem(iCount-i);
			}

		}

	}

	moveRowAlternateColors();

	onUserInterfaceChange();

	return TRUE;
}

BOOL CExtListCtrl::pushDataToBottom(std::vector<CString>& InputList) {
	int nColumns = getColumnsCount();
	if (nColumns <=0) {
		return FALSE;
	}

	int iCount = GetItemCount();
	if (iCount<=0) {
		return FALSE;
	}

	int iItemPosition = -1;
	
	moveRowAlternateColors();
	refreshHighlightEntry(TRUE);

	BOOL bMoveDataOnly = FALSE;

	if (bMoveDataOnly) {
		if (iCount>1) {
			CString sLocal;
			for (int iStart = 1;iStart < iCount;iStart++) {

				for (int i = 0;i < nColumns;i++) {
					if (i<(int)InputList.size()) {
						sLocal = GetItemText(iStart, i);
						SetItemText((iStart - 1), i, sLocal);
					}
				}
			}
		}

		iItemPosition = (iCount - 1);

		if (iItemPosition>-1) {
			for (int i = 0;i < nColumns;i++) {
				if (i<(int)InputList.size()) {
					SetItemText(iItemPosition, i, InputList[i]);
				}
			}
		}
	}else{
		
		_iLockLastEntryItem = (iCount - 1);
		DeleteItem(0);
		CString sTextLocal;
		sTextLocal.Format(L"%d", (iCount-1));
		InsertItem(LVIF_TEXT | LVIF_STATE, (iCount - 1), sTextLocal, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0, 0);
		iItemPosition = (iCount - 1);
		for (int i = 0;i < nColumns;i++) {
			if (i<(int)InputList.size()) {
				SetItemText(iItemPosition, i, InputList[i]);
			}
		}
		_iLockLastEntryItem = -1;
		Invalidate(TRUE);
	}

	InputList.clear();


	
	return TRUE;
}

BOOL CExtListCtrl::pushDataToTop(std::vector<CString>& InputList) {
	int nColumns = getColumnsCount();
	if (nColumns <= 0) {
		return FALSE;
	}

	int iCount = GetItemCount();
	if (iCount <= 0) {
		return FALSE;
	}

	int iItemPosition = -1;

	moveRowAlternateColors();
	refreshHighlightEntry(FALSE);

	BOOL bMoveDataOnly = FALSE;

	if (bMoveDataOnly) {
		if (iCount>1) {

			CString sLocal;
			for (int iStart = (iCount - 2);iStart >= 0;iStart--) {

				for (int i = 0;i < nColumns;i++) {
					if (i<(int)InputList.size()) {
						sLocal = GetItemText(iStart, i);
						SetItemText((iStart + 1), i, sLocal);
					}
				}

			}

		}

		iItemPosition = 0;

		if (iItemPosition>-1) {
			for (int i = 0;i < nColumns;i++) {
				if (i<(int)InputList.size()) {
					SetItemText(iItemPosition, i, InputList[i]);
				}
			}
		}
	}
	else {
		
		_iLockLastEntryItem = 0;
		DeleteItem(iCount-1);
		iItemPosition = (iCount - 1);
		CString sTextLocal;
		sTextLocal.Format(L"%d", 0);
		InsertItem(LVIF_TEXT | LVIF_STATE, 0, sTextLocal, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0, 0);
		
		for (int i = 0;i < nColumns;i++) {
			if (i<(int)InputList.size()) {
				SetItemText(0, i, InputList[i]);
			}
		}
		_iLockLastEntryItem = -1;
		Invalidate(TRUE);

	}

	InputList.clear();
	
	
	return TRUE;
}


void CExtListCtrl::setItemsBorder(BOOL bInputEnable, COLORREF cInputColor) {
	
	_bShowRowItemBorder = bInputEnable;
	_cBorderColorRowItem = cInputColor;
	onUserInterfaceChange();

}

BOOL CExtListCtrl::getColumnAttribute(int iInput, ColumnAttribute& objInput) {
	
	if (iInput >= getColumnsCount()) {
		return FALSE;
	}

	objInput = m_vectorColumnAttList[iInput];

	return TRUE;
}


void CExtListCtrl::setHeaderAttribute(COLORREF cInputBg, COLORREF cInputText, BOOL bInputShowSep, COLORREF cInputSepClr) {
	if (_objHeaderCtrl.GetSafeHwnd()!=NULL) {
		_objHeaderCtrl.setHeaderAttributes(cInputBg, cInputText, bInputShowSep, cInputSepClr);
	}
	_bShowHeaderSep = bInputShowSep;
	_cHeaderSepColor = cInputSepClr;
	_cHeaderBgColor = cInputBg;
	_cHeaderTextColor = cInputText;
}


BOOL CExtListCtrl::setColumnAttribute(BOOL bApplyInputToAll, int nColInput, ColumnAttribute& objSetInput) {
	BOOL bIfAnyChange = FALSE;

	if (nColInput >= getColumnsCount() || ((int)m_vectorColumnAttList.size() <= nColInput) || nColInput<0) {
		return bIfAnyChange;
	}

	if (objSetInput.m_iOriginalWidth>kMaxColumnWidth || objSetInput.m_iOriginalWidth<kMinColumnWidth) {
		return bIfAnyChange;
	}

	int iStart = 0;

	int iColumnCount = getColumnsCount();
	if (bApplyInputToAll) {
		iStart = 0;
	}
	else {
		iStart = nColInput;
	}

	do {
		
		//Column Heading Value
		if (objSetInput.m_sColumnHeading.CompareNoCase(m_vectorColumnAttList[iStart].m_sColumnHeading) != 0) {

			LVCOLUMN lvLocalCol;
			::ZeroMemory((void *)&lvLocalCol, sizeof(LVCOLUMN));
			lvLocalCol.mask = LVCF_TEXT;
			GetColumn(iStart, &lvLocalCol);
			lvLocalCol.pszText = LPWSTR(objSetInput.m_sColumnHeading.GetString());
			SetColumn(iStart, &lvLocalCol);

			m_vectorColumnAttList[iStart].m_sColumnHeading = objSetInput.m_sColumnHeading;

			bIfAnyChange = TRUE;
		}

		//Item Format
		if (m_vectorColumnAttList[iStart].m_uiItemFormat != objSetInput.m_uiItemFormat) {
			m_vectorColumnAttList[iStart].m_uiItemFormat = objSetInput.m_uiItemFormat;
			bIfAnyChange = TRUE;
		}

		//Column Width
		if (m_vectorColumnAttList[iStart].m_iOriginalWidth != objSetInput.m_iOriginalWidth) {
			m_vectorColumnAttList[iStart].m_iOriginalWidth = objSetInput.m_iOriginalWidth;
			m_vectorColumnAttList[iStart].m_iColumnWidth = objSetInput.m_iOriginalWidth;
			if (_bEnableAdjustLastColumn ==FALSE) {
				SetColumnWidth(iStart, m_vectorColumnAttList[iStart].m_iOriginalWidth);
			}
			bIfAnyChange = TRUE;
		}

		//Item Bg Color
		if (m_vectorColumnAttList[iStart].m_cBkColor != objSetInput.m_cBkColor) {
			m_vectorColumnAttList[iStart].m_cBkColor = objSetInput.m_cBkColor;
			bIfAnyChange = TRUE;
		}

		//Item Text Color
		if (m_vectorColumnAttList[iStart].m_cTextColor != objSetInput.m_cTextColor) {
			m_vectorColumnAttList[iStart].m_cTextColor = objSetInput.m_cTextColor;
			bIfAnyChange = TRUE;
		}

		//Item Auto Fit BOOL
		if (m_vectorColumnAttList[iStart].m_bAutoFitText != objSetInput.m_bAutoFitText) {
			m_vectorColumnAttList[iStart].m_bAutoFitText = objSetInput.m_bAutoFitText;
			bIfAnyChange = TRUE;
		}

		iStart++;

	} while (bApplyInputToAll && iStart<iColumnCount);

	
	if (bIfAnyChange==TRUE) {
		//Full width the last column
		onUserInterfaceChange();
	}

	return bIfAnyChange;
}


BOOL CExtListCtrl::setColumnFontAttribute(BOOL bApplyInputToAll, int nColInput, LONG cHeight, LONG cWidth, LONG cEscapement, LONG cOrientation, LONG cWeight, BYTE bItalic, BYTE bUnderline, BYTE bStrikeOut, BYTE iCharSet, BYTE iOutPrecision, BYTE iClipPrecision, BYTE iQuality, BYTE iPitchAndFamily, CString pszFaceName){
	
	BOOL bIfAnyChange = FALSE;


	//Validations
	if (nColInput >= getColumnsCount() || ((int)m_vectorColumnAttList.size() <= nColInput) || nColInput<0) {
		return bIfAnyChange;
	}



	LOGFONT lfLocal;
	BOOL bSingleChange = FALSE;
	int iStart = 0;

	int iColumnCount = getColumnsCount();
	if (bApplyInputToAll) {
		iStart = 0;
	}
	else {
		iStart = nColInput;
	}

	do{
	
		bSingleChange = FALSE;
		memset(&lfLocal, 0, sizeof(LOGFONT));
		memcpy(&lfLocal, &m_vectorColumnAttList[iStart].m_lfItemFont, sizeof(LOGFONT));

		if (lfLocal.lfHeight != cHeight) {
			lfLocal.lfHeight = cHeight;
			m_vectorColumnAttList[iStart].m_lfcHeight = cHeight;
			bSingleChange = TRUE;
		}

		if (lfLocal.lfWeight != cWeight) {
			lfLocal.lfWeight = cWeight;
			m_vectorColumnAttList[iStart].m_lfcWeight = cWeight;
			bSingleChange = TRUE;
		}

		if (lfLocal.lfWidth != cWidth) {
			lfLocal.lfWidth = cWidth;
			m_vectorColumnAttList[iStart].m_lfcWidth = cWidth;
			bSingleChange = TRUE;
		}

		if (lfLocal.lfEscapement != cEscapement) {
			lfLocal.lfEscapement = cEscapement;
			m_vectorColumnAttList[iStart].m_lfcEscapement = cEscapement;
			bSingleChange = TRUE;
		}

		if (lfLocal.lfOrientation != cOrientation) {
			lfLocal.lfOrientation = cOrientation;
			m_vectorColumnAttList[iStart].m_lfcOrientation = cOrientation;
			bSingleChange = TRUE;
		}

		if (lfLocal.lfItalic != bItalic) {
			lfLocal.lfItalic = bItalic;
			m_vectorColumnAttList[iStart].m_lfbItalic = bItalic;
			bSingleChange = TRUE;
		}

		if (lfLocal.lfUnderline != bUnderline) {
			lfLocal.lfUnderline = bUnderline;
			m_vectorColumnAttList[iStart].m_lfbUnderline = bUnderline;
			bSingleChange = TRUE;
		}

		if (lfLocal.lfStrikeOut != bStrikeOut) {
			lfLocal.lfStrikeOut = bStrikeOut;
			m_vectorColumnAttList[iStart].m_lfbStrikeOut = bStrikeOut;
			bSingleChange = TRUE;
		}

		if (lfLocal.lfCharSet != iCharSet) {
			lfLocal.lfCharSet = iCharSet;
			m_vectorColumnAttList[iStart].m_lfiCharSet = iCharSet;
			bSingleChange = TRUE;
		}

		if (lfLocal.lfOutPrecision != iOutPrecision) {
			lfLocal.lfOutPrecision = iOutPrecision;
			m_vectorColumnAttList[iStart].m_lfiOutPrecision = iOutPrecision;
			bSingleChange = TRUE;
		}

		if (lfLocal.lfClipPrecision != iClipPrecision) {
			lfLocal.lfClipPrecision = iClipPrecision;
			m_vectorColumnAttList[iStart].m_lfiClipPrecision = iClipPrecision;
			bSingleChange = TRUE;
		}

		if (lfLocal.lfQuality != iQuality) {
			lfLocal.lfQuality = iQuality;
			m_vectorColumnAttList[iStart].m_lfiQuality = iQuality;
			bSingleChange = TRUE;
		}

		if (lfLocal.lfPitchAndFamily != iPitchAndFamily) {
			lfLocal.lfPitchAndFamily = iPitchAndFamily;
			m_vectorColumnAttList[iStart].m_lfiPitchAndFamily = iPitchAndFamily;
			bSingleChange = TRUE;
		}

		if (m_vectorColumnAttList[iStart].m_lfpszFaceNam.CompareNoCase(pszFaceName) != 0) {
			wcscpy_s(lfLocal.lfFaceName, (CString)pszFaceName);
			m_vectorColumnAttList[iStart].m_lfpszFaceNam = pszFaceName;
			bSingleChange = TRUE;
		}

		if (bSingleChange) {
			memcpy(&m_vectorColumnAttList[iStart].m_lfItemFont, &lfLocal, sizeof(LOGFONT));
			bIfAnyChange = TRUE;
		}

		iStart++;

	} while (bApplyInputToAll && iStart<iColumnCount);


	if (bIfAnyChange) {
		Invalidate(TRUE);
	}
	memset(&lfLocal, 0, sizeof(LOGFONT));

	return bIfAnyChange;
}

void CExtListCtrl::OnNcCalcSize(BOOL bCalcInputValid, NCCALCSIZE_PARAMS* lpInputNcsp)
{
	
	if (_bShowHVScrollBars ==FALSE) {
		ModifyStyle(WS_HSCROLL | WS_VSCROLL, 0);
	}

	CListCtrl::OnNcCalcSize(bCalcInputValid, lpInputNcsp);
}


void CExtListCtrl::allowScrollBars(BOOL bInput) {
	_bShowHVScrollBars = bInput;
}

void CExtListCtrl::setAdjustColumns(BOOL bInput) {
	_bEnableAdjustLastColumn = bInput;
}


//For removing flickering
BOOL CExtListCtrl::OnEraseBkgnd(CDC* pInputDC)
{
	return FALSE;
	//return CListCtrl::OnEraseBkgnd(pInputDC);
}

//To Remove Flickering the OnPaint() method is modified
void CExtListCtrl::OnPaint()
{
	CPaintDC dc(this); // DC for painting
					   
	// Do not call CListCtrl::OnPaint() for painting messages
	CRect rect;
	GetClientRect(&rect);
	
	CMemExtDC memDC(&dc,&rect);

	if (_bHeaderShow) {
		CRect headerRect;
		GetDlgItem(0)->GetWindowRect(&headerRect);
		ScreenToClient(&headerRect);
		if (_bEnableAdjustLastColumn == FALSE && !_rLastRectColumn.IsRectEmpty()) {
			headerRect.right = _rLastRectColumn.right;
		}
		dc.ExcludeClipRect(&headerRect);		
	}
	

	CRect clip;
	memDC.GetClipBox(&clip);
	memDC.FillSolidRect(clip, _cItemsBackgroundColor);

	DefWindowProc(WM_PAINT, (WPARAM)memDC->m_hDC, (LPARAM)0);
}


void CExtListCtrl::setControlBgColor(COLORREF cInput) {
	_cItemsBackgroundColor = cInput;
}

void CExtListCtrl::setAllItemsDataEmpty() {
	
	if (GetItemCount()<=0 || getColumnsCount()<=0) {
		return;
	}

	for (int iStart = 0;iStart < GetItemCount();iStart++) {
		for (int i = 0;i < getColumnsCount();i++) {
			SetItemText(iStart, i, L"");
		}
	}

}

LRESULT CExtListCtrl::OnAddHeader(WPARAM wParam, LPARAM lParam) {
	if (_bHeaderShow) {
		ModifyStyle(0, /*LVS_NOCOLUMNHEADER |*/ LVS_REPORT | WS_TABSTOP | LVS_ALIGNLEFT, 0);

		CHeaderCtrl* pHeader = NULL;
		pHeader = GetHeaderCtrl();
		if (pHeader != NULL)
		{
			VERIFY(_objHeaderCtrl.SubclassWindow(pHeader->m_hWnd)); // m_HeaderCtrl is the new wrapper object
			_objHeaderCtrl.setHeaderAttributes(_cHeaderBgColor, _cHeaderTextColor, _bShowHeaderSep, _cHeaderSepColor);
		}
	}
	else {
		ModifyStyle(0, LVS_NOCOLUMNHEADER | LVS_REPORT | WS_TABSTOP | LVS_ALIGNLEFT, 0);
	}

	return 1;
}

void CExtListCtrl::setRowAlternateColor(BOOL bInput, COLORREF cInputColorFirst, COLORREF cInputColorSecond) {
	
	_bShowRowAlternateColor = bInput;
	_cRowFirstColor = cInputColorFirst;
	_cRowSecondColor = cInputColorSecond;

}

void CExtListCtrl::moveRowAlternateColors() {
	
	if (GetItemCount()<=0 || _bShowRowAlternateColor ==FALSE) {
		return;
	}

	int iLocalCount = (int)_cVectorAlternateColorRows.size();
	int iItemCount = GetItemCount();
	if (iLocalCount!= iItemCount) {
		
		if (iItemCount>iLocalCount) {
			//New Additions
			while ((int)_cVectorAlternateColorRows.size()<iItemCount) {
				_cVectorAlternateColorRows.push_back(_cRowFirstColor);
			}
		}
		else {
			
			//Delete Items
			int iLocalTemp = (int)_cVectorAlternateColorRows.size() - 1;
			while ((int)_cVectorAlternateColorRows.size()>iItemCount) {
				_cVectorAlternateColorRows.erase(_cVectorAlternateColorRows.begin()+ iLocalTemp);
				iLocalTemp--;
			}

		}

		BOOL bAlternate = TRUE;
		for (int k = 0;k < (int)_cVectorAlternateColorRows.size();k++) {
			if (bAlternate) {
				_cVectorAlternateColorRows[k] = _cRowFirstColor;
				bAlternate = FALSE;
			}
			else {
				_cVectorAlternateColorRows[k] = _cRowSecondColor;
				bAlternate = TRUE;
			}
		}

	}
	else {
	
		for (int k = 0;k < (int)_cVectorAlternateColorRows.size();k++) {
			if (_cVectorAlternateColorRows[k]== _cRowFirstColor) {
				_cVectorAlternateColorRows[k] = _cRowSecondColor;
			}
			else {
				_cVectorAlternateColorRows[k] = _cRowFirstColor;
			}
		}

	}
}


void CExtListCtrl::setRowHightlight(BOOL bInput, COLORREF cInputBgColor) {
	
	_bHighlightOnNewEntry = bInput;
	_cRowHightlightForNewEntry = cInputBgColor;

}

void CExtListCtrl::OnTimer(UINT_PTR nIDInputEvent)
{
	switch (nIDInputEvent)
	{

	case 1: {

		if (GetItemCount() > 0 && (int)_cVectorAlternateColorRows.size() > 0) {
			int iCount = (int)_cVectorAlternateColorRows.size();
			if ((iCount-2)>=0) {
				if (_cVectorAlternateColorRows[iCount - 2]== _cRowFirstColor) {
					_cVectorAlternateColorRows[iCount - 1] = _cRowSecondColor;
				}
				else {
					_cVectorAlternateColorRows[iCount - 1] = _cRowFirstColor;
				}
			}
			else {
				_cVectorAlternateColorRows[iCount - 1] = _cRowFirstColor;
			}
			Invalidate(TRUE);
		}
		KillTimer(1);
		
		break;
	}
	case 2: {
		
		if (GetItemCount() > 0 && (int)_cVectorAlternateColorRows.size() > 0) {
			int iCount = (int)_cVectorAlternateColorRows.size();
			if (iCount<=1) {
				_cVectorAlternateColorRows[0] = _cRowFirstColor;
			}
			else {
				if (_cVectorAlternateColorRows[1] == _cRowFirstColor) {
					_cVectorAlternateColorRows[0] = _cRowSecondColor;
				}
				else {
					_cVectorAlternateColorRows[0] = _cRowFirstColor;
				}
			}
			Invalidate(TRUE);
		}
		KillTimer(2);

		break;
	}
	default:
		break;
	}

	CListCtrl::OnTimer(nIDInputEvent);
}

void CExtListCtrl::refreshHighlightEntry(BOOL bInputFromBottom) {

	if (GetItemCount()<=0 || (int)_cVectorAlternateColorRows.size()<=0 || _bHighlightOnNewEntry ==FALSE) {
		return;
	}

	KillTimer(1);
	KillTimer(2);
	if (bInputFromBottom) {
		int iCount = (int)_cVectorAlternateColorRows.size();
		_cVectorAlternateColorRows[iCount - 1] = _cRowHightlightForNewEntry;
		SetTimer(1, 3000, NULL);
	}
	else {	
		int iCount = (int)_cVectorAlternateColorRows.size();
		_cVectorAlternateColorRows[0] = _cRowHightlightForNewEntry;
		SetTimer(2, 3000, NULL);
	}
	
	
}
