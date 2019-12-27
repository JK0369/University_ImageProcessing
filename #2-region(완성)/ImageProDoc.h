
// ImageProDoc.h : CImageProDoc Ŭ������ �������̽�
//


#pragma once


class CImageProDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CImageProDoc();
	DECLARE_DYNCREATE(CImageProDoc)

// Ư���Դϴ�.
public:
   unsigned char **inputImg;   // �Է� ������ ��� ��ҿ� ���� ������ ����
   unsigned char **inputImg2;   // �Է� ������ ��� ��ҿ� ���� ������ ����
   unsigned char **resultImg;   // ��� ������ ��� ��ҿ� ���� ������ ����
   int imageWidth;      // ������ ���� ũ��
   int imageHeight;      // ������ ���� ũ��
   int depth;         // 1 = ��� ����, 3 = �÷� ����
   CString name; // ���� �̸� ����
// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CImageProDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	void PixelAdd(void);
	void PixelHistoEq(void);
	void PixelTwoImageAdd(void);
	void LoadTwoImages(void);
	void LoadImageFile(CArchive& ar);
	void LoadSecondImageFile(CArchive& ar);
	void EmbossingSub45(void);
	void EmbossingSub90(void);
	void EmbossingSub135(void);
	void AverageSub3x3(void);
	void AverageSub5x5(void);
	void SharpeningSubMask1(void);
	void SharpeningSubMask2(void);
	void SharpeningSubMask3(void);
	void EdgeSubSobel(void);
	void EdgeSubPrewitt(void);
	void EdgeSubRoberts(void);
	void Median(void);
	void MySaveCustomState(void);
	void Convolve(unsigned char ** inputImg, unsigned char ** resultImg, int cols, int rows, float ** mask, int bias, int depth, int weight);
};
