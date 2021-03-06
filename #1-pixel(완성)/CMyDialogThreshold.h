#pragma once


// CMyDialogThreshold 대화 상자

class CMyDialogThreshold : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialogThreshold)

public:
	CMyDialogThreshold(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMyDialogThreshold();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	double m_threshold;
};
