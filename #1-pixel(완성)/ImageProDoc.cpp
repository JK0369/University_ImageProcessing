
// ImageProDoc.cpp : CImageProDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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


// CImageProDoc ����/�Ҹ�

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
	
	//������׷� 256*256�ȿ� �� �����Բ� �������� - 5%�� ���
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
			AfxMessageBox("256*256 ũ���� ���ϸ� ��밡���մϴ�");
			return;
		}

		imageWidth = 256;
		imageHeight = 256;
		depth = 1;
	}

	//��� ��� �Ҵ�
	inputImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));
	resultImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));
	
	for (i = 0; i < imageHeight; i++) {
		inputImg[i] = (unsigned char *)malloc(imageWidth * depth);
		resultImg[i] = (unsigned char *)malloc(imageWidth * depth);
	}

	
	//���� ������ �б�
	for (i = 0; i < imageHeight; i++) {
		ar.Read(inputImg[i], imageWidth*depth);
	}
}
