
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
#include "CMyDialogZOOM.h"
#include "CMyDialogAngle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//������ ����� ���, �ȼ��� +128�� ���ֱ� ���� Ȯ�� ����
bool check = false;

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
	check = false;

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

void CImageProDoc::LoadTwoImages(void)
{

	CFile file;
	CFileDialog dlg(TRUE);

	AfxMessageBox("Select the First Image");

	if (dlg.DoModal() == IDOK) {
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		LoadImageFile(ar);
		file.Close();
	}

	AfxMessageBox("Select the Second Image");

	if (dlg.DoModal() == IDOK) {
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

	// ������ ��� �б� 
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
		sscanf_s(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0) depth = 1;
		else depth = 3;
	}
	else if (strcmp(strchr(fname, '.'), ".raw") == 0 ||
		strcmp(strchr(fname, '.'), ".RAW") == 0)
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
	inputImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));
	resultImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));

	for (i = 0; i < imageHeight; i++) {
		inputImg[i] = (unsigned char *)malloc(imageWidth * depth);
		resultImg[i] = (unsigned char *)malloc(imageWidth * depth);
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
		sscanf_s(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0) depth = 1;
		else depth = 3;

		// sprintf(buf, "%s %d %d %d", type, imageWidth, imageHeight, maxValue);
	}
	else if (strcmp(strchr(fname, '.'), ".raw") == 0 ||
		strcmp(strchr(fname, '.'), ".RAW") == 0) {

		if (fp->GetLength() != 256 * 256) {
			AfxMessageBox("256x256 ũ���� ���ϸ� ��밡���մϴ�.");
			return;
		}

		imageWidth = 256;
		imageHeight = 256;
		depth = 1;
	}

	// ������ �Ҵ�
	inputImg2 = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));

	for (i = 0; i < imageHeight; i++) {
		inputImg2[i] = (unsigned char *)malloc(imageWidth * depth);
	}

	// ���� ������ �б� 
	for (i = 0; i < imageHeight; i++)
		ar.Read(inputImg2[i], imageWidth*depth);

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
	int freq2[256] = { 0, };
	int max = 0;
	bool check = false;

	for (int y = 0; y<imageHeight; y++)
		for (int x = 0; x<imageWidth; x++) {
			int tmp = inputImg[y][x];
			freq[tmp]++;
			freq2[tmp]++;
			if (max < freq[tmp]) max = freq[tmp];
		}
	
	int max2 = max;

	//������׷� 256*256�ȿ� �� �����Բ� �������� - 5%�� ���
	for(double count = 1 ; max > 255 ; count+=0.05){
		max = 0;
		for (int y = 0; y < 256; y++) {
			freq[y] /= count;
			if (max < freq[y]) max = freq[y];
		}
	}

	//������׷��� ���� ���, Ȯ��
	for (double count = 1; max2 < 200; count += 0.05) {
		max2 = 0;
		for (int y = 0; y < 256; y++) {
			freq2[y] *= count;
			if (max2 < freq2[y]) max2 = freq2[y];
		}
		check = true;
	}

	if (check) {
		for (int i = 0; i < 256; ++i)
			for (int j = 255; j >= 255 - freq2[i]; --j)
				histoImg[j][i] = 0;
	}
	else {
		for (int i = 0; i < 256; ++i)
			for (int j = 255; j >= 255 - freq[i]; --j)
				histoImg[j][i] = 0;
	}
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

//////////////////////////////////#2


void CImageProDoc::EmbossingSub45(void)
{

	float ** kernel = NULL;

	// kernel ������ �Ҵ�
	kernel = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		kernel[i] = (float *)malloc(3 * sizeof(float));
	}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			kernel[i][j] = 0;
	kernel[0][2] = -1; kernel[2][0] = 1;

	check = true; // ������ ����Ͻ�, �ȼ����� +128�� ���ֱ� ���� Ȯ�� ����
	Convolve(inputImg, resultImg, imageWidth, imageHeight, kernel, 0, depth, 3);
}

void CImageProDoc::EmbossingSub90(void)
{
	float ** kernel = NULL;

	// kernel ������ �Ҵ�
	kernel = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		kernel[i] = (float *)malloc(sizeof(float) * 3);
	}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			kernel[i][j] = 0;
	kernel[0][1] = -1; kernel[2][1] = 1;

	check = true;
	Convolve(inputImg, resultImg, imageWidth, imageHeight, kernel, 0, depth, 3);
}

void CImageProDoc::EmbossingSub135(void)
{
	float ** kernel = NULL;

	// kernel ������ �Ҵ�
	kernel = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		kernel[i] = (float *)malloc(sizeof(float) * 3);
	}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			kernel[i][j] = 0;
	kernel[0][0] = -1; kernel[2][2] = 1;
	check = true;
	Convolve(inputImg, resultImg, imageWidth, imageHeight, kernel, 0, depth, 3);
}

void CImageProDoc::AverageSub3x3(void)
{
	float ** mask = NULL;

	// mask ������ �Ҵ�
	mask = (float **)malloc(3 * sizeof(float *));

	for (int i = 0; i < 3; i++) {
		mask[i] = (float *)malloc(sizeof(float) * 3);
	}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			mask[i][j] = 1 / 9.0f;

	Convolve(inputImg, resultImg, imageWidth, imageHeight, mask, 0, depth, 3);
}

void CImageProDoc::AverageSub5x5(void)
{
	float ** mask = NULL;

	// mask ������ �Ҵ�
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

	// mask ������ �Ҵ�
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

	// mask ������ �Ҵ�
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

	// mask ������ �Ҵ�
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

	// ������ �Ҵ�
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

	// ������ �Ҵ�
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

	// ������ �Ҵ�
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

			// ���� ����(bubble sorting)   
			for (i = 8; i > 0; i--)
				for (j = 0; j < i; j++)
					if (n[j] > n[j + 1]) {
						temp = n[j + 1];
						n[j + 1] = n[j];
						n[j] = temp;
					}

			resultImg[y][x] = n[4];   // �߰����� ��� ���� ����
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

//weight ���� : filter�� ũ��
void CImageProDoc::Convolve(unsigned char ** inputImg, unsigned char ** resultImg, int cols, int rows, float ** mask, int bias, int depth, int weight)
{
	int i, j, x, y;
	int red, green, blue;
	int sum;
	unsigned char **tmpImg;

	// ������ �Ҵ�
	tmpImg = (unsigned char **)malloc((imageHeight + 2) * sizeof(unsigned char *));

	for (i = 0; i <= imageHeight + 2; i++)
		tmpImg[i] = (unsigned char *)malloc((imageWidth + 2) * depth);

	// 0-������ ���� 0���� �ʱ�ȭ
	for (y = 0; y < imageHeight + 2; y++)
		for (x = 0; x < (imageWidth + 2) * depth; x++)
			tmpImg[y][x] = 0;

	// ���� ���� 
	for (y = 1; y < imageHeight + 1; y++)
		for (x = 1; x < imageWidth + 1; x++)
			if (depth == 1) tmpImg[y][x] = inputImg[y - 1][x - 1];
			else if (depth == 3) {
				tmpImg[y][3 * x] = inputImg[y - 1][3 * (x - 1)];
				tmpImg[y][3 * x + 1] = inputImg[y - 1][3 * (x - 1) + 1];
				tmpImg[y][3 * x + 2] = inputImg[y - 1][3 * (x - 1) + 2];
			}

			//������ ����� ���, +128
			int embossingPlus = 0;
			if (check) embossingPlus = 128;

			for (y = 0; y < imageHeight; y++)
				for (x = 0; x < imageWidth; x++) {
					if (depth == 1) {
						sum = 0;
						for (i = 0; i<weight; i++)
							for (j = 0; j < weight; j++) {
								if (y + i > imageHeight + 2 || x + j > imageWidth + 2) continue;
								sum += (int)(tmpImg[y + i][x + j] * mask[i][j]);
							}
						if (sum > 255) sum = 255;
						if (sum < 0) sum = 0;
						sum += embossingPlus;
						if (sum > 255) sum = 255;
						resultImg[y][x] = (unsigned char)sum;
					}
					else if (depth == 3) {
						red = 0;
						green = 0;
						blue = 0;

						for (i = 0; i<weight; i++)
							for (j = 0; j<weight; j++) {
								red += (int)(tmpImg[y + i][3 * (x + j)] * mask[i][j]);
								green += (int)(tmpImg[y + i][3 * (x + j) + 1] * mask[i][j]);
								blue += (int)(tmpImg[y + i][3 * (x + j) + 2] * mask[i][j]);
							}

						if (red > 255) red = 255;
						if (red < 0) red = 0;
						if (green > 255) green = 255;
						if (green < 0) green = 0;
						if (blue > 255) blue = 255;
						if (blue < 0) blue = 0;

						resultImg[y][3 * x] = (unsigned char)red;
						resultImg[y][3 * x + 1] = (unsigned char)green;
						resultImg[y][3 * x + 2] = (unsigned char)blue;
					}
				}


			// ������ ��� ���� ��, ������ ������� Ȯ���ϴ� check ���� �ٽ� �ʱ�ȭ
			check = false;
}

/////////#3
void CImageProDoc::GeometryZoom()
{
	CMyDialogZOOM dlg;

	int result = dlg.DoModal();
	if (result == IDOK) {
		float scale = dlg.m_zoom_size;
		if (scale < 1) GeometryZoominInterpolation(scale);
		else GeometryZoomoutSubsampling(scale);
	}
}

void CImageProDoc::GeometryZoominInterpolation(float scale)
{

	int i, y, x;
	float src_x, src_y;      // ���� ȭ�� ��ġ  
	float alpha, beta;
	float scale_x, scale_y;    // x �����  y ���������� Ȯ�� ����
	float E, F;              // x �������� ������ ��� �� 
	int Ax, Ay, Bx, By, Cx, Cy, Dx, Dy; // ������ ���� 4�� �ȼ��� ��ǥ

	scale_x = scale;
	scale_y = scale;

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

			alpha = src_x - (int)(x / scale_x);
			beta = src_y - (int)(y / scale_y);

			// ������ ���� 4 �ȼ��� ��ǥ ���
			Ax = (int)(x / scale_x);
			Ay = (int)(y / scale_y);
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

void CImageProDoc::GeometryZoomoutSubsampling(float scale)
{

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

		//���� ũ�� ����
		while (myAngle > 360) myAngle -= 360;
		while (myAngle < 0) myAngle += 360;

		// 90���� ������ -90�� ��, 90�� ȸ�� �Ѵ�. �� ����� inputImg�� �����Ѵ�.
		// 90������ ������ ���� �� ������ �ݺ��Ѵ�.
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


void CImageProDoc::GeometryOrigin(void)
{
	int y, x;

	for (y = 0; y < imageHeight; y++)
		for (x = 0; x < imageWidth; x++)
			resultImg[imageHeight - 1 - y][x] = inputImg[y][imageWidth - 1 - x];
}

/////////// #4


#define NUM_FRAMES 6
#define SIZE_LINE 10
void CImageProDoc::GeometryMorphing(POINT sourceSt[], POINT sourceEnd[], POINT destSt[], POINT destEnd[])
{
	control_line source_lines[SIZE_LINE];
	control_line dest_lines[SIZE_LINE];

	for (int i = 0; i < SIZE_LINE; i++) {
		source_lines[i].Px = sourceSt[i].x;
		source_lines[i].Py = sourceSt[i].y;
		source_lines[i].Qx = sourceEnd[i].x;
		source_lines[i].Qy = sourceEnd[i].y;

		dest_lines[i].Px = destSt[i].x;
		dest_lines[i].Py = destSt[i].y;
		dest_lines[i].Qx = destEnd[i].x;
		dest_lines[i].Qy = destEnd[i].y;
	}

	double u;       // ���� �������� ��ġ   
	double h;       // ������κ��� �ȼ��� ���� ���� 
	double d;       // ����� �ȼ� ������ �Ÿ� 
	double tx, ty;   // ������� �ȼ��� �����Ǵ� �Է� ���� �ȼ� ������ ������ ��  
	double xp, yp;  // �� ����� ���� ���� �Է� ������ �����Ǵ� �ȼ� ��ġ     
	double weight;     // �� ����� ����ġ       
	double totalWeight; // ����ġ�� ��          
	double a = 0.001;
	double b = 2.0;
	double p = 0.75;

	unsigned char **warpedImg;
	unsigned char **warpedImg2;
	int frame;
	double fweight;
	control_line warp_lines[SIZE_LINE];
	double tx2, ty2;
	double xp2, yp2;
	int dest_x1, dest_y1, dest_x2, dest_y2;
	int source_x2, source_y2;

	int x1, x2, y1, y2;
	int src_x1, src_y1, src_x2, src_y2;
	double src_line_length, dest_line_length;

	int i, j;
	int num_lines = SIZE_LINE;         // ����� �� 
	int line;
	int x, y;
	int source_x, source_y;
	int last_row, last_col;

	// �߰� �������� ���� ����� ������ ���� ������ �Ҵ� 
	warpedImg = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));
	for (i = 0; i < imageHeight; i++) {
		warpedImg[i] = (unsigned char *)malloc(imageWidth * depth);
	}

	warpedImg2 = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));
	for (i = 0; i < imageHeight; i++) {
		warpedImg2[i] = (unsigned char *)malloc(imageWidth * depth);
	}

	for (i = 0; i < NUM_FRAMES; i++) {
		morphedImg[i] = (unsigned char **)malloc(imageHeight * sizeof(unsigned char *));
		for (j = 0; j < imageHeight; j++) {
			morphedImg[i][j] = (unsigned char *)malloc(imageWidth * depth);
		}
	}

	last_row = imageHeight - 1;
	last_col = imageWidth - 1;

	// �� �߰� �����ӿ� ���Ͽ� 
	for (frame = 1; frame <= NUM_FRAMES; frame++)
	{
		// �߰� �����ӿ� ���� ����ġ ���
		fweight = (double)(frame) / (NUM_FRAMES + 1);

		// ���� ����ġ Ȯ��
		// TRACE("\nweight=%lf\n", fweight);

		// �߰� �����ӿ� ���� ��� ��� 
		for (line = 0; line<num_lines; line++)
		{
			warp_lines[line].Px = (int)(source_lines[line].Px +
				(dest_lines[line].Px - source_lines[line].Px)*fweight);
			warp_lines[line].Py = (int)(source_lines[line].Py +
				(dest_lines[line].Py - source_lines[line].Py)*fweight);
			warp_lines[line].Qx = (int)(source_lines[line].Qx +
				(dest_lines[line].Qx - source_lines[line].Qx)*fweight);
			warp_lines[line].Qy = (int)(source_lines[line].Qy +
				(dest_lines[line].Qy - source_lines[line].Qy)*fweight);
		}

		// ��� ������ �� �ȼ��� ���Ͽ� 
		for (y = 0; y<imageHeight; y++)
		{
			for (x = 0; x<imageWidth; x++)
			{
				totalWeight = 0.0;
				tx = 0.0;
				ty = 0.0;

				tx2 = 0.0;
				ty2 = 0.0;

				// �� ����� ���Ͽ� 
				for (line = 0; line < num_lines; line++)
				{
					x1 = warp_lines[line].Px;
					y1 = warp_lines[line].Py;
					x2 = warp_lines[line].Qx;
					y2 = warp_lines[line].Qy;

					dest_line_length = sqrt((double)((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)));

					// ������������ ��ġ �� �ȼ��� ���� ���� ��� 
					u = (double)((x - x1)*(x2 - x1) + (y - y1)*(y2 - y1)) /
						(double)((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
					h = (double)((y - y1)*(x2 - x1) - (x - x1)*(y2 - y1)) / dest_line_length;

					// ����� �ȼ� ������ �Ÿ� ��� 
					if (u < 0) d = sqrt((double)((x - x1) * (x - x1) + (y - y1) * (y - y1)));
					else if (u > 1) d = sqrt((double)((x - x2) * (x - x2) + (y - y2) * (y - y2)));
					else d = fabs(h);

					src_x1 = source_lines[line].Px;
					src_y1 = source_lines[line].Py;
					src_x2 = source_lines[line].Qx;
					src_y2 = source_lines[line].Qy;
					src_line_length = sqrt((double)((src_x2 - src_x1)*(src_x2 - src_x1) +
						(src_y2 - src_y1)*(src_y2 - src_y1)));

					dest_x1 = dest_lines[line].Px;
					dest_y1 = dest_lines[line].Py;
					dest_x2 = dest_lines[line].Qx;
					dest_y2 = dest_lines[line].Qy;
					dest_line_length = sqrt((double)((dest_x2 - dest_x1)*(dest_x2 - dest_x1) +
						(dest_y2 - dest_y1)*(dest_y2 - dest_y1)));

					// �Է� ���� 1������ ���� �ȼ� ��ġ ��� 
					xp = src_x1 + u * (src_x2 - src_x1) -
						h * (src_y2 - src_y1) / src_line_length;
					yp = src_y1 + u * (src_y2 - src_y1) +
						h * (src_x2 - src_x1) / src_line_length;

					// �Է� ���� 2������ ���� �ȼ� ��ġ ��� 
					xp2 = dest_x1 + u * (dest_x2 - dest_x1) -
						h * (dest_y2 - dest_y1) / dest_line_length;
					yp2 = dest_y1 + u * (dest_y2 - dest_y1) +
						h * (dest_x2 - dest_x1) / dest_line_length;

					// ����� ���� ����ġ ��� 
					weight = pow((pow((double)(dest_line_length), p) / (a + d)), b);

					// �Է� ���� 1�� ���� �ȼ����� ���� ��� 
					tx += (xp - x) * weight;
					ty += (yp - y) * weight;

					// �Է� ���� 2�� ���� �ȼ����� ���� ��� 
					tx2 += (xp2 - x) * weight;
					ty2 += (yp2 - y) * weight;

					totalWeight += weight;
				}

				// �Է� ���� 1�� ���� �ȼ� ��ġ ���     
				source_x = x + (int)(tx / totalWeight + 0.5);
				source_y = y + (int)(ty / totalWeight + 0.5);

				// �Է� ���� 2�� ���� �ȼ� ��ġ ���
				source_x2 = x + (int)(tx2 / totalWeight + 0.5);
				source_y2 = y + (int)(ty2 / totalWeight + 0.5);

				// ������ ��踦 ������� �˻� 
				if (source_x < 0) source_x = 0;
				if (source_x > last_col) source_x = last_col;
				if (source_y < 0) source_y = 0;
				if (source_y > last_row) source_y = last_row;

				if (source_x2 < 0) source_x2 = 0;
				if (source_x2 > last_col) source_x2 = last_col;
				if (source_y2 < 0) source_y2 = 0;
				if (source_y2 > last_row) source_y2 = last_row;

				// ���� ��� ����
				warpedImg[y][x] = inputImg[source_y][source_x];
				warpedImg2[y][x] = inputImg2[source_y2][source_x2];
			}
		}

		// ���� ��� �պ�
		for (y = 0; y<imageHeight; y++)
			for (x = 0; x<imageWidth; x++) {
				int val = (int)((1.0 - fweight) * warpedImg[y][x] +
					fweight * warpedImg2[y][x]);
				if (val < 0) val = 0;
				if (val > 255) val = 255;
				morphedImg[frame - 1][y][x] = val;
			}
	}
}
