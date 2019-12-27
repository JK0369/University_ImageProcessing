
// ImageProView.cpp : CImageProView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "ImagePro.h"
#endif

#include "ImageProDoc.h"
#include "ImageProView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProView

IMPLEMENT_DYNCREATE(CImageProView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_PIXEL_ADD, &CImageProView::OnPixelAdd)
	ON_COMMAND(ID_SUB_ADD, &CImageProView::OnSubAdd)
	ON_COMMAND(ID_SUB_MUL, &CImageProView::OnSubMul)
	ON_COMMAND(ID_HISTOGRAM, &CImageProView::OnHistogram)
	ON_COMMAND(ID_STRETCHING, &CImageProView::OnStretching)
	ON_COMMAND(ID_NEGATIVETRANSFORMATION, &CImageProView::OnNegativetransformation)
	ON_COMMAND(ID_BINARYIMAGE, &CImageProView::OnBinaryimage)
END_MESSAGE_MAP()

// CImageProView ����/�Ҹ�

CImageProView::CImageProView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CImageProView::~CImageProView()
{
}

BOOL CImageProView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CImageProView �׸���

void CImageProView::OnDraw(CDC* pDC)
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->inputImg==NULL) return;

	if (pDoc->depth == 1) {
		for (int y = 0; y < pDoc->imageHeight; y++)    // �Է� ���� ��� 
			for (int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],
					pDoc->inputImg[y][x],
					pDoc->inputImg[y][x]));

		for (int y = 0; y<pDoc->imageHeight; y++)       // ��� ���� ���
			for (int x = 0; x<pDoc->imageWidth; x++)
				pDC->SetPixel(x + pDoc->imageWidth+30, y, RGB(pDoc->resultImg[y][x],
					pDoc->resultImg[y][x],
					pDoc->resultImg[y][x]));

		for (int y = 0; y<256; y++)       // histogram ���
			for (int x = 0; x<256; x++)
				pDC->SetPixel(x, y + pDoc->imageHeight+30, RGB(pDoc->histoImg[y][x],
					pDoc->histoImg[y][x],
					pDoc->histoImg[y][x]));
	}
	else if (pDoc->depth == 3) {
		for (int y = 0; y<pDoc->imageHeight; y++)       // �÷� ���� ���
			for (int x = 0; x<pDoc->imageWidth; x++)
				pDC->SetPixel(x + pDoc->imageWidth+30, y, RGB(pDoc->histoImg[y][x],
					pDoc->histoImg[y][x],
					pDoc->histoImg[y][x]));
	}
}

void CImageProView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	
	sizeTotal.cx = 2048;     // ������ �ڵ�
    sizeTotal.cy = 1024;     // ������ �ڵ� 

	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CImageProView �μ�


void CImageProView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageProView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CImageProView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CImageProView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CImageProView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageProView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageProView ����

#ifdef _DEBUG
void CImageProView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProDoc* CImageProView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProDoc)));
	return (CImageProDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProView �޽��� ó����


void CImageProView::OnPixelAdd()
{
    CImageProDoc* pDoc;       // ���� 1 : pDoc ���� ����

    pDoc = GetDocument();   // ���� 2 : ���� ��ü�� ���� ������ ȹ��
    ASSERT_VALID(pDoc);    // ���� 3 : pDoc ������ ���� ����

    pDoc->PixelAdd();        // ���� 4 : ���� ��ü�� PointAdd() �Լ� ȣ��

    Invalidate(FALSE);        // ���� 5 : ȭ���� �ٽ� �׸�

}


void CImageProView::OnSubAdd()
{
	CImageProDoc* pDoc;

	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->SubAdd();

	Invalidate(FALSE);
}


void CImageProView::OnSubMul()
{
	CImageProDoc* pDoc;

	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->SubMul();

	Invalidate(FALSE);
}

void CImageProView::OnHistogram()
{
	CImageProDoc* pDoc;

	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Histogram();

	Invalidate(FALSE);
}


void CImageProView::OnStretching()
{
	CImageProDoc* pDoc;

	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->StretchingEedInSearch();

	Invalidate(FALSE);
}


void CImageProView::OnNegativetransformation()
{
	CImageProDoc* pDoc;

	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->NegativeTransformation();

	Invalidate(FALSE);
}


void CImageProView::OnBinaryimage()
{
	CImageProDoc* pDoc;

	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Binaryimage();

	Invalidate(FALSE);
}
