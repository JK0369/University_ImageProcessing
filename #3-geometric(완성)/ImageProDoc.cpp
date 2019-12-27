
// ImageProDoc.cpp : CImageProDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.

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


// CImageProDoc ����/�Ҹ�

CImageProDoc::CImageProDoc()
{
   inputImg = NULL;
   inputImg2 = NULL;
   resultImg = NULL;
   gResultImg = NULL;     // �߰��Ǵ� �ڵ� 
}

CImageProDoc::~CImageProDoc()
{
}

BOOL CImageProDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

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

// ����� �׸��� �����մϴ�.
void CImageProDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
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

// �˻� ó���⸦ �����մϴ�.
void CImageProDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CImageProDoc ����

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


// CImageProDoc ���


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
    int acc_hist = 0;      // ������׷��� ���� �����ϴ� ����
    float N = (float) imageHeight * imageWidth;   // ������ ��ü �ȼ� ��
    int hist[256], sum[256];
   
    for(k=0; k<256; k++) hist[k]=0;
   
    // ��ϰ��� �󵵼� ����
    for(y = 0; y < imageHeight; y++)
        for(x = 0; x < imageWidth; x++) {
           k = inputImg[y][x];
           hist[k] = hist[k]+1;
        }

    // ������ ������׷� �� ���
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

   // ������ ��� �б� 
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
         AfxMessageBox("256x256 ũ���� ���ϸ� ��밡���մϴ�."); 
         return;
      }

      imageWidth = 256;
      imageHeight = 256;
      depth = 1;
   }

   // ������ �Ҵ�
   inputImg = (unsigned char **) malloc(imageHeight * sizeof(unsigned char *));
   resultImg = (unsigned char **) malloc(imageHeight * sizeof(unsigned char *));

   for (i = 0; i < imageHeight; i++) {
        inputImg[i] = (unsigned char *) malloc(imageWidth * depth);
        resultImg[i] = (unsigned char *) malloc(imageWidth * depth);
   }

   // ���� ������ �б� 
   for (i = 0; i < imageHeight; i++)
      ar.Read(inputImg[i], imageWidth*depth);

}


void CImageProDoc::LoadSecondImageFile(CArchive& ar)
{
   int i, maxValue;
   char type[16], buf[256];

   CFile *fp = ar.GetFile();
   CString fname = fp->GetFilePath();  

   // ������ ��� �б� 
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
         AfxMessageBox("256x256 ũ���� ���ϸ� ��밡���մϴ�."); 
         return;
      }

      imageWidth = 256;
      imageHeight = 256;
      depth = 1;
   }

   // ������ �Ҵ�
   inputImg2 = (unsigned char **) malloc(imageHeight * sizeof(unsigned char *));

   for (i = 0; i < imageHeight; i++) {
        inputImg2[i] = (unsigned char *) malloc(imageWidth * depth);
   }

   // ���� ������ �б� 
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

   // ������ �Ҵ�
   tmpImg = (unsigned char **) malloc((imageHeight + 2)* sizeof(unsigned char *));

   for (i = 0; i < imageHeight + 2; i++)
        tmpImg[i] = (unsigned char *) malloc((imageWidth + 2) * depth);

   // 0-������ ���� 0���� �ʱ�ȭ
   for (y = 0; y < imageHeight + 2; y++)
       for (x = 0; x < (imageWidth + 2) * depth; x++)
           tmpImg[y][x] = 0;

   // ���� ���� 
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

   // ������ ��ȯ 
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

   // ������ �Ҵ�
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
      
           // ���� ����(bubble sorting)   
           for (i = 8; i > 0; i--)
               for (j = 0; j < i; j++)
                   if (n[j] > n[j+1]) {
                       temp = n[j+1];
                       n[j+1] = n[j];
                       n[j] = temp;
                   }
      
            resultImg[y][x] = n[4];   // �߰����� ��� ���� ����
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
      
         resultImg[y][x] = min;   // �߰����� ��� ���� ����
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
      
         resultImg[y][x] = max;   // �߰����� ��� ���� ����
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
		float src_x, src_y;      // ���� ȭ�� ��ġ  
		float alpha, beta;
		float scale_x, scale_y;    // x �����  y ���������� Ȯ�� ����
		float E, F;              // x �������� ������ ��� �� 
		int Ax, Ay, Bx, By, Cx, Cy, Dx, Dy; // ������ ���� 4�� �ȼ��� ��ǥ

		scale_x = dlg.m_zoom_in_scale;
		scale_y = dlg.m_zoom_in_scale;

		// ��� ������ ũ�� ����
		gImageWidth = (int)imageWidth * scale_x;
		gImageHeight = (int)imageHeight * scale_y;

		// ��� ������ ������ ������ ���� �Ҵ�
		gResultImg = (unsigned char **)malloc(gImageHeight * sizeof(unsigned char *));
		for (i = 0; i < gImageHeight; i++) {
			gResultImg[i] = (unsigned char *)malloc(gImageWidth * depth);
		}

		for (y = 0; y < gImageHeight; y++)
			for (x = 0; x < gImageWidth; x++) {
				// ���� ���󿡼��� �ȼ� ��ǥ ���
				src_x = x / scale_x;
				src_y = y / scale_y;

				alpha = src_x - (int) (x / scale_x);
				beta = src_y - (int) (y / scale_y);

				// ������ ���� 4 �ȼ��� ��ǥ ���
				Ax = (int) (x / scale_x);
				Ay = (int) (y / scale_y);
				Bx = Ax + 1;
				By = Ay;
				Cx = Ax;
				Cy = Ay + 1;
				Dx = Ax + 1;
				Dy = Ay + 1;

				// �ȼ� ��ġ�� ������ ��踦 ������� �˻�
				if (Bx > imageWidth - 1) Bx = imageWidth - 1;
				if (Dx > imageWidth - 1) Bx = imageWidth - 1;
				if (Cy > imageHeight - 1) Cy = imageHeight - 1;
				if (Dy > imageHeight - 1) Dy = imageHeight - 1;

				// x �������� ����
				E = (inputImg[Ay][Ax] * (1 - alpha) + inputImg[By][Bx] * alpha);
				F = (inputImg[Cy][Cx] * (1 - alpha) + inputImg[Dy][Dx] * alpha);

				// y �������� ����
				gResultImg[y][x] = (unsigned char)(E * (1 - beta) + F * beta);

				// �ȼ� ����� : TRACE(" = %u\n", (unsigned char)(E * (1 - beta) + F * beta));
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

		// ��� ������ ũ�� ���
		gImageWidth = imageWidth * scale_x;
		gImageHeight = imageHeight * scale_y;

		// ��� ���� ������ ���� ������ �Ҵ�
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

		// ��� ������ ũ�� ���
		gImageWidth = imageWidth * scale_x + 1;
		gImageHeight = imageHeight * scale_y + 1;

		// ��� ������ ������ ������ �Ҵ�
		gResultImg = (unsigned char **)malloc(gImageHeight * sizeof(unsigned char *));
		for (i = 0; i < gImageHeight; i++) {
			gResultImg[i] = (unsigned char *)malloc(gImageWidth * depth);
		}

		for (y = 0; y < imageHeight; y = y +(int) (1/scale_y))
			for (x = 0; x < imageWidth; x = x +(int) (1/scale_x)) {

				// ���� ������ ���� �ȼ��� �հ� ���
				sum = 0;
				for (i = 0; i < (int)(1/scale_y); i++)
					for (j = 0; j < (int)(1/scale_x); j++) {
						src_x = x + j;
						src_y = y + i;

						// ������ ��踦 ������� �˻�
						if (src_x > imageWidth - 1) src_x = imageWidth - 1;
						if (src_y > imageHeight - 1) src_y = imageHeight - 1;

						sum += inputImg[src_y][src_x];
					}

				// ��հ� ���
				sum = sum / ((1/scale_x) * (1/scale_y));
				if (sum > 255) sum = 255;
				if (sum < 0) sum = 0;

				// ��� �� ����
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
		unsigned char ** tmpImg; //������ inputImg ���� (�ٲ���� inputImg �����ϱ� ����)
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

	angle = (float)(PI / 180.0 * myAngle);   // 30���� ���� ���� ������ ��ȯ

	Cx = imageWidth / 2;   // ȸ�� �߽��� x��ǥ
	Cy = imageHeight / 2;   // ȸ�� �߽��� y��ǥ 

							// ��� ���� ũ�� ���
	gImageWidth = (int)(imageHeight*cos(PI / 2.0 - angle) + imageWidth * cos(angle));
	gImageHeight = (int)(imageHeight*cos(angle) + imageWidth * cos(PI / 2.0 - angle));
	// ��� ������ ������ ������ �Ҵ�
	gResultImg = (unsigned char **)malloc(gImageHeight * sizeof(unsigned char *));

	for (i = 0; i < gImageHeight; i++) {
		gResultImg[i] = (unsigned char *)malloc(gImageWidth * depth);
	}

	// ��� ������ x ��ǥ ���� : -xdiff ~ gImageWidth - xdiff - 1
	// ��� ������ y ��ǥ ���� : -ydiff ~ gImageHeight - ydiff - 1
	xdiff = (gImageWidth - imageWidth) / 2;
	ydiff = (gImageHeight - imageHeight) / 2;

	for (y = -ydiff; y < gImageHeight - ydiff; y++)
		for (x = -xdiff; x < gImageWidth - xdiff; x++)
		{
			// ��ȯ �ܰ�
			//     1 �ܰ� : ������ ������ ���� �ϴܿ� ������ y ��ǥ ��ȯ
			//     2 �ܰ� : ȸ�� �߽��� ������ ������ �̵�
			//     3 �ܰ� : ���� angle ��ŭ ȸ��
			//     4 �ܰ� : ȸ�� �߽��� ���� ��ġ�� ���ư����� �̵�
			x_source = (int)(((Oy - y) - Cy) * sin(angle) + (x - Cx) * cos(angle) + Cx);
			y_source = (int)(((Oy - y) - Cy) * cos(angle) - (x - Cx) * sin(angle) + Cy);

			//     5 �ܰ� : ������ ������ ���� ��ܿ� ������ y ��ǥ ��ȯ
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
