
// ImageProDoc.h : CImageProDoc 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CImageProDoc();
	DECLARE_DYNCREATE(CImageProDoc)

// 특성입니다.
public:
	CString name; // 파일 이름 저장
	//
	unsigned char **inputImg;   // 입력 영상의 기억 장소에 대한 포인터 변수
	unsigned char **inputImg2;   // 입력 영상의 기억 장소에 대한 포인터 변수
	unsigned char **resultImg;   // 출력 영상의 기억 장소에 대한 포인터 변수
	int imageWidth;      // 영상의 가로 크기
	int imageHeight;      // 영상의 세로 크기
	int depth;         // 1 = 흑백 영상, 3 = 컬러 영상
	unsigned char histoImg[256][256]; //histo image

					   // 결과 영상의 크기가 변화되는 기하학적 처리를 위해 새로이 추가되는 코드
	int gImageWidth;           // 크기가 변한 결과 영상의 가로 크기
	int gImageHeight;           // 크기가 변한 결과 영상의 세로 크기
	unsigned char **gResultImg;  // 크기가 변한 결과 영상에 대한 포인터 변수 
	unsigned char **morphedImg[8];
	//
   
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
