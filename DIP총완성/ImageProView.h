
// ImageProView.h : CImageProView Ŭ������ �������̽�
//

#pragma once


class CImageProView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CImageProView();
	DECLARE_DYNCREATE(CImageProView)

// Ư���Դϴ�.
public:
	CImageProDoc * GetDocument() const;
	int viewMode;           // ���� ó�� �η�

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CImageProView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSubAdd();
	afx_msg void OnSubMul();
	afx_msg void OnHistogram();
	afx_msg void OnStretching();
	afx_msg void OnNegativetransformation();
	afx_msg void OnBinaryimage();
	afx_msg void OnEmbossingSub45();
	afx_msg void OnEmbossingSub90();
	afx_msg void OnEmbossingSub135();
	afx_msg void OnAverageSub3x3();
	afx_msg void OnAverageSub5x5();
	afx_msg void OnSharpeningSubMask1();
	afx_msg void OnSharpeningSubMask2();
	afx_msg void OnSharpningSubMask3();
	afx_msg void OnEdgeSubSobel();
	afx_msg void OnEdgeSubPrewitt();
	afx_msg void OnEdgeSubRoberts();
	afx_msg void OnMedian();
	afx_msg void OnSave();
	afx_msg void OnGeometryZoominZoomout();
	afx_msg void OnGeometryRotate();
	afx_msg void OnGeometryOrigin();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnGeometryMorphing();
};

#ifndef _DEBUG  // ImageProView.cpp�� ����� ����
inline CImageProDoc* CImageProView::GetDocument() const
   { return reinterpret_cast<CImageProDoc*>(m_pDocument); }
#endif

