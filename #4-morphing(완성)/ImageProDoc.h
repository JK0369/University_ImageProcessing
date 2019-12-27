
// ImageProDoc.h : CImageProDoc Ŭ������ �������̽�
//

typedef struct
{
	int Px;
	int Py;
	int Qx;
	int Qy;
} control_line;

#pragma once


class CImageProDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CImageProDoc();
	DECLARE_DYNCREATE(CImageProDoc)

// Ư���Դϴ�.
public:
	CString name; // ���� �̸� ����
	//
	unsigned char **inputImg;   // �Է� ������ ��� ��ҿ� ���� ������ ����
	unsigned char **inputImg2;   // �Է� ������ ��� ��ҿ� ���� ������ ����
	unsigned char **resultImg;   // ��� ������ ��� ��ҿ� ���� ������ ����
	int imageWidth;      // ������ ���� ũ��
	int imageHeight;      // ������ ���� ũ��
	int depth;         // 1 = ��� ����, 3 = �÷� ����
	unsigned char histoImg[256][256]; //histo image

					   // ��� ������ ũ�Ⱑ ��ȭ�Ǵ� �������� ó���� ���� ������ �߰��Ǵ� �ڵ�
	int gImageWidth;           // ũ�Ⱑ ���� ��� ������ ���� ũ��
	int gImageHeight;           // ũ�Ⱑ ���� ��� ������ ���� ũ��
	unsigned char **gResultImg;  // ũ�Ⱑ ���� ��� ���� ���� ������ ���� 
	unsigned char **morphedImg[8];
	//
   
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
	void SubAdd(void);
	void SubMul(void);
	void Histogram(void);
	void StretchingEedInSearch(void);
	void NegativeTransformation(void);
	void Binaryimage(void);

	int m_int;
	double m_double;
	double m_min, m_max;
	double m_threshold;
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
	void GeometryZoom(void);
	void GeometryZoominInterpolation(float scale);
	void GeometryZoomoutSubsampling(float scale);
	void GeometryRotate(void);
	void rotateFunction(float);
	void GeometryOrigin(void);
	void GeometryMorphing(POINT[], POINT[], POINT[], POINT[]);
};
