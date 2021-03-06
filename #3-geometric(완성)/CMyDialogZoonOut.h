#pragma once


// CMyDialogZoonOut 대화 상자

class CMyDialogZoonOut : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialogZoonOut)

public:
	CMyDialogZoonOut(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMyDialogZoonOut();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_zoom_out_scale;
};
