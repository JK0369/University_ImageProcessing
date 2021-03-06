#pragma once


// CMyDialogAngle 대화 상자

class CMyDialogAngle : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialogAngle)

public:
	CMyDialogAngle(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMyDialogAngle();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_angle;
};
