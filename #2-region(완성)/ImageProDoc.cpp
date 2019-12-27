
// ImageProDoc.cpp : CImageProDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImagePro.h"
#endif

#include "ImageProDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//엠보싱 기법인 경우, 픽셀에 +128을 해주기 위한 확인 변수
bool check = false;

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
   check = false;
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
   name = fname;

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

void CImageProDoc::EmbossingSub45(void)
{

	float ** kernel=NULL;

	// kernel 기억장소 할당
	kernel = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		kernel[i] = (float *)malloc(3 * sizeof(float));
	}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			kernel[i][j] = 0;
	kernel[0][2] = -1; kernel[2][0] = 1;

   check = true; // 엠보싱 기법일시, 픽셀값에 +128을 해주기 위한 확인 변수
   Convolve(inputImg, resultImg, imageWidth, imageHeight, kernel, 0, depth, 3);
}

void CImageProDoc::EmbossingSub90(void)
{
	float ** kernel = NULL;

	// kernel 기억장소 할당
	kernel = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		kernel[i] = (float *)malloc(sizeof(float)*3);
	}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			kernel[i][j] = 0;
	kernel[0][1] = -1; kernel[2][1] = 1;
	
	check = true;
	Convolve(inputImg, resultImg, imageWidth, imageHeight, kernel, 0, depth,3);
}

void CImageProDoc::EmbossingSub135(void)
{
	float ** kernel = NULL;

	// kernel 기억장소 할당
	kernel = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		kernel[i] = (float *)malloc(sizeof(float) * 3);
	}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			kernel[i][j] = 0;
	kernel[0][0] = -1; kernel[2][2] = 1;
	check = true;
	Convolve(inputImg, resultImg, imageWidth, imageHeight, kernel, 0, depth,3);
}

void CImageProDoc::AverageSub3x3(void)
{
	float ** mask = NULL;

	// mask 기억장소 할당
	mask = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		mask[i] = (float *)malloc(sizeof(float) * 3);
	}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			mask[i][j] = 1 / 9.0f;

	Convolve(inputImg, resultImg, imageWidth, imageHeight, mask, 0, depth,3);
}

void CImageProDoc::AverageSub5x5(void)
{
	float ** mask = NULL;

	// mask 기억장소 할당
	mask = (float **)malloc(5 * sizeof(float *));

	for (int i = 0; i < 5; i++) {
		mask[i] = (float *)malloc(sizeof(float) * 5);
	}

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			mask[i][j] = 1 / 25.0f;

	Convolve(inputImg, resultImg, imageWidth, imageHeight, mask, 0, depth, 5);
}

void CImageProDoc::SharpeningSubMask1(void)
{
	float ** mask = NULL;

	// mask 기억장소 할당
	mask = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		mask[i] = (float *)malloc(sizeof(float) * 3);
	}
	
	mask[0][0] = 0; mask[0][1] = -1; mask[0][2] = 0;
	mask[1][0] = -1; mask[1][1] = 5; mask[1][2] = -1;
	mask[2][0] = 0; mask[2][1] = -1; mask[2][2] = 0;

	Convolve(inputImg, resultImg, imageWidth, imageHeight, mask, 0, depth, 3);
}

void CImageProDoc::SharpeningSubMask2(void)
{
	float ** mask = NULL;

	// mask 기억장소 할당
	mask = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		mask[i] = (float *)malloc(sizeof(float) * 3);
	}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			mask[i][j] = -1;
	mask[1][1] = 9;

	Convolve(inputImg, resultImg, imageWidth, imageHeight, mask, 0, depth, 3);
}

void CImageProDoc::SharpeningSubMask3(void)
{
	float ** mask = NULL;

	// mask 기억장소 할당
	mask = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		mask[i] = (float *)malloc(sizeof(float) * 3);
	}

	mask[0][0] = 1; mask[0][1] = -2; mask[0][2] = 1;
	mask[1][0] = -2; mask[1][1] = 5; mask[1][2] = -2;
	mask[2][0] = 1; mask[2][1] = -2; mask[2][2] = 1;

	Convolve(inputImg, resultImg, imageWidth, imageHeight, mask, 0, depth, 3);
}

#include <math.h>

void CImageProDoc::EdgeSubSobel(void)
{
	float ** mask1 = NULL;
	float ** mask2 = NULL;

	mask1 = (float **)malloc(3 * sizeof(float *));
	mask2 = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		mask1[i] = (float *)malloc(sizeof(float) * 3);
		mask2[i] = (float *)malloc(sizeof(float) * 3);
	}
	
	mask1[0][0] = 1; mask1[0][1] = 0; mask1[0][2] = -1;
	mask1[1][0] = 2; mask1[1][1] = 0; mask1[1][2] = -2;
	mask1[2][0] = 1; mask1[2][1] = 0; mask1[2][2] = -1;

	mask2[0][0] = -1; mask2[0][1] = -2; mask2[0][2] = -1;
	mask2[1][0] = 0; mask2[1][1] = 0; mask2[1][2] = 0;
	mask2[2][0] = 1; mask2[2][1] = 2; mask2[2][2] = 1;

	unsigned char **Er, **Ec;

	// 기억장소 할당
	Er = (unsigned char **)malloc((imageHeight) * sizeof(unsigned char *));
	Ec = (unsigned char **)malloc((imageHeight) * sizeof(unsigned char *));

	for (int i = 0; i < imageHeight; i++) {
		Er[i] = (unsigned char *)malloc(imageWidth * depth);
		Ec[i] = (unsigned char *)malloc(imageWidth * depth);
	}

	Convolve(inputImg, Er, imageWidth, imageHeight, mask1, 0, depth,3);
	Convolve(inputImg, Ec, imageWidth, imageHeight, mask2, 0, depth,3);

	for (int y = 0; y < imageHeight; y++)
		for (int x = 0; x < imageWidth * depth; x++) {
			int sum = (int)sqrt((float)(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]));
			if (sum > 255) sum = 255;
			if (sum < 0) sum = 0;
			resultImg[y][x] = (unsigned char)sum;
		}
}

void CImageProDoc::EdgeSubPrewitt(void)
{
	float ** mask1 = NULL;
	float ** mask2 = NULL;

	mask1 = (float **)malloc(3 * sizeof(float *));
	mask2 = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		mask1[i] = (float *)malloc(sizeof(float) * 3);
		mask2[i] = (float *)malloc(sizeof(float) * 3);
	}

	mask1[0][0] = -1; mask1[0][1] = -1; mask1[0][2] = -1;
	mask1[1][0] = 0; mask1[1][1] = 0; mask1[1][2] = 0;
	mask1[2][0] = 1; mask1[2][1] = 1; mask1[2][2] = 1;

	mask2[0][0] = 1; mask2[0][1] = 0; mask2[0][2] = -1;
	mask2[1][0] = 1; mask2[1][1] = 0; mask2[1][2] = -1;
	mask2[2][0] = 1; mask2[2][1] = 0; mask2[2][2] = -1;

	unsigned char **Er, **Ec;

	// 기억장소 할당
	Er = (unsigned char **)malloc((imageHeight) * sizeof(unsigned char *));
	Ec = (unsigned char **)malloc((imageHeight) * sizeof(unsigned char *));

	for (int i = 0; i < imageHeight; i++) {
		Er[i] = (unsigned char *)malloc(imageWidth * depth);
		Ec[i] = (unsigned char *)malloc(imageWidth * depth);
	}

	Convolve(inputImg, Er, imageWidth, imageHeight, mask1, 0, depth, 3);
	Convolve(inputImg, Ec, imageWidth, imageHeight, mask2, 0, depth, 3);

	for (int y = 0; y < imageHeight; y++)
		for (int x = 0; x < imageWidth * depth; x++) {
			int sum = (int)sqrt((float)(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]));
			if (sum > 255) sum = 255;
			if (sum < 0) sum = 0;
			resultImg[y][x] = (unsigned char)sum;
		}
}

void CImageProDoc::EdgeSubRoberts(void)
{
	float ** mask1 = NULL;
	float ** mask2 = NULL;

	mask1 = (float **)malloc(3 * sizeof(float *));
	mask2 = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		mask1[i] = (float *)malloc(sizeof(float) * 3);
		mask2[i] = (float *)malloc(sizeof(float) * 3);
	}

	mask1[0][0] = -1; mask1[0][1] = 0; mask1[0][2] = 0;
	mask1[1][0] = 0; mask1[1][1] = 1; mask1[1][2] = 0;
	mask1[2][0] = 0; mask1[2][1] = 0; mask1[2][2] = 0;

	mask2[0][0] = 0; mask2[0][1] = 0; mask2[0][2] = -1;
	mask2[1][0] = 0; mask2[1][1] = 1; mask2[1][2] = 0;
	mask2[2][0] = 0; mask2[2][1] = 0; mask2[2][2] = 0;

	unsigned char **Er, **Ec;

	// 기억장소 할당
	Er = (unsigned char **)malloc((imageHeight) * sizeof(unsigned char *));
	Ec = (unsigned char **)malloc((imageHeight) * sizeof(unsigned char *));

	for (int i = 0; i < imageHeight; i++) {
		Er[i] = (unsigned char *)malloc(imageWidth * depth);
		Ec[i] = (unsigned char *)malloc(imageWidth * depth);
	}

	Convolve(inputImg, Er, imageWidth, imageHeight, mask1, 0, depth, 3);
	Convolve(inputImg, Ec, imageWidth, imageHeight, mask2, 0, depth, 3);

	for (int y = 0; y < imageHeight; y++)
		for (int x = 0; x < imageWidth * depth; x++) {
			int sum = (int)sqrt((float)(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]));
			if (sum > 255) sum = 255;
			if (sum < 0) sum = 0;
			resultImg[y][x] = (unsigned char)sum;
		}
}

void CImageProDoc::Median()
{
	int i, j, temp, x, y;
	int n[9];

	for (y = 1; y < imageHeight - 1; y++)
		for (x = 1; x < imageWidth - 1; x++)
		{
			n[0] = inputImg[y - 1][x - 1];
			n[1] = inputImg[y - 1][x];
			n[2] = inputImg[y - 1][x + 1];
			n[3] = inputImg[y][x - 1];
			n[4] = inputImg[y][x];
			n[5] = inputImg[y][x + 1];
			n[6] = inputImg[y + 1][x - 1];
			n[7] = inputImg[y + 1][x];
			n[8] = inputImg[y + 1][x + 1];

			// 버블 정렬(bubble sorting)   
			for (i = 8; i > 0; i--)
				for (j = 0; j < i; j++)
					if (n[j] > n[j + 1]) {
						temp = n[j + 1];
						n[j + 1] = n[j];
						n[j] = temp;
					}

			resultImg[y][x] = n[4];   // 중간값을 결과 영상에 저장
		}
}

void CImageProDoc::MySaveCustomState()
{

	char szFilter[] = "PGM FILE (*.PGM)| *.pgm;||";
	CFileDialog dlg(FALSE, "pgm", name, OFN_HIDEREADONLY, szFilter);
	
	if (IDOK == dlg.DoModal())
	{
		FILE* fp;
		
		fp = fopen(dlg.GetPathName(), "wb");
		
		fprintf(fp, "%c%c\n", 'P', '5');
		fprintf(fp, "%d %d\n", imageWidth, imageHeight);
		fprintf(fp, "%d\n", 255);

		for (int i = 0; i < imageHeight; i++) {
			for (int j = 0; j < imageWidth; j++) {
				fprintf(fp, "%c", resultImg[i][j]);
			}
		}
		fclose(fp);
	}
}

//weight 변수 : filter의 크기
void CImageProDoc::Convolve(unsigned char ** inputImg, unsigned char ** resultImg, int cols, int rows, float ** mask, int bias, int depth, int weight)
{
   int i, j, x, y;
   int red, green, blue;
   int sum;
   unsigned char **tmpImg;

   // 기억장소 할당
   tmpImg = (unsigned char **) malloc((imageHeight + 2)* sizeof(unsigned char *));

   for (i = 0; i <= imageHeight + 2; i++)
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

		  //엠보싱 기법일 경우, +128
		  int embossingPlus = 0;
		  if (check) embossingPlus = 128;

   for (y=0; y < imageHeight; y++)
        for (x=0; x < imageWidth; x++) {
            if (depth == 1) {
                sum=0;
                for (i=0; i<weight; i++)
					for (j = 0; j < weight; j++) {
						if (y + i > imageHeight + 2 || x + j > imageWidth + 2) continue;
						sum += (int)(tmpImg[y + i][x + j] * mask[i][j]);
					}
                if (sum > 255) sum=255;
                if (sum < 0) sum = 0;
				sum += embossingPlus;
				if (sum > 255) sum = 255;
                resultImg[y][x] = (unsigned char) sum;
            }
            else if (depth == 3) {
                red = 0;
                green = 0;
                blue = 0;

                for (i=0; i<weight; i++)
                    for (j=0; j<weight; j++) {
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

  
   // 엠보싱 기법 종료 후, 엠보싱 기법인지 확인하는 check 변수 다시 초기화
   check = false;
}
