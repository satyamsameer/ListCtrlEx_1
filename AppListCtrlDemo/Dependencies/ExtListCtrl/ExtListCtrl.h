#pragma once
#include <vector>

/* 
  CExtListCtrl and CExtListHeader

  Note :- 
  1. Each cell is read only
  2. Appearance can be changed as whole and column wise
  3. Row wise UI modifications will be added in future

*/

/* Extended Header class for List Control */
class CExtListHeader :public CHeaderCtrl
{
	protected:DECLARE_MESSAGE_MAP()

	/* Private members */

private:

	/* Header background color attribute */
	COLORREF _cHeaderBg;

	/* Header control text color attribute */
	COLORREF _cHeaderTextClr;

	/* Transparent brush attributes (brushes) */
	CBrush _cBrushTransparent;
	LOGBRUSH _lbBrushTransparent;

	/* Show or hide header control (Type Boolean) (cannot be done through any member method) */
	BOOL _bShowHeaderSep;

	/* Header metrices attributes (for internal calculation use only) */
	int _iHeightValueToStop;
	int _iWidthValueToStop;

	/* Auto column adjust attribute (Boolean) */
	BOOL _bEnableLastColumnAdjust;

	/* Total number of columns created attribute (count) */
	int _iNumberOfColumnsCount;

	/* Header separator color attribute */
	COLORREF _cHeaderSep;
	
	/* Drawing Handler method */
	afx_msg void OnNMCustomDraw(NMHDR *pInputNMHDR, LRESULT *pInputResult);

public:

	/* Constructor method */
	CExtListHeader();

	/* Destructor method */
	~CExtListHeader();

	/* Method used to apply the header apperance related properties */
	void setHeaderAttributes(COLORREF cInputBg, COLORREF cInputText, BOOL bInputShowSep, COLORREF cInputSepClr);

	/* Method gives auto calculated cell height using item height */
	LONG getCalculatedTextHeight(CDC* pInputDC, int iInputHeight);
	
	/* Method to apply the header metrices related values */
	void setInternalMetrices(int iInputValue1,int iInputValue2,int iInputValue3,BOOL bInputColumnAdjust);

};

//Custom List Control Class
class CExtListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CExtListCtrl)

	/* Private Members */

private:

	/* Constant data members */

	const int kColumnWidthInitialOffset = 0;//Not used

	const int kDefaultItemTextHeight = 15;

	//----------------------------------------

	const int kDefaultItemHeight = 20;

	const int kMaxItemHeight = 100;

	const int kMinItemHeight = 3;

	//---------------------------------------

	const int kDefaultColumnWidth = 30;

	const int kMaxColumnWidth = 1000;

	const int kMinColumnWidth = 0;

	//--------------------------------------

	const int kMaxColumnCountLimit = 30;

	const int kMaxRowsCountLimit = 100;

	//--------------------------------------


	/* Header Control Object */
	CExtListHeader _objHeaderCtrl;

	/* Hide/Show attribute for header control */
	BOOL _bHeaderShow;

	/* Hide/Show header control attribute (not applicable through any member functions) */
	BOOL _bShowHeaderSep;

	/* Header seperator color attribute */
	COLORREF _cHeaderSepColor;

	/* Header background color attribute */
	COLORREF _cHeaderBgColor;

	/* Header text color attribute */
	COLORREF _cHeaderTextColor;


	/* Hide/Show horizontal and vertical scroll bars */
	BOOL _bShowHVScrollBars;

	/* Disable/Enable attribute for auto height adjustment for items */
	BOOL _bEnableAdjustHeight;

	/* Items height attribute */
	int _iItemHeight;

	/* Last column to auto fill the full width of control attribute */
	BOOL _bEnableAdjustLastColumn;

	/* Transparent brush attribute for drawing */
	CBrush _BrushTransparent;
	LOGBRUSH _lbBrushTransparent;

	/* Current drawing area attribute (excluding the Header if shown) */
	CRect _rControlAreaRect;

	/* Last coloumn rectangle or metrice value attribute */
	CRect _rLastRectColumn;

	/* Attribute for storing previous sizes cx and cy ( Compare for any change in control size ) */
	int _iPrevCtrlCX, _iPrevCtrlCY;
	
	/* Items default font attribute */
	LOGFONT _lfDefaultFont;

	/* Hide/Show items row wise border */
	BOOL _bShowRowItemBorder;

	/* Items border color attribute */
	COLORREF _cBorderColorRowItem;

	/* Items background color attribute */
	COLORREF _cItemsBackgroundColor;

	/* Show alternate colors or not attribute */
	BOOL _bShowRowAlternateColor;

	/* Colors to use as alternate rows */
	COLORREF _cRowFirstColor;
	COLORREF _cRowSecondColor;

	/* Show/Not Show highlight on row attribute */
	BOOL _bHighlightOnNewEntry;

	/* Highlight Row color attribute */
	COLORREF _cRowHightlightForNewEntry;

	/* Alternate row color list for rows */
	std::vector <COLORREF> _cVectorAlternateColorRows;

	/* Attribute to refresh row entry by Locking mechanism */
	int _iLockLastEntryItem;

	/* Method to get individual items rectangle */
	BOOL GetSubItemRect(int iInputItem, int iSubInputItem, int nInputArea, CRect& RectInput);

	/* Method to auto calculate text height using the rectangle of individual items */
	LONG getCalculateTxtHeight(CDC* pInputDC,int iInputBoxHeight);

	/* Method automatically adjust the columns when resized */
	void reAdjustColumns();

	/* Method to automatically fill columns attributes */
	void fillColumnAttributes();

	/* Method to automatically calculate items individual height on resized height wise */
	void recalculateRowHeight();

	
	/* Public Members */

public:

	/* Constructor Method */
	CExtListCtrl();

	/* Destructor Method */
	virtual ~CExtListCtrl();

	/* Structure to store attributes of a column */
	struct ColumnAttribute {

		/* Row Index */
		int m_iRowIndex;//Currently not usefull

		/* Column Index */
		int m_iColumnIndex;

		/* Text to show in column heading */
		CString m_sColumnHeading;

		/* Format for drawing column header text */
		UINT m_uiColumnFormat;
		UINT m_uiItemFormat;

		/* Font Attributes */
		LONG m_lfcHeight;
		LONG m_lfcWidth;
		LONG m_lfcEscapement;
		LONG m_lfcOrientation;
		LONG m_lfcWeight;
		BYTE m_lfbItalic;
		BYTE m_lfbUnderline;
		BYTE m_lfbStrikeOut;
		BYTE m_lfiCharSet;
		BYTE m_lfiOutPrecision;
		BYTE m_lfiClipPrecision;
		BYTE m_lfiQuality;
		BYTE m_lfiPitchAndFamily;
		CString m_lfpszFaceNam;

		LOGFONT m_lfItemFont;
		BOOL m_bAutoFitText;
		/*................*/

		/* Column header background color */
		COLORREF m_cBkColor;

		/* Column header text color */
		COLORREF m_cTextColor;

		/* Attributes used for column width and resizing */
		/* Have the original user set width value */
		int m_iOriginalWidth;

		/* Have current column Width */
		int m_iColumnWidth;

		/* Have temporary calculated width value */
		int m_iTempWidth;

	}m_objColumnAttItem;


	/* Column Attribute Object */
	ColumnAttribute m_objDefaultColAtt;

	/* Column Attributes list vector */
	std::vector<ColumnAttribute> m_vectorColumnAttList;

	/* Method for intializing custom control (currently not in use) */
	void reInitializeControl();

	/* Over writing the drawing methods */
	virtual void DrawItem(LPDRAWITEMSTRUCT lpInputItem);
	
	/* Over writing the MeasureItem() method for changing height */
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpItem);

	/* Method to get column count */
	int getColumnsCount();

	/* Method to Disable/Enable and set border color */
	void setItemsBorder(BOOL bInputEnable,COLORREF cInputColor);

	/* Method to Disable/Enable full height and item height */
	BOOL setItemsHeight(int iInputHeight,BOOL bInputAutoFullHeight);

	/* Method to set background color of control */
	void setControlBgColor(COLORREF cInput);
	
	/* Method to automatically re-size the control components and its dimensions */
	void onUserInterfaceChange();

	/* Method to add column with default properties */
	BOOL customAddColumn(int nInputCol);

	/* Method to delete already created control column */
	BOOL customDeleteColumn(int nColInput);
	
	/* Handler for tracking resizing of control */
	afx_msg void OnSize(UINT nType, int cx, int cy);

	/* Method to bulk add empty or filled items in control */
	BOOL bulkAddItems(int iInput,BOOL bFillInputWithEmptyText);

	/* Method to (Add/Delete) from top or bottom basically modify */
	BOOL modifyBulkAddItems(int iInput, BOOL bFillInputWithEmptyText,BOOL bInputTop);

	/* Method to push data row from bottom */
	BOOL pushDataToBottom(std::vector<CString>& InputList);

	/* Method to push data row from top */
	BOOL pushDataToTop(std::vector<CString>& InputList);

	/* Method to get attribute of already created column */
	BOOL getColumnAttribute(int iInput, ColumnAttribute& objInput);

	/* Method to apply an attribute to a column or all the columns */
	BOOL setColumnAttribute(BOOL bApplyInputToAll,int nColInput, ColumnAttribute& objSetInput);

	/* Method to apply font attributes to a column or all columns */
	BOOL setColumnFontAttribute(BOOL bApplyInputToAll, int nColInput, LONG cHeight, LONG cWidth, LONG cEscapement, LONG cOrientation, LONG cWeight, BYTE bItalic, BYTE bUnderline, BYTE bStrikeOut, BYTE iCharSet, BYTE iOutPrecision, BYTE iClipPrecision, BYTE iQuality, BYTE iPitchAndFamily, CString pszFaceName);

	/* Method to set column header appearance attribute */
	void setHeaderAttribute(COLORREF cInputBg, COLORREF cInputText, BOOL bInputShowSep, COLORREF cInputSepClr);

	/* Method handler for hiding vertical and horizontal scroll bars */
	afx_msg void OnNcCalcSize(BOOL bCalcInputValid, NCCALCSIZE_PARAMS* lpInputNcsp);

	/* Method to allow or dis-allow scroll bars */
	void allowScrollBars(BOOL bInput);

	/* Method to Disable/Enable last column full width  */
	void setAdjustColumns(BOOL bInput);

	/* Method to set all the items value in list to empty */
	void setAllItemsDataEmpty();

	/* Handler called when adding a header to list control */
	LRESULT OnAddHeader(WPARAM wParam, LPARAM lParam);

	/* Method to set row alternate coloring attributes */
	void setRowAlternateColor(BOOL bInput,COLORREF cInputColorFirst,COLORREF cInputColorSecond);

	/* Method to call whenever any data row is pushed either to top or bottom */
	void moveRowAlternateColors();

	/* Method to set row highlight attribute */
	void setRowHightlight(BOOL bInput, COLORREF cInputBgColor);

	/* Internal method to set UI of highlight row */
	void refreshHighlightEntry(BOOL bInputFromBottom);

	/* OnTimer() Handler to work with timers */
	afx_msg void OnTimer(UINT_PTR nIDInputEvent);

	/* Method handler for applying background color */
	afx_msg BOOL OnEraseBkgnd(CDC* pInputDC);

	/* OnPaint() Handler for painting background */
	afx_msg void OnPaint();


	/* Protected Members */
protected:
	DECLARE_MESSAGE_MAP()

	/* Virtual function over write and add the sub class to control */
	virtual void PreSubclassWindow();
};


