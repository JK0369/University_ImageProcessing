
// ImageProView.cpp : CImageProView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
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

// CImageProView 생성/소멸

CImageProView::CImageProView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CImageProView::~CImageProView()
{
}

BOOL CImageProView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CImageProView 그리기

void CImageProView::OnDraw(CDC* pDC)
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->inputImg==NULL) return;

	if (pDoc->depth == 1) {
		for (int y = 0; y < pDoc->imageHeight; y++)    // 입력 영상 출력 
			for (int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],
					pDoc->inputImg[y][x],
					pDoc->inputImg[y][x]));

		for (int y = 0; y<pDoc->imageHeight; y++)       // 결과 영상 출력
			for (int x = 0; x<pDoc->imageWidth; x++)
				pDC->SetPixel(x + pDoc->imageWidth+30, y, RGB(pDoc->resultImg[y][x],
					pDoc->resultImg[y][x],
					pDoc->resultImg[y][x]));

		for (int y = 0; y<256; y++)       // histogram 출력
			for (int x = 0; x<256; x++)
				pDC->SetPixel(x, y + pDoc->imageHeight+30, RGB(pDoc->histoImg[y][x],
					pDoc->histoImg[y][x],
					pDoc->histoImg[y][x]));
	}
	else if (pDoc->depth == 3) {
		for (int y = 0; y<pDoc->imageHeight; y++)       // 컬러 영상 출력
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
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	
	sizeTotal.cx = 2048;     // 수정된 코드
    sizeTotal.cy = 1024;     // 수정된 코드 

	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CImageProView 인쇄


void CImageProView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageProView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CImageProView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CImageProView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
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


// CImageProView 진단

#ifdef _DEBUG
void CImageProView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProDoc* CImageProView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProDoc)));
	return (CImageProDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProView 메시지 처리기


void CImageProView::OnPixelAdd()
{
    CImageProDoc* pDoc;       // 문장 1 : pDoc 변수 선언

    pDoc = GetDocument();   // 문장 2 : 문서 객체에 대한 포인터 획득
    ASSERT_VALID(pDoc);    // 문장 3 : pDoc 변수의 오류 검증

    pDoc->PixelAdd();        // 문장 4 : 문서 객체의 PointAdd() 함수 호출

    Invalidate(FALSE);        // 문장 5 : 화면을 다시 그림

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
