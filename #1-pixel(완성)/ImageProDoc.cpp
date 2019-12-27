
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
#include "CMyDialogInt.h"
#include "CMyDialogDouble.h"
#include "CMyDialogMinMax.h"
#include "CMyDialogThreshold.h"

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
	resultImg = NULL;
	
	m_int = 0;
	m_double = 0;
	m_min = 0;
	m_max = 0;
	m_threshold = 0;
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

    for(int y=0; y<imageHeight; y++)
        for(int x=0; x<imageWidth; x++) {
            value = inputImg[y][x]+100; 
            if(value > 255) resultImg[y][x] = 255;
            else resultImg[y][x]=value;
        }

}

void CImageProDoc::SubAdd(void)
{
	CMyDialogInt dlg;
	dlg.m_int = m_int;
	int result = dlg.DoModal();
	if (result == IDOK) {
		m_int = dlg.m_int;

		int value = 0;

		for (int y = 0; y<imageHeight; y++)
			for (int x = 0; x<imageWidth; x++) {
				value = inputImg[y][x] + m_int;
				if (value > 255) resultImg[y][x] = 255;
				else if (value < 0) resultImg[y][x] = 0;
				else resultImg[y][x] = value;
			}
	}
}

void CImageProDoc::SubMul(void)
{
	CMyDialogDouble dlg;
	dlg.m_double = m_double;
	int result = dlg.DoModal();
	if (result == IDOK) {
		m_double = dlg.m_double;

		double value = 0;

		for (int y = 0; y<imageHeight; y++)
			for (int x = 0; x<imageWidth; x++) {
				value = inputImg[y][x] * m_double;
				if (value > 255) resultImg[y][x] = 255;
				else if (value < 0) resultImg[y][x] = 0;
				else resultImg[y][x] = value;
			}
	}
}

void CImageProDoc::Histogram() {
	int freq[256] = { 0, };
	int max = 0;

	for (int y = 0; y<imageHeight; y++)
		for (int x = 0; x<imageWidth; x++) {
			int tmp = inputImg[y][x];
			freq[tmp]++;
			if (max < freq[tmp]) max = freq[tmp];
		}
	
	//히스토그램 256*256안에 다 들어오게끔 배율설정 - 5%씩 축소
	for(double count = 1 ; max > 255 ; count+=0.05){
		max = 0;
		for (int y = 0; y < 256; y++) {
			freq[y] /= count;
			if (max < freq[y]) max = freq[y];
		}
	}

	for (int i = 0; i < 256; ++i)
		for (int j = 255; j >= 255 - freq[i]; --j)
			histoImg[j][i] = 0;
}

void CImageProDoc::StretchingEedInSearch() {
	CMyDialogMinMax dlg;
	dlg.m_max = m_max;
	dlg.m_min = m_min;

	int result = dlg.DoModal();
	if (result == IDOK) {
		m_max = dlg.m_max;
		m_min = dlg.m_min;

		double value = 0;

		for (int y = 0; y<imageHeight; y++)
			for (int x = 0; x<imageWidth; x++) {
				if (inputImg[y][x] < m_min) resultImg[y][x] = 0;
				else if (inputImg[y][x] > m_max) resultImg[y][x] = 255;
				else resultImg[y][x] = ((inputImg[y][x] - m_min) / (m_max - m_min)) * 255;
			}
	}
}

void CImageProDoc::NegativeTransformation() {
	for (int y = 0; y<imageHeight; y++)
		for (int x = 0; x<imageWidth; x++)
			resultImg[y][x] = 255 - inputImg[y][x];
}

void CImageProDoc::Binaryimage() {
	CMyDialogThreshold dlg;
	dlg.m_threshold = m_threshold;
	int result = dlg.DoModal();
	if (result == IDOK) {
		m_threshold = dlg.m_threshold;
		for (int y = 0; y<imageHeight; y++)
			for (int x = 0; x<imageWidth; x++)
				if (inputImg[y][x] >= m_threshold) resultImg[y][x] = 255;
				else resultImg[y][x] = 0;
	}
}

void CImageProDoc::LoadImageFile(CArchive & ar)
{
	int i, maxValue;
	char type[16], buf[256];
	CFile*fp = ar.GetFile();
	CString fname = fp->GetFilePath();

	//reading the header
	if (strcmp(strrchr(fname, '.'), ".ppm") == 0 ||
		strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strrchr(fname, '.'), ".PGM") == 0 ||
		strcmp(strrchr(fname, '.'), ".pgm") == 0)
	{
		ar.ReadString(type, 15);

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d %d", &imageWidth, &imageHeight);

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d",&maxValue);

		if (strcmp(type, "P5") == 0) depth = 1;
		else depth = 3;
	}
	else if (strcmp(strchr(fname, '.'), ".raw") == 0 ||
		strcmp(strchr(fname, '.'), ".RAW") == 0)
	{
		if (fp->GetLength() != 256 * 256) {
			AfxMessageBox("256*256 크기의 파일만 사용가능합니다");
			return;
		}

		imageWidth = 256;
		imageHeight = 256;
		depth = 1;
	}

	//기억 장소 할당
	inputImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));
	resultImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));
	
	for (i = 0; i < imageHeight; i++) {
		inputImg[i] = (unsigned char *)malloc(imageWidth * depth);
		resultImg[i] = (unsigned char *)malloc(imageWidth * depth);
	}

	
	//영상 데이터 읽기
	for (i = 0; i < imageHeight; i++) {
		ar.Read(inputImg[i], imageWidth*depth);
	}
}
