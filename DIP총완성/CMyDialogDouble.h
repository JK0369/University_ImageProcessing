#pragma once


// CMyDialogDouble 대화 상자

class CMyDialogDouble : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialogDouble)

public:
	CMyDialogDouble(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMyDialogDouble();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	double m_double;
};
