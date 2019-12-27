
// ImageProDoc.h : CImageProDoc 클래스의 인터페이스
//


#pragma once


class CImageProDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CImageProDoc();
	DECLARE_DYNCREATE(CImageProDoc)

// 특성입니다.
public:
   unsigned char ** inputImg;
   unsigned char ** resultImg;
   unsigned char histoImg[256][256];
   int imageWidth;
   int imageHeight;
   int depth;
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CImageProDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
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
