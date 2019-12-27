
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

#define TWO_IMAGES          1
#define THREE_IMAGES        2
#define TWO_IMAGES_SCALED   4
#define MORPHING_INPUT       8
#define MORPHING            16

//histogram�׸��� ������� üũ
bool isHisto = false;

// CImageProView

IMPLEMENT_DYNCREATE(CImageProView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_SUB_ADD, &CImageProView::OnSubAdd)
	ON_COMMAND(ID_SUB_MUL, &CImageProView::OnSubMul)
	ON_COMMAND(ID_HISTOGRAM, &CImageProView::OnHistogram)
	ON_COMMAND(ID_STRETCHING, &CImageProView::OnStretching)
	ON_COMMAND(ID_NEGATIVETRANSFORMATION, &CImageProView::OnNegativetransformation)
	ON_COMMAND(ID_BINARYIMAGE, &CImageProView::OnBinaryimage)
	ON_COMMAND(ID_EMBOSSING_SUB_45, &CImageProView::OnEmbossingSub45)
	ON_COMMAND(ID_EMBOSSING_SUB_90, &CImageProView::OnEmbossingSub90)
	ON_COMMAND(ID_EMBOSSING_SUB_135, &CImageProView::OnEmbossingSub135)
	ON_COMMAND(ID_AVERAGE_SUB_3X3, &CImageProView::OnAverageSub3x3)
	ON_COMMAND(ID_AVERAGE_SUB_5X5, &CImageProView::OnAverageSub5x5)
	ON_COMMAND(ID_SHARPENING_SUB_MASK1, &CImageProView::OnSharpeningSubMask1)
	ON_COMMAND(ID_SHARPENING_SUB_MASK2, &CImageProView::OnSharpeningSubMask2)
	ON_COMMAND(ID_SHARPNING_SUB_MASK3, &CImageProView::OnSharpningSubMask3)
	ON_COMMAND(ID_EDGE_SUB_SOBEL, &CImageProView::OnEdgeSubSobel)
	ON_COMMAND(ID_EDGE_SUB_PREWITT, &CImageProView::OnEdgeSubPrewitt)
	ON_COMMAND(ID_EDGE_SUB_ROBERTS, &CImageProView::OnEdgeSubRoberts)
	ON_COMMAND(ID_MEDIAN, &CImageProView::OnMedian)
	ON_COMMAND(ID_SAVE, &CImageProView::OnSave)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_ZOOMOUT, &CImageProView::OnGeometryZoominZoomout)
	ON_COMMAND(ID_GEOMETRY_ROTATE, &CImageProView::OnGeometryRotate)
	ON_COMMAND(ID_GEOMETRY_ORIGIN, &CImageProView::OnGeometryOrigin)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_GEOMETRY_MORPHING, &CImageProView::OnGeometryMorphing)
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

#define NUM_FRAME 6

// CImageProView �׸���

void CImageProView::OnDraw(CDC* pDC)
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->inputImg==NULL) return;

	if (isHisto) {
		for (int y = 0; y < 256; y++)       // histogram ���
			for (int x = 0; x < 256; x++)
				pDC->SetPixel(x, y + pDoc->imageHeight + 30, RGB(pDoc->histoImg[y][x],
					pDoc->histoImg[y][x],
					pDoc->histoImg[y][x]));
		isHisto = false;
	}

	if (viewMode == MORPHING) { // 8�� ȭ���� �׸��� �۾�
		for (int y = 0; y < pDoc->imageHeight; y++)       // �Է� ���� ��� 
			for (int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],
					pDoc->inputImg[y][x],
					pDoc->inputImg[y][x]));

		for (int i = 0; i < NUM_FRAME; i++)
			for (int y = 0; y < pDoc->imageHeight; y++)       // ���� ��� ���
				for (int x = 0; x < pDoc->imageWidth; x++) {
					if (i < 3) {
						pDC->SetPixel(x + pDoc->imageWidth * (i + 1) + (i + 1) * 30, y,
							RGB(pDoc->morphedImg[i][y][x],
								pDoc->morphedImg[i][y][x],
								pDoc->morphedImg[i][y][x]));
					}
					else {
						int tmp = i % 3;
						pDC->SetPixel(x + pDoc->imageWidth * tmp + tmp * 30, y + pDoc->imageHeight + 30,
							RGB(pDoc->morphedImg[i][y][x],
								pDoc->morphedImg[i][y][x],
								pDoc->morphedImg[i][y][x]));
					}
				}

		for (int y = 0; y< pDoc->imageHeight; y++)       // Ÿ�� ���� ���
			for (int x = 0; x< pDoc->imageWidth; x++)
				pDC->SetPixel(x + pDoc->imageWidth * 3 + 30 * 3, y + pDoc->imageHeight + 30,
					RGB(pDoc->inputImg2[y][x],
						pDoc->inputImg2[y][x],
						pDoc->inputImg2[y][x]));
	}
	else if (pDoc->depth == 1) {
		for (int y = 0; y < pDoc->imageHeight; y++)       // �Է� ���� ��� 
			for (int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],
					pDoc->inputImg[y][x],
					pDoc->inputImg[y][x]));

		if (viewMode == THREE_IMAGES) {
			for (int y = 0; y< pDoc->imageHeight; y++)       // �ι�° �Է� ���� ���
				for (int x = 0; x< pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->inputImg2[y][x],
							pDoc->inputImg2[y][x],
							pDoc->inputImg2[y][x]));
			for (int y = 0; y< pDoc->imageHeight; y++)       // ��� ���� ���
				for (int x = 0; x< pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth * 2 + 60, y,
						RGB(pDoc->resultImg[y][x],
							pDoc->resultImg[y][x],
							pDoc->resultImg[y][x]));
		}
		else if (viewMode == TWO_IMAGES_SCALED) {
			for (int y = 0; y< pDoc->gImageHeight; y++)  // ũ�Ⱑ ���� ��� ���� ���
				for (int x = 0; x< pDoc->gImageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->gResultImg[y][x],
							pDoc->gResultImg[y][x],
							pDoc->gResultImg[y][x]));

		}
		else if (viewMode == MORPHING_INPUT) {
			for (int y = 0; y< pDoc->imageHeight; y++)       // �ι�° �Է� ���� ���
				for (int x = 0; x< pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->inputImg2[y][x],
							pDoc->inputImg2[y][x],
							pDoc->inputImg2[y][x]));
		}
		else {
			for (int y = 0; y< pDoc->imageHeight; y++)       // ��� ���� ���
				for (int x = 0; x< pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->resultImg[y][x],
							pDoc->resultImg[y][x],
							pDoc->resultImg[y][x]));
		}
	}
	else if (pDoc->depth == 3) {
		for (int y = 0; y < pDoc->imageHeight; y++)       // �Է� ���� ��� 
			for (int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][3 * x],
					pDoc->inputImg[y][3 * x + 1],
					pDoc->inputImg[y][3 * x + 2]));

		if (viewMode == THREE_IMAGES) {
			for (int y = 0; y< pDoc->imageHeight; y++)       // �ι�° �Է� ���� ���
				for (int x = 0; x< pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->inputImg2[y][3 * x],
							pDoc->inputImg2[y][3 * x + 1],
							pDoc->inputImg2[y][3 * x + 2]));
			for (int y = 0; y< pDoc->imageHeight; y++)       // ��� ���� ���
				for (int x = 0; x< pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth * 2 + 60, y,
						RGB(pDoc->resultImg[y][3 * x],
							pDoc->resultImg[y][3 * x + 1],
							pDoc->resultImg[y][3 * x + 2]));
		}
		else if (viewMode == TWO_IMAGES_SCALED) {
			for (int y = 0; y< pDoc->gImageHeight; y++)  // ũ�Ⱑ ���� ��� ���� ���
				for (int x = 0; x< pDoc->gImageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->gResultImg[y][3 * x],
							pDoc->gResultImg[y][3 * x + 1],
							pDoc->gResultImg[y][3 * x + 2]));

		}
		else {
			for (int y = 0; y< pDoc->imageHeight; y++)       // ��� ���� ���
				for (int x = 0; x< pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->resultImg[y][3 * x],
							pDoc->resultImg[y][3 * x + 1],
							pDoc->resultImg[y][3 * x + 2]));
		}
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
	isHisto = true;

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


void CImageProView::OnEmbossingSub45()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->EmbossingSub45();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnEmbossingSub90()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->EmbossingSub90();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnEmbossingSub135()
{
		CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->EmbossingSub135();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnAverageSub3x3()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->AverageSub3x3();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnAverageSub5x5()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->AverageSub5x5();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnSharpeningSubMask1()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->SharpeningSubMask1();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnSharpeningSubMask2()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->SharpeningSubMask2();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnSharpningSubMask3()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->SharpeningSubMask3();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnEdgeSubSobel()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->EdgeSubSobel();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnEdgeSubPrewitt()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->EdgeSubPrewitt();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnEdgeSubRoberts()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->EdgeSubRoberts();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnMedian()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->Median();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnSave()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->resultImg == NULL) return;

	pDoc->MySaveCustomState();
}


void CImageProView::OnGeometryZoominZoomout()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryZoom();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(TRUE);
}


void CImageProView::OnGeometryRotate()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryRotate();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(TRUE);
}


void CImageProView::OnGeometryOrigin()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryOrigin();
	viewMode = TWO_IMAGES;
	Invalidate(TRUE);
}

/////////////////// #4


bool isImageOpen = false; // 10���� ������ ����
bool isGetData = false; // �����͸� ������ 

#define LINE_SIZE 10

static POINT startPoint;
static POINT oldPoint;

POINT sourceStartData[LINE_SIZE] = { 0 };
POINT sourceEndData[LINE_SIZE] = { 0 };
POINT destStartData[LINE_SIZE] = { 0 };
POINT DestEndData[LINE_SIZE] = { 0 };

int sourceInd = 0;
int destInd = 0;

bool isStart = false;

CImageProDoc* pDocMor;

void CImageProView::OnGeometryMorphing()
{
	pDocMor = GetDocument();

	ASSERT_VALID(pDocMor);
	pDocMor->LoadTwoImages();

	viewMode = MORPHING_INPUT;
	Invalidate(TRUE);

	isImageOpen = true;
}

void CImageProView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (isImageOpen) {
		if (!isStart) {
			return;
		}
		// �巡�� ���� ����Ʈ ����
		startPoint.x = point.x;
		startPoint.y = point.y;

		oldPoint = startPoint;

		CScrollView::OnLButtonDown(nFlags, point);
	}
}


void CImageProView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (isImageOpen) {
		if (!isStart) {
			return;
		}
		if ((nFlags&&MK_LBUTTON) == MK_LBUTTON) {
			POINT endPoint;
			endPoint.x = point.x;
			endPoint.y = point.y;

			CClientDC dc(this);
			CPen pen1;

			dc.SetROP2(R2_XORPEN);
			pen1.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
			dc.SelectObject(&pen1);

			//�巡�׽�, ������ �巡�� �ߴ� ������ ����(�л�Ǿ� �׷����� �� ����)
			dc.MoveTo(startPoint.x, startPoint.y);
			dc.LineTo(oldPoint.x, oldPoint.y);

			//�巡�׽�, �� ǥ��
			dc.MoveTo(startPoint.x, startPoint.y);
			dc.LineTo(endPoint.x, endPoint.y);

			oldPoint = endPoint;
		}
		CScrollView::OnMouseMove(nFlags, point);
	}
}


void CImageProView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (isImageOpen) {
		if (!isStart) {
			isStart = true;
			return;
		}
		POINT endPoint;
		endPoint.x = point.x;
		endPoint.y = point.y;

		CClientDC dc(this);
		CPen pen1;

		dc.SetROP2(R2_COPYPEN);
		pen1.CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
		dc.SelectObject(&pen1);

		//���콺�� ������ ��, �������� ������ �̾���.
		dc.MoveTo(startPoint.x, startPoint.y);
		dc.LineTo(endPoint.x, endPoint.y);

		//��ǥ�� ����

		//�ε��� ���� ¦���� ��� = source ������
		//���� Ȧ���� ��� = dest ������
		if ((sourceInd + destInd) % 2 == 0) {
			sourceStartData[sourceInd].x = startPoint.x;
			sourceStartData[sourceInd].y = startPoint.y;
			sourceEndData[sourceInd].x = endPoint.x;
			sourceEndData[sourceInd].y = endPoint.y;
			sourceInd++;
		}
		else {
			destStartData[destInd].x = startPoint.x - 30 - pDocMor->imageWidth;
			destStartData[destInd].y = startPoint.y;
			DestEndData[destInd].x = endPoint.x - 30 - pDocMor->imageWidth;
			DestEndData[destInd].y = endPoint.y;
			destInd++;
		}

		if (destInd >= LINE_SIZE) {
			pDocMor->GeometryMorphing(sourceStartData, sourceEndData, destStartData, DestEndData);
			viewMode = MORPHING;
			Invalidate(TRUE);

			//////////////�ʱ�ȭ///////////////
			isImageOpen = false;
			isGetData = false;
			sourceStartData[LINE_SIZE] = { 0 };
			sourceEndData[LINE_SIZE] = { 0 };
			destStartData[LINE_SIZE] = { 0 };
			DestEndData[LINE_SIZE] = { 0 };

			sourceInd = 0;
			destInd = 0;

			isStart = false;
			//////////////////////////////////
		}
		CScrollView::OnLButtonUp(nFlags, point);
	}
}
