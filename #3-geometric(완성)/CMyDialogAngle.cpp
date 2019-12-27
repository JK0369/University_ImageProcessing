// CMyDialogAngle.cpp: 구현 파일
//

#include "stdafx.h"
#include "ImagePro.h"
#include "CMyDialogAngle.h"
#include "afxdialogex.h"


// CMyDialogAngle 대화 상자

IMPLEMENT_DYNAMIC(CMyDialogAngle, CDialogEx)

CMyDialogAngle::CMyDialogAngle(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, m_angle(0)
{

}

CMyDialogAngle::~CMyDialogAngle()
{
}

void CMyDialogAngle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_angle);
}


BEGIN_MESSAGE_MAP(CMyDialogAngle, CDialogEx)
END_MESSAGE_MAP()


// CMyDialogAngle 메시지 처리기
