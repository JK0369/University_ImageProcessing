
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

#define TWO_IMAGES      1
#define THREE_IMAGES    2

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
	ON_COMMAND(ID_PIXEL_HISTO_EQ, &CImageProView::OnPixelHistoEq)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_ADD, &CImageProView::OnPixelTwoImageAdd)
	ON_COMMAND(ID_EMBOSSING_SUB_45, &CImageProView::OnEmbossingSub45)
	ON_COMMAND(ID_EMBOSSING_SUB_90, &CImageProView::OnEmbossingSub90)
	ON_COMMAND(ID_EMBOSSING_SUB_135, &CImageProView::OnEmbossingSub135)
	ON_COMMAND(ID_AVERAGE_SUB_3X3, &CImageProView::OnAverageSub3x3)
	ON_COMMAND(ID_AVERAGE_SUB_5X5, &CImageProView::OnAverageSub5x5)
	ON_COMMAND(ID_SHARPENING_SUB_MASK1, &CImageProView::OnSharpeningSubMask1)
	ON_COMMAND(ID_SHARPENING_SUB_MASK3, &CImageProView::OnSharpeningSubMask2)
	ON_COMMAND(ID_SHARPENING_SUB_MASK2, &CImageProView::OnSharpeningSubMask2)
	ON_COMMAND(ID_SHARPNING_SUB_MASK3, &CImageProView::OnSharpningSubMask3)
	ON_COMMAND(ID_EDGE_SUB_SOBEL, &CImageProView::OnEdgeSubSobel)
	ON_COMMAND(ID_EDGE_SUB_PREWITT, &CImageProView::OnEdgeSubPrewitt)
	ON_COMMAND(ID_EDGE_SUB_ROBERTS, &CImageProView::OnEdgeSubRoberts)
	ON_COMMAND(ID_MEDIAN, &CImageProView::OnMedian)
	ON_COMMAND(ID_SAVE, &CImageProView::OnSave)
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
   
   if (pDoc->inputImg == NULL) return; // 입력 영상이 읽히지 않았으면 종료

   if (pDoc->depth == 1) { // 흑백 영상 출력
       for(int y=0; y < pDoc->imageHeight; y++)       // 입력 영상 출력 
           for(int x=0; x < pDoc->imageWidth; x++) 
               pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],
                           pDoc->inputImg[y][x], pDoc->inputImg[y][x]));

       if (viewMode == THREE_IMAGES) {
           for(int y=0; y< pDoc->imageHeight; y++)   // 두번째 입력 영상 출력
              for(int x=0; x< pDoc->imageWidth; x++) 
                  pDC->SetPixel(x+pDoc->imageWidth+30,y,
                                  RGB(pDoc->inputImg2[y][x],
                                       pDoc->inputImg2[y][x], 
                                       pDoc->inputImg2[y][x]));
           for(int y=0; y< pDoc->imageHeight; y++)       // 결과 영상 출력
              for(int x=0; x< pDoc->imageWidth; x++) 
                 pDC->SetPixel(x+pDoc->imageWidth*2+60,y, 
                                   RGB(pDoc->resultImg[y][x],
                                        pDoc->resultImg[y][x], 
                                        pDoc->resultImg[y][x]));
       }
       else {
           for(int y=0; y< pDoc->imageHeight; y++)       // 결과 영상 출력
              for(int x=0; x< pDoc->imageWidth; x++) 
                  pDC->SetPixel(x+pDoc->imageWidth+30,y, 
                           RGB(pDoc->resultImg[y][x],
                                pDoc->resultImg[y][x], 
                                pDoc->resultImg[y][x]));
       }
    }
    else if (pDoc->depth == 3) { // 컬러 영상 출력
       for(int y=0; y < pDoc->imageHeight; y++)       // 입력 영상 출력 
          for(int x=0; x < pDoc->imageWidth; x++) 
             pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][3*x],
                pDoc->inputImg[y][3*x+1], pDoc->inputImg[y][3*x+2]));

       if (viewMode == THREE_IMAGES) {
           for(int y=0; y< pDoc->imageHeight; y++)   // 두번째 입력 영상 출력
              for(int x=0; x< pDoc->imageWidth; x++) 
                  pDC->SetPixel(x+pDoc->imageWidth+30,y,
                       RGB(pDoc->inputImg2[y][3*x],    
                            pDoc->inputImg2[y][3*x+1], 
                            pDoc->inputImg2[y][3*x+2]));
           for(int y=0; y< pDoc->imageHeight; y++)       // 결과 영상 출력
              for(int x=0; x< pDoc->imageWidth; x++) 
                  pDC->SetPixel(x+pDoc->imageWidth*2+60,y,
                       RGB(pDoc->resultImg[y][3*x],   
                            pDoc->resultImg[y][3*x+1], 
                            pDoc->resultImg[y][3*x+2]));
       }
       else {
           for(int y=0; y< pDoc->imageHeight; y++)       // 결과 영상 출력
              for(int x=0; x< pDoc->imageWidth; x++) 
                  pDC->SetPixel(x+pDoc->imageWidth+30,y, 
                       RGB(pDoc->resultImg[y][3*x],   
                            pDoc->resultImg[y][3*x+1],
                            pDoc->resultImg[y][3*x+2]));
       }
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

	if (pDoc->inputImg == NULL) return; // 추가된 문장: 입력 영상이 있는지 검사

    pDoc->PixelAdd();        // 문장 4 : 문서 객체의 PointAdd() 함수 호출

	viewMode = TWO_IMAGES; // 문장 5 : 연산의 종류 설정

    Invalidate(FALSE);      // 문장 6 : 화면을 다시 그림


}


void CImageProView::OnPixelHistoEq()
{
    CImageProDoc* pDoc = GetDocument();   
    ASSERT_VALID(pDoc);
		
	if (pDoc->inputImg == NULL) return; // 추가된 문장: 입력 영상이 있는지 검사

    pDoc->PixelHistoEq();      // CImageProDoc 클래스의 PixelHistoEq() 호출
	viewMode = TWO_IMAGES;
    Invalidate(FALSE);      //화면 갱신.   

}


void CImageProView::OnPixelTwoImageAdd()
{
   CImageProDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   pDoc->PixelTwoImageAdd();

   viewMode = THREE_IMAGES;

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
