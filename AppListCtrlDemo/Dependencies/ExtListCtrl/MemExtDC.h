#ifndef _MEMEXTDC_H_
#define _MEMEXTDC_H_

//////////////////////////////////////////////////
// CMemExtDC - Holds memory DC
//
// This class works on memory device context which fixes
// that when the control is drawn it will not flicker.
///////////////////////////////////////////////////

class CMemExtDC : public CDC {

	// Private Member Variables
private:	
	CBitmap		m_extbitmap;		// Bitmap variable for Off-screen 
	CBitmap*	m_extoldBitmap;		// Bitmap variable which is originally found in CMemExtDC
	CDC*		m_pExtDC;			// Saves CDC as a parameter in constructor
	CRect		m_ExtRect;			// Rectangle of the area on which to draw
	BOOL		m_bExtMemDC;		// Value is true if CDC is a type of a Memory DC

	// Public members
public:
	
	CMemExtDC(CDC* pParamDC, const CRect* pParamRect = NULL) : CDC()
	{
		ASSERT(pParamDC != NULL);

		// Initialize variables
		m_pExtDC = pParamDC;
		m_extoldBitmap = NULL;
		m_bExtMemDC = !pParamDC->IsPrinting();

		// Rectangle to draw
		if (pParamRect == NULL) {
			pParamDC->GetClipBox(&m_ExtRect);
		} else {
			m_ExtRect = *pParamRect;
		}

		if (m_bExtMemDC) {

			// Make Memory DC

			CreateCompatibleDC(pParamDC);
			pParamDC->LPtoDP(&m_ExtRect);

			m_extbitmap.CreateCompatibleBitmap(pParamDC, m_ExtRect.Width(), m_ExtRect.Height());
			m_extoldBitmap = SelectObject(&m_extbitmap);

			SetMapMode(pParamDC->GetMapMode());

			SetWindowExt(pParamDC->GetWindowExt());
			SetViewportExt(pParamDC->GetViewportExt());

			pParamDC->DPtoLP(&m_ExtRect);
			SetWindowOrg(m_ExtRect.left, m_ExtRect.top);

		} else {

			// Copy the parts of the current device context for printing
			m_bPrinting = pParamDC->m_bPrinting;
			m_hDC       = pParamDC->m_hDC;
			m_hAttribDC = pParamDC->m_hAttribDC;

		}

		// Fill the background with rectangle
		FillSolidRect(m_ExtRect, pParamDC->GetBkColor());
	}

	// Operator to use it as a pointer	
	CMemExtDC* operator->()
	{
		return this;
	}
	
	~CMemExtDC()
	{		
		if (m_bExtMemDC) {

			// Do copy the off-screen bitmap onto the screen to display
			m_pExtDC->BitBlt(m_ExtRect.left, m_ExtRect.top, m_ExtRect.Width(), m_ExtRect.Height(),this, m_ExtRect.left, m_ExtRect.top, SRCCOPY);
			
			//Original bitmap is swapped
			SelectObject(m_extoldBitmap);

		} else {

			//Fill the DC with a NULL value so that we cannot accidently delete handles associated with the CDC as an argument to the constructor
			m_hDC = m_hAttribDC = NULL;

		}	
	}
	
	// Operator to use it as a pointer		
	operator CMemExtDC*()
	{
		return this;
	}

};

#endif