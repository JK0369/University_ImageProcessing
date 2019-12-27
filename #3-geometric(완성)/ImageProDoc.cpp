
// ImageProDoc.cpp : CImageProDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.

#include "CMyDialogZoomIn.h"
#include "CMyDialogZoonOut.h"
#include "CMyDialogAngle.h"

#ifndef SHARED_HANDLERS
#include "ImagePro.h"
#endif

#include "ImageProDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageProDoc

IMPLEMENT_DYNCREATE(CImageProDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProDoc, CDocument)
END_MESSAGE_MAP()


// CImageProDoc 생성/소멸

CImageProDoc::CImageProDoc()
{
   inputImg = NULL;
   inputImg2 = NULL;
   resultImg = NULL;
   gResultImg = NULL;     // 추가되는 코드 
}

CImageProDoc::~CImageProDoc()
{
}

BOOL CImageProDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CImageProDoc serialization

void CImageProDoc::Serialize(CArchive& ar)
{
   if (ar.IsStoring())
   {
      
   }
   else
   {
      LoadImageFile(ar);
   }
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageProDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CImageProDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageProDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageProDoc 진단

#ifdef _DEBUG
void CImageProDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageProDoc 명령


void CImageProDoc::PixelAdd(void)
{
    int value=0;

    for(int y=0; y < imageHeight; y++)
        for(int x=0; x < imageWidth * depth; x++) {
            value = inputImg[y][x]+100; 
            if(value > 255) resultImg[y][x] = 255;
            else resultImg[y][x]=value;
        }

}


void CImageProDoc::PixelHistoEq(void)
{
    int x, y, i, k;
    int acc_hist = 0;      // 히스토그램의 합을 누적하는 변수
    float N = (float) imageHeight * imageWidth;   // 영상의 전체 픽셀 수
    int hist[256], sum[256];
   
    for(k=0; k<256; k++) hist[k]=0;
   
    // 명암값의 빈도수 조사
    for(y = 0; y < imageHeight; y++)
        for(x = 0; x < imageWidth; x++) {
           k = inputImg[y][x];
           hist[k] = hist[k]+1;
        }

    // 누적된 히스토그램 합 계산
    for(i=0; i<256; i++) {
       acc_hist = acc_hist + hist[i];
       sum[i] = acc_hist;
    } 

   for(y = 0; y < imageHeight; y++) 
       for(x = 0; x < imageWidth; x++) {
          k = inputImg[y][x];
          resultImg[y][x] = (unsigned char) (sum[k] / N * 255);
       }
}


void CImageProDoc::PixelTwoImageAdd(void)
{
   int value = 0;      

   LoadTwoImages();   

   for(int y=0; y<imageHeight; y++)
       for(int x=0; x<imageWidth * depth; x++) {
          value = inputImg[y][x] + inputImg2[y][x];
          if (value > 255) resultImg[y][x] = 255; 
          else resultImg[y][x] = value;
       }

}


void CImageProDoc::LoadTwoImages(void)
{
 
   CFile file;
   CFileDialog dlg(TRUE);

   AfxMessageBox("Select the First Image");      
   
   if(dlg.DoModal()==IDOK)    {
      file.Open(dlg.GetPathName(), CFile::modeRead);
      CArchive ar(&file, CArchive::load);
      LoadImageFile(ar);
	  file.Close();
   }
     
   AfxMessageBox("Select the Second Image");    
   
   if(dlg.DoModal()==IDOK)    {
      file.Open(dlg.GetPathName(), CFile::modeRead);
      CArchive ar(&file, CArchive::load);
      LoadSecondImageFile(ar);
	  file.Close();
   }
}


void CImageProDoc::LoadImageFile(CArchive& ar)
{
   int i, maxValue;
   char type[16], buf[256];
   CFile *fp = ar.GetFile();
   CString fname = fp->GetFilePath(); 

   // 파일의 헤더 읽기 
   if (strcmp(strrchr(fname, '.'), ".ppm") == 0 ||
      strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
      strcmp(strrchr(fname, '.'), ".PGM") == 0 ||         
      strcmp(strrchr(fname, '.'), ".pgm") == 0   ) 
   {
      ar.ReadString(type, 15);

      do {
         ar.ReadString(buf, 255);
      } while (buf[0] == '#');
      sscanf_s(buf, "%d %d", &imageWidth, &imageHeight);

      do {
         ar.ReadString(buf, 255);
      } while (buf[0] == '#');
      sscanf_s(buf, "%d", &maxValue);

      if (strcmp(type, "P5") == 0) depth = 1;
      else depth = 3;
   }
   else if (strcmp(strchr(fname, '.'), ".raw") == 0 ||
          strcmp(strchr(fname, '.'), ".RAW") == 0 ) 
   {

      if (fp->GetLength() != 256 * 256) {
         AfxMessageBox("256x256 크기의 파일만 사용가능합니다."); 
         return;
      }

      imageWidth = 256;
      imageHeight = 256;
      depth = 1;
   }

   // 기억장소 할당
   inputImg = (unsigned char **) malloc(imageHeight * sizeof(unsigned char *));
   resultImg = (unsigned char **) malloc(imageHeight * sizeof(unsigned char *));

   for (i = 0; i < imageHeight; i++) {
        inputImg[i] = (unsigned char *) malloc(imageWidth * depth);
        resultImg[i] = (unsigned char *) malloc(imageWidth * depth);
   }

   // 영상 데이터 읽기 
   for (i = 0; i < imageHeight; i++)
      ar.Read(inputImg[i], imageWidth*depth);

}


void CImageProDoc::LoadSecondImageFile(CArchive& ar)
{
   int i, maxValue;
   char type[16], buf[256];

   CFile *fp = ar.GetFile();
   CString fname = fp->GetFilePath();  

   // 파일의 헤더 읽기 
   if (strcmp(strrchr(fname, '.'), ".ppm") == 0 ||
      strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
      strcmp(strrchr(fname, '.'), ".PGM") == 0 ||         
      strcmp(strrchr(fname, '.'), ".pgm") == 0   ) 
   {
      ar.ReadString(type, 15);

      do {
         ar.ReadString(buf, 255);
      } while (buf[0] == '#');
      sscanf_s(buf, "%d %d", &imageWidth, &imageHeight);

      do {
         ar.ReadString(buf, 255);
      } while (buf[0] == '#');
      sscanf_s(buf, "%d", &maxValue);

      if (strcmp(type, "P5") == 0) depth = 1;
      else depth = 3;

      // sprintf(buf, "%s %d %d %d", type, imageWidth, imageHeight, maxValue);
   }
   else if (strcmp(strchr(fname, '.'), ".raw") == 0 ||
      strcmp(strchr(fname, '.'), ".RAW") == 0 ) {

      if (fp->GetLength() != 256 * 256) {
         AfxMessageBox("256x256 크기의 파일만 사용가능합니다."); 
         return;
      }

      imageWidth = 256;
      imageHeight = 256;
      depth = 1;
   }

   // 기억장소 할당
   inputImg2 = (unsigned char **) malloc(imageHeight * sizeof(unsigned char *));

   for (i = 0; i < imageHeight; i++) {
        inputImg2[i] = (unsigned char *) malloc(imageWidth * depth);
   }

   // 영상 데이터 읽기 
   for (i = 0; i < imageHeight; i++)
      ar.Read(inputImg2[i], imageWidth*depth);

}


void CImageProDoc::RegionSharpening(void)
{
   float kernel[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

   Convolve(inputImg, resultImg, imageWidth, imageHeight, kernel, 0, depth);

}


void CImageProDoc::Convolve(unsigned char ** inputImg, unsigned char ** resultImg, int cols, int rows, float mask[][3], int bias, int depth)
{
   int i, j, x, y;
   int red, green, blue;
   int sum;
   unsigned char **tmpImg;

   // 기억장소 할당
   tmpImg = (unsigned char **) malloc((imageHeight + 2)* sizeof(unsigned char *));

   for (i = 0; i < imageHeight + 2; i++)
        tmpImg[i] = (unsigned char *) malloc((imageWidth + 2) * depth);

   // 0-삽입을 위해 0으로 초기화
   for (y = 0; y < imageHeight + 2; y++)
       for (x = 0; x < (imageWidth + 2) * depth; x++)
           tmpImg[y][x] = 0;

   // 영상 복사 
   for (y = 1; y < imageHeight + 1; y++)
       for (x = 1; x < imageWidth + 1; x++)
          if (depth == 1) tmpImg[y][x] = inputImg[y-1][x-1];
          else if (depth == 3) {
             tmpImg[y][3*x] = inputImg[y-1][3*(x-1)];
             tmpImg[y][3*x+1] = inputImg[y-1][3*(x-1)+1];
             tmpImg[y][3*x+2] = inputImg[y-1][3*(x-1)+2];
          }

   for (y=0; y < imageHeight; y++)
        for (x=0; x < imageWidth; x++) {
            if (depth == 1) {
                sum=0;
                for (i=0; i<3; i++)
                    for (j=0; j<3; j++)
                        sum += (int) (tmpImg[y+i][x+j] * mask[i][j]);
   
                if (sum > 255) sum=255;
                if (sum < 0) sum = 0;
                resultImg[y][x] = (unsigned char) sum;
            }
            else if (depth == 3) {
                red = 0;
                green = 0;
                blue = 0;

                for (i=0; i<3; i++)
                    for (j=0; j<3; j++) {
                         red += (int) (tmpImg[y+i][3*(x+j)] * mask[i][j]);
                         green += (int) (tmpImg[y+i][3*(x+j)+1] * mask[i][j]);
                         blue += (int) (tmpImg[y+i][3*(x+j)+2] * mask[i][j]);
                    }
   
                if (red > 255) red = 255;
                if (red < 0) red = 0;
                if (green > 255) green = 255;
                if (green < 0) green = 0;
                if (blue > 255) blue = 255;
                if (blue < 0) blue = 0;

                resultImg[y][3*x] = (unsigned char) red;
                resultImg[y][3*x+1] = (unsigned char) green;
                resultImg[y][3*x+2] = (unsigned char) blue;
            }
    }

   // 기억장소 반환 
   for (i = 0; i < imageHeight + 2; i++) free(tmpImg[i]);
   free(tmpImg);

}


void CImageProDoc::RegionMeaning(void)
{
   float mask[3][3] = {{1/9.0f, 1/9.0f, 1/9.0f}, 
                     {1/9.0f, 1/9.0f, 1/9.0f}, 
                     {1/9.0f, 1/9.0f, 1/9.0f}};

   Convolve(inputImg, resultImg, imageWidth, imageHeight, mask, 0, depth);

}


#include <math.h>


void CImageProDoc::RegionSobel(void)
{
   int i, x, y, sum;
   float mask1[3][3] = {1, 0, -1, 
                     2, 0, -2,
                     1, 0, -1};
   float mask2[3][3] = {-1, -2, -1,
                     0,  0,  0,
                     1,  2,  1};
   unsigned char **Er, **Ec;

   // 기억장소 할당
   Er = (unsigned char **) malloc((imageHeight)* sizeof(unsigned char *));
   Ec = (unsigned char **) malloc((imageHeight)* sizeof(unsigned char *));

    for (i = 0; i < imageHeight; i++) {
        Er[i] = (unsigned char *) malloc(imageWidth * depth);
        Ec[i] = (unsigned char *) malloc(imageWidth * depth);
   }

   Convolve(inputImg, Er, imageWidth, imageHeight, mask1, 0,  depth);
   Convolve(inputImg, Ec, imageWidth, imageHeight, mask2, 0,  depth);

   for (y = 0; y < imageHeight; y++) 
      for (x = 0; x < imageWidth * depth; x++) {
          sum = (int) sqrt( (float) (Er[y][x]*Er[y][x] + Ec[y][x]*Ec[y][x]));
          if (sum > 255) sum = 255;
          if (sum < 0) sum = 0;
          resultImg[y][x] = (unsigned char) sum;
   }

}


void CImageProDoc::RegionMedian(void)
{
   int i, j, temp, x, y;
   int n[9];

   for (y = 1; y < imageHeight - 1; y++)
       for (x = 1; x < imageWidth - 1; x++) 
       {
           n[0] = inputImg[y-1][x-1];
           n[1] = inputImg[y-1][x];
           n[2] = inputImg[y-1][x+1];
           n[3] = inputImg[y][x-1];
           n[4] = inputImg[y][x];
           n[5] = inputImg[y][x+1];
           n[6] = inputImg[y+1][x-1];
           n[7] = inputImg[y+1][x];
           n[8] = inputImg[y+1][x+1];
      
           // 버블 정렬(bubble sorting)   
           for (i = 8; i > 0; i--)
               for (j = 0; j < i; j++)
                   if (n[j] > n[j+1]) {
                       temp = n[j+1];
                       n[j+1] = n[j];
                       n[j] = temp;
                   }
      
            resultImg[y][x] = n[4];   // 중간값을 결과 영상에 저장
       }
}


void CImageProDoc::Erosion(void)
{
    int x, y, min;

    for (y = 1; y < imageHeight - 1; y++)
       for (x = 1; x < imageWidth - 1; x++) {
         min = 255;
         if (inputImg[y-1][x-1]   < min) min = inputImg[y-1][x-1];
         if (inputImg[y-1][x]     < min) min = inputImg[y-1][x];
         if (inputImg[y-1][x+1]   < min) min = inputImg[y-1][x+1];
         if (inputImg[y][x-1]     < min) min = inputImg[y][x-1];
         if (inputImg[y][x]       < min) min = inputImg[y][x];
         if (inputImg[y][x+1]     < min) min = inputImg[y][x+1];
         if (inputImg[y+1][x-1]   < min) min = inputImg[y+1][x-1];
         if (inputImg[y+1][x]     < min) min = inputImg[y+1][x];
         if (inputImg[y+1][x+1]   < min) min = inputImg[y+1][x+1];
      
         resultImg[y][x] = min;   // 중간값을 결과 영상에 저장
      }

}


void CImageProDoc::Dilation(void)
{
    int x, y, max;

    for (y = 1; y < imageHeight - 1; y++)
       for (x = 1; x < imageWidth - 1; x++) {
         max = 0;
         if (inputImg[y-1][x-1] > max) max = inputImg[y-1][x-1];
         if (inputImg[y-1][x]   > max) max = inputImg[y-1][x];
         if (inputImg[y-1][x+1] > max) max = inputImg[y-1][x+1];
         if (inputImg[y][x-1]   > max) max = inputImg[y][x-1];
         if (inputImg[y][x]     > max) max = inputImg[y][x];
         if (inputImg[y][x+1]   > max) max = inputImg[y][x+1];
         if (inputImg[y+1][x-1] > max) max = inputImg[y+1][x-1];
         if (inputImg[y+1][x]   > max) max = inputImg[y+1][x];
         if (inputImg[y+1][x+1] > max) max = inputImg[y+1][x+1];
      
         resultImg[y][x] = max;   // 중간값을 결과 영상에 저장
       }

}


void CImageProDoc::Opening(void)
{
   Erosion();

   CopyResultToInput();
   Erosion();

   CopyResultToInput();
   Erosion();

   CopyResultToInput();
   Dilation(); 

   CopyResultToInput();
   Dilation(); 

   CopyResultToInput();
   Dilation(); 

}


void CImageProDoc::CopyResultToInput(void)
{
   int x, y;

   for (y = 0; y < imageHeight; y++)
      for (x = 0; x < imageWidth; x++)
         inputImg[y][x] = resultImg[y][x];

}


void CImageProDoc::Closing(void)
{
   Dilation(); 

   CopyResultToInput();
   Dilation(); 

   CopyResultToInput();
   Dilation(); 

   CopyResultToInput();
   Erosion();

   CopyResultToInput();
   Erosion();

   CopyResultToInput();
   Erosion();

}


void CImageProDoc::GeometryZoominPixelCopy(void)
{

	CMyDialogZoomIn dlg;
	int result = dlg.DoModal();
	if (result == IDOK) {
		float scale = dlg.m_zoom_in_scale;

		int i, y, x;

		gImageWidth = (int) imageWidth * scale;
		gImageHeight = (int)imageHeight * scale;

		gResultImg = (unsigned char **)malloc(gImageHeight * sizeof(unsigned char *));

		for (i = 0; i < gImageHeight; i++) {
			gResultImg[i] = (unsigned char *)malloc(gImageWidth * depth);
		}

		for (y = 0; y < gImageHeight; y++)
			for (x = 0; x < gImageWidth; x++)
				gResultImg[y][x] = inputImg[(int)(y / scale)][(int)(x / scale)];
	}
}


void CImageProDoc::GeometryZoominInterpolation(void)
{

	CMyDialogZoomIn dlg;
	int result = dlg.DoModal();
	if (result == IDOK) {
		int i, y, x;
		float src_x, src_y;      // 원시 화소 위치  
		float alpha, beta;
		float scale_x, scale_y;    // x 방향과  y 방향으로의 확대 비율
		float E, F;              // x 방향으로 보간된 결과 값 
		int Ax, Ay, Bx, By, Cx, Cy, Dx, Dy; // 보간에 사용될 4개 픽셀의 좌표

		scale_x = dlg.m_zoom_in_scale;
		scale_y = dlg.m_zoom_in_scale;

		// 결과 영상의 크기 설정
		gImageWidth = (int)imageWidth * scale_x;
		gImageHeight = (int)imageHeight * scale_y;

		// 결과 영상을 저장할 기억장소 공간 할당
		gResultImg = (unsigned char **)malloc(gImageHeight * sizeof(unsigned char *));
		for (i = 0; i < gImageHeight; i++) {
			gResultImg[i] = (unsigned char *)malloc(gImageWidth * depth);
		}

		for (y = 0; y < gImageHeight; y++)
			for (x = 0; x < gImageWidth; x++) {
				// 원시 영상에서의 픽셀 좌표 계산
				src_x = x / scale_x;
				src_y = y / scale_y;

				alpha = src_x - (int) (x / scale_x);
				beta = src_y - (int) (y / scale_y);

				// 보간에 사용된 4 픽셀의 좌표 계산
				Ax = (int) (x / scale_x);
				Ay = (int) (y / scale_y);
				Bx = Ax + 1;
				By = Ay;
				Cx = Ax;
				Cy = Ay + 1;
				Dx = Ax + 1;
				Dy = Ay + 1;

				// 픽셀 위치가 영상의 경계를 벗어나는지 검사
				if (Bx > imageWidth - 1) Bx = imageWidth - 1;
				if (Dx > imageWidth - 1) Bx = imageWidth - 1;
				if (Cy > imageHeight - 1) Cy = imageHeight - 1;
				if (Dy > imageHeight - 1) Dy = imageHeight - 1;

				// x 방향으로 보간
				E = (inputImg[Ay][Ax] * (1 - alpha) + inputImg[By][Bx] * alpha);
				F = (inputImg[Cy][Cx] * (1 - alpha) + inputImg[Dy][Dx] * alpha);

				// y 방향으로 보간
				gResultImg[y][x] = (unsigned char)(E * (1 - beta) + F * beta);

				// 픽셀 디버깅 : TRACE(" = %u\n", (unsigned char)(E * (1 - beta) + F * beta));
			}
	}
}


void CImageProDoc::GeometryZoomoutSubsampling(void)
{
	CMyDialogZoonOut dlg;
	int result = dlg.DoModal();
	if (result == IDOK) {
		float scale = dlg.m_zoom_out_scale;

		int i, y, x;
		int src_x, src_y;
		float scale_x = scale, scale_y = scale;

		// 결과 영상의 크기 계산
		gImageWidth = imageWidth * scale_x;
		gImageHeight = imageHeight * scale_y;

		// 결과 영상 저장을 위한 기억장소 할당
		gResultImg = (unsigned char **)malloc(gImageHeight * sizeof(unsigned char *));
		for (i = 0; i < gImageHeight; i++) {
			gResultImg[i] = (unsigned char *)malloc(gImageWidth * depth);
		}

		for (y = 0; y < gImageHeight; y++)
			for (x = 0; x < gImageWidth; x++) {
				src_y = (int)(y / scale_y);
				src_x = (int)(x / scale_x);
				if (src_x > imageWidth - 1) src_x = imageWidth - 1;
				if (src_y > imageHeight - 1) src_y = imageHeight - 1;
				gResultImg[y][x] = inputImg[src_y][src_x];
			}
	}
}


void CImageProDoc::GeometryZoomoutAvg(void)
{

	CMyDialogZoonOut dlg;
	int result = dlg.DoModal();
	if (result == IDOK) {

		float scale = dlg.m_zoom_out_scale;

		int i, j, x, y;
		int sum;
		int src_x, src_y;
		float scale_x = scale, scale_y = scale;

		// 결과 영상의 크기 계산
		gImageWidth = imageWidth * scale_x + 1;
		gImageHeight = imageHeight * scale_y + 1;

		// 결과 영상을 저장할 기억장소 할당
		gResultImg = (unsigned char **)malloc(gImageHeight * sizeof(unsigned char *));
		for (i = 0; i < gImageHeight; i++) {
			gResultImg[i] = (unsigned char *)malloc(gImageWidth * depth);
		}

		for (y = 0; y < imageHeight; y = y +(int) (1/scale_y))
			for (x = 0; x < imageWidth; x = x +(int) (1/scale_x)) {

				// 필터 윈도우 내의 픽셀값 합계 계산
				sum = 0;
				for (i = 0; i < (int)(1/scale_y); i++)
					for (j = 0; j < (int)(1/scale_x); j++) {
						src_x = x + j;
						src_y = y + i;

						// 영상의 경계를 벗어나는지 검사
						if (src_x > imageWidth - 1) src_x = imageWidth - 1;
						if (src_y > imageHeight - 1) src_y = imageHeight - 1;

						sum += inputImg[src_y][src_x];
					}

				// 평균값 계산
				sum = sum / ((1/scale_x) * (1/scale_y));
				if (sum > 255) sum = 255;
				if (sum < 0) sum = 0;

				// 결과 값 저장
				gResultImg[(int)(y * scale_y)][(int)(x * scale_x)] = (unsigned char)sum;
			}
	}
}

#define PI 3.14159

void CImageProDoc::GeometryRotate(void)
{
	CMyDialogAngle dlg;
	int result = dlg.DoModal();
	if (result == IDOK) {
		float myAngle = dlg.m_angle;
		unsigned char ** tmpImg; //원래의 inputImg 저장 (바뀌어진 inputImg 복귀하기 위함)
		int tmpHeight, tmpWidth;
		tmpHeight = imageHeight;
		tmpWidth = imageWidth;

		tmpImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));

		for (int i = 0; i < imageHeight; i++) {
			tmpImg[i] = (unsigned char *)malloc(imageWidth * depth);
		}

		for (int y = 0; y < imageHeight; y++)
			for (int x = 0; x < imageWidth; x++)
				tmpImg[y][x] = inputImg[y][x];

		while (myAngle > 360) myAngle -= 360;
		while (myAngle < 0) myAngle += 360;

		while (true) {
			if (myAngle > 90) {
				myAngle -= 90;
				rotateFunction(90);
				imageWidth = gImageWidth;
				imageHeight = gImageHeight;
				for (int y = 0; y < imageHeight; y++)
					for (int x = 0; x < imageWidth; x++)
						inputImg[y][x] = gResultImg[y][x];
			}
			else {
				rotateFunction(myAngle);

				for (int y = 0; y < tmpHeight; y++)
					for (int x = 0; x < tmpWidth; x++)
						inputImg[y][x] = tmpImg[y][x];
				break;
			}
		}
	}
}

void CImageProDoc::rotateFunction(float myAngle) {
	int y, x, x_source, y_source, Cx, Cy;
	float angle;
	int Oy;
	int i, xdiff, ydiff;
	Oy = imageHeight - 1;

	angle = (float)(PI / 180.0 * myAngle);   // 30도를 라디안 값의 각도로 변환

	Cx = imageWidth / 2;   // 회전 중심의 x좌표
	Cy = imageHeight / 2;   // 회전 중심의 y좌표 

							// 결과 영상 크기 계산
	gImageWidth = (int)(imageHeight*cos(PI / 2.0 - angle) + imageWidth * cos(angle));
	gImageHeight = (int)(imageHeight*cos(angle) + imageWidth * cos(PI / 2.0 - angle));
	// 결과 영상을 저장할 기억장소 할당
	gResultImg = (unsigned char **)malloc(gImageHeight * sizeof(unsigned char *));

	for (i = 0; i < gImageHeight; i++) {
		gResultImg[i] = (unsigned char *)malloc(gImageWidth * depth);
	}

	// 결과 영상의 x 좌표 범위 : -xdiff ~ gImageWidth - xdiff - 1
	// 결과 영상의 y 좌표 범위 : -ydiff ~ gImageHeight - ydiff - 1
	xdiff = (gImageWidth - imageWidth) / 2;
	ydiff = (gImageHeight - imageHeight) / 2;

	for (y = -ydiff; y < gImageHeight - ydiff; y++)
		for (x = -xdiff; x < gImageWidth - xdiff; x++)
		{
			// 변환 단계
			//     1 단계 : 원점이 영상의 좌측 하단에 오도록 y 좌표 변환
			//     2 단계 : 회전 중심이 원점에 오도록 이동
			//     3 단계 : 각도 angle 만큼 회전
			//     4 단계 : 회전 중심이 원래 위치로 돌아가도록 이동
			x_source = (int)(((Oy - y) - Cy) * sin(angle) + (x - Cx) * cos(angle) + Cx);
			y_source = (int)(((Oy - y) - Cy) * cos(angle) - (x - Cx) * sin(angle) + Cy);

			//     5 단계 : 원점이 영상의 좌측 상단에 오도록 y 좌표 변환
			y_source = Oy - y_source;

			if (x_source < 0 || x_source > imageWidth - 1 ||
				y_source < 0 || y_source > imageHeight - 1)
				gResultImg[y + ydiff][x + xdiff] = 255;
			else
				gResultImg[y + ydiff][x + xdiff] = inputImg[y_source][x_source];
		}
}

void CImageProDoc::GeometryMirror(void)
{
   int y, x;

   for (y = 0; y < imageHeight; y++)
      for (x = 0; x < imageWidth; x++)
         resultImg[y][x] = inputImg[y][imageWidth - 1 - x];

}


void CImageProDoc::GeometryFlip(void)
{
   int y, x;

   for (y = 0; y < imageHeight; y++)
      for (x = 0; x < imageWidth; x++)
         resultImg[imageHeight - 1 - y][x] = inputImg[y][x];

}


void CImageProDoc::GeometryOrigin(void)
{
	int y, x;

	for (y = 0; y < imageHeight; y++)
		for (x = 0; x < imageWidth; x++)
			resultImg[imageHeight - 1 - y][x] = inputImg[y][imageWidth - 1 - x];
}
