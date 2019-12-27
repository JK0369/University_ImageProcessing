
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

// ��� ������ ũ�Ⱑ ��ȭ�Ǵ� �������� ó���� ���� ������ �߰��Ǵ� �ڵ�
   int gImageWidth;           // ũ�Ⱑ ���� ��� ������ ���� ũ��
   int gImageHeight;           // ũ�Ⱑ ���� ��� ������ ���� ũ��
   unsigned char **gResultImg;  // ũ�Ⱑ ���� ��� ���� ���� ������ ���� 

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
	void RegionSharpening(void);
	void Convolve(unsigned char ** inputImg, unsigned char ** resultImg, int cols, int rows, float mask[][3], int bias, int depth);
	void RegionMeaning(void);
	void RegionSobel(void);
	void RegionMedian(void);
	void Erosion(void);
	void Dilation(void);
	void Opening(void);
	void CopyResultToInput(void);
	void Closing(void);
	void GeometryZoominPixelCopy(void);
	void GeometryZoominInterpolation(void);
	void GeometryZoomoutSubsampling(void);
	void GeometryZoomoutAvg(void);
	void GeometryRotate(void);
	void GeometryMirror(void);
	void GeometryFlip(void);
	void rotateFunction(float);
	void GeometryOrigin(void);
};
