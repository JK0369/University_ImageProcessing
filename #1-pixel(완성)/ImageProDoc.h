
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
   unsigned char ** inputImg;
   unsigned char ** resultImg;
   unsigned char histoImg[256][256];
   int imageWidth;
   int imageHeight;
   int depth;
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
	void LoadImageFile(CArchive &);
};
