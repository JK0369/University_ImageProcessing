#pragma once


// CMyDialogZOOM 대화 상자

class CMyDialogZOOM : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialogZOOM)

public:
	CMyDialogZOOM(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMyDialogZOOM();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_zoom_size;
};
