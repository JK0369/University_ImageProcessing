
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
	ON_COMMAND(ID_PIXEL_HISTO_EQ, &CImageProView::OnPixelHistoEq)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_ADD, &CImageProView::OnPixelTwoImageAdd)
	ON_COMMAND(ID_REGION_SHARPENING, &CImageProView::OnRegionSharpening)
	ON_COMMAND(ID_REGION_MEANING, &CImageProView::OnRegionMeaning)
	ON_COMMAND(ID_REGION_SOBEL, &CImageProView::OnRegionSobel)
	ON_COMMAND(ID_REGION_MEDIAN, &CImageProView::OnRegionMedian)
	ON_COMMAND(ID_EROSION, &CImageProView::OnErosion)
	ON_COMMAND(ID_DILATION, &CImageProView::OnDilation)
	ON_COMMAND(ID_OPENING, &CImageProView::OnOpening)
	ON_COMMAND(ID_CLOSING, &CImageProView::OnClosing)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_PIXEL_COPY, &CImageProView::OnGeometryZoominPixelCopy)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_INTERPOLATION, &CImageProView::OnGeometryZoominInterpolation)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_SUBSAMPLING, &CImageProView::OnGeometryZoomoutSubsampling)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_AVG, &CImageProView::OnGeometryZoomoutAvg)
	ON_COMMAND(ID_GEOMETRY_ROTATE, &CImageProView::OnGeometryRotate)
	ON_COMMAND(ID_GEOMETRY_MIRROR, &CImageProView::OnGeometryMirror)
	ON_COMMAND(ID_GEOMETRY_FLIP, &CImageProView::OnGeometryFlip)
	ON_COMMAND(ID_GEOMETRY_ORIGIN, &CImageProView::OnGeometryOrigin)
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
   
   if (pDoc->inputImg == NULL) return;

   if (pDoc->depth == 1) {
      for(int y=0; y < pDoc->imageHeight; y++)       // �Է� ���� ��� 
         for(int x=0; x < pDoc->imageWidth; x++) 
           pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],
                                 pDoc->inputImg[y][x],
                                 pDoc->inputImg[y][x]));

      if (viewMode == THREE_IMAGES) {
         for(int y=0; y< pDoc->imageHeight; y++)       // �ι�° �Է� ���� ���
            for(int x=0; x< pDoc->imageWidth; x++) 
               pDC->SetPixel(x+pDoc->imageWidth+30, y, 
                               RGB(pDoc->inputImg2[y][x],
                                    pDoc->inputImg2[y][x], 
                                    pDoc->inputImg2[y][x]));
         for(int y=0; y< pDoc->imageHeight; y++)       // ��� ���� ���
            for(int x=0; x< pDoc->imageWidth; x++) 
               pDC->SetPixel(x+pDoc->imageWidth*2+60, y, 
                               RGB(pDoc->resultImg[y][x],
                                    pDoc->resultImg[y][x], 
                                    pDoc->resultImg[y][x]));
      }
      else if (viewMode == TWO_IMAGES_SCALED) {
         for(int y=0; y< pDoc->gImageHeight; y++)   // ũ�Ⱑ ���� ��� ���� ���
            for(int x=0; x< pDoc->gImageWidth; x++) 
               pDC->SetPixel(x+pDoc->imageWidth+30, y, 
                               RGB(pDoc->gResultImg[y][x],
                                    pDoc->gResultImg[y][x], 
                                    pDoc->gResultImg[y][x]));

      }
      else {
         for(int y=0; y< pDoc->imageHeight; y++)       // ��� ���� ���
            for(int x=0; x< pDoc->imageWidth; x++) 
               pDC->SetPixel(x+pDoc->imageWidth+30, y, 
                               RGB(pDoc->resultImg[y][x],
                                    pDoc->resultImg[y][x], 
                                    pDoc->resultImg[y][x]));
      }
   }
   else if (pDoc->depth == 3) {
      for(int y=0; y < pDoc->imageHeight; y++)       // �Է� ���� ��� 
         for(int x=0; x < pDoc->imageWidth; x++) 
                  pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][3*x],
                                 pDoc->inputImg[y][3*x+1],
                                 pDoc->inputImg[y][3*x+2]));

      if (viewMode == THREE_IMAGES) {
         for(int y=0; y< pDoc->imageHeight; y++)       // �ι�° �Է� ���� ���
            for(int x=0; x< pDoc->imageWidth; x++) 
               pDC->SetPixel(x+pDoc->imageWidth+30, y, 
                               RGB(pDoc->inputImg2[y][3*x],
                                    pDoc->inputImg2[y][3*x+1], 
                                    pDoc->inputImg2[y][3*x+2]));
         for(int y=0; y< pDoc->imageHeight; y++)       // ��� ���� ���
            for(int x=0; x< pDoc->imageWidth; x++) 
               pDC->SetPixel(x+pDoc->imageWidth*2+60,y,
                               RGB(pDoc->resultImg[y][3*x],
                                    pDoc->resultImg[y][3*x+1], 
                                    pDoc->resultImg[y][3*x+2]));
      }
      else {
         for(int y=0; y< pDoc->imageHeight; y++)       // ��� ���� ���
            for(int x=0; x< pDoc->imageWidth; x++) 
               pDC->SetPixel(x+pDoc->imageWidth+30, y, 
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

	if (pDoc->inputImg == NULL) return; // �߰��� ����: �Է� ������ �ִ��� �˻�

    pDoc->PixelAdd();        // ���� 4 : ���� ��ü�� PointAdd() �Լ� ȣ��

	viewMode = TWO_IMAGES; // ���� 5 : ������ ���� ����

    Invalidate(FALSE);      // ���� 6 : ȭ���� �ٽ� �׸�


}


void CImageProView::OnPixelHistoEq()
{
    CImageProDoc* pDoc = GetDocument();   
    ASSERT_VALID(pDoc);
		
	if (pDoc->inputImg == NULL) return; // �߰��� ����: �Է� ������ �ִ��� �˻�

    pDoc->PixelHistoEq();      // CImageProDoc Ŭ������ PixelHistoEq() ȣ��
	viewMode = TWO_IMAGES;
    Invalidate(FALSE);      //ȭ�� ����.   

}


void CImageProView::OnPixelTwoImageAdd()
{
   CImageProDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   pDoc->PixelTwoImageAdd();

   viewMode = THREE_IMAGES;

   Invalidate(FALSE);

}


void CImageProView::OnRegionSharpening()
{
    CImageProDoc* pDoc = GetDocument();   
    ASSERT_VALID(pDoc);

    if (pDoc->inputImg == NULL) return;
    pDoc->RegionSharpening();      
    viewMode = TWO_IMAGES;
    Invalidate(FALSE); 

}


void CImageProView::OnRegionMeaning()
{
    CImageProDoc* pDoc = GetDocument();   
    ASSERT_VALID(pDoc);

    if (pDoc->inputImg == NULL) return;
    pDoc->RegionMeaning();      
    viewMode = TWO_IMAGES;
    Invalidate(FALSE); 

}


void CImageProView::OnRegionSobel()
{
    CImageProDoc* pDoc = GetDocument();   
    ASSERT_VALID(pDoc);

    if (pDoc->inputImg == NULL) return;
    pDoc->RegionSobel();      
    viewMode = TWO_IMAGES;
    Invalidate(FALSE); 

}


void CImageProView::OnRegionMedian()
{
    CImageProDoc* pDoc = GetDocument();   
    ASSERT_VALID(pDoc);

    if (pDoc->inputImg == NULL) return;
    pDoc->RegionMedian();      
    viewMode = TWO_IMAGES;
    Invalidate(FALSE);  

}


void CImageProView::OnErosion()
{
    CImageProDoc* pDoc = GetDocument();   
    ASSERT_VALID(pDoc);

    if (pDoc->inputImg == NULL) return;
    pDoc->Erosion();      
    viewMode = TWO_IMAGES;
    Invalidate(FALSE);  

}


void CImageProView::OnDilation()
{
    CImageProDoc* pDoc = GetDocument();   
    ASSERT_VALID(pDoc);

    if (pDoc->inputImg == NULL) return;
    pDoc->Dilation();      
    viewMode = TWO_IMAGES;
    Invalidate(FALSE); 

}


void CImageProView::OnOpening()
{
    CImageProDoc* pDoc = GetDocument();   
    ASSERT_VALID(pDoc);

    if (pDoc->inputImg == NULL) return;
    pDoc->Opening();      
    viewMode = TWO_IMAGES;
    Invalidate(FALSE);  

}


void CImageProView::OnClosing()
{
    CImageProDoc* pDoc = GetDocument();   
    ASSERT_VALID(pDoc);

    if (pDoc->inputImg == NULL) return;
    pDoc->Closing();      
    viewMode = TWO_IMAGES;
    Invalidate(FALSE);   

}


void CImageProView::OnGeometryZoominPixelCopy()
{
   CImageProDoc* pDoc = GetDocument();   
   ASSERT_VALID(pDoc);

   if (pDoc->inputImg == NULL) return;
   pDoc->GeometryZoominPixelCopy();      
   viewMode = TWO_IMAGES_SCALED;
   Invalidate(FALSE);  

}


void CImageProView::OnGeometryZoominInterpolation()
{
   CImageProDoc* pDoc = GetDocument();   
   ASSERT_VALID(pDoc);

   if (pDoc->inputImg == NULL) return;
   pDoc->GeometryZoominInterpolation();      
   viewMode = TWO_IMAGES_SCALED;
   Invalidate(FALSE); 

}


void CImageProView::OnGeometryZoomoutSubsampling()
{
   CImageProDoc* pDoc = GetDocument();   
   ASSERT_VALID(pDoc);

   if (pDoc->inputImg == NULL) return;
   pDoc->GeometryZoomoutSubsampling();      
   viewMode = TWO_IMAGES_SCALED;
   Invalidate(FALSE);

}


void CImageProView::OnGeometryZoomoutAvg()
{
   CImageProDoc* pDoc = GetDocument();   
   ASSERT_VALID(pDoc);

   if (pDoc->inputImg == NULL) return;
   pDoc->GeometryZoomoutAvg();      
   viewMode = TWO_IMAGES_SCALED;
   Invalidate(FALSE); 

}


void CImageProView::OnGeometryRotate()
{
   CImageProDoc* pDoc = GetDocument();   
   ASSERT_VALID(pDoc);

   if (pDoc->inputImg == NULL) return;
   pDoc->GeometryRotate();      
   viewMode = TWO_IMAGES_SCALED;
   Invalidate(FALSE);  

}


void CImageProView::OnGeometryMirror()
{
   CImageProDoc* pDoc = GetDocument();   
   ASSERT_VALID(pDoc);

   if (pDoc->inputImg == NULL) return;
   pDoc->GeometryMirror();      
   viewMode = TWO_IMAGES;
   Invalidate(FALSE);   

}


void CImageProView::OnGeometryFlip()
{
   CImageProDoc* pDoc = GetDocument();   
   ASSERT_VALID(pDoc);

   if (pDoc->inputImg == NULL) return;
   pDoc->GeometryFlip();      
   viewMode = TWO_IMAGES;
   Invalidate(FALSE);  
}


void CImageProView::OnGeometryOrigin()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryOrigin();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}
