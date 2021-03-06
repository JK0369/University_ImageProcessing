// CMyDialogDouble.cpp: 구현 파일
//

#include "stdafx.h"
#include "ImagePro.h"
#include "CMyDialogDouble.h"
#include "afxdialogex.h"


// CMyDialogDouble 대화 상자

IMPLEMENT_DYNAMIC(CMyDialogDouble, CDialogEx)

CMyDialogDouble::CMyDialogDouble(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_double(0)
{

}

CMyDialogDouble::~CMyDialogDouble()
{
}

void CMyDialogDouble::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_double);
	DDV_MinMaxDouble(pDX, m_double, 0, DBL_MAX);
}


BEGIN_MESSAGE_MAP(CMyDialogDouble, CDialogEx)
END_MESSAGE_MAP()


// CMyDialogDouble 메시지 처리기
