// CMyDialogInt.cpp: 구현 파일
//

#include "stdafx.h"
#include "ImagePro.h"
#include "CMyDialogInt.h"
#include "afxdialogex.h"


// CMyDialogInt 대화 상자

IMPLEMENT_DYNAMIC(CMyDialogInt, CDialogEx)

CMyDialogInt::CMyDialogInt(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_int(0)
{

}

CMyDialogInt::~CMyDialogInt()
{
}

void CMyDialogInt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_int);
}


BEGIN_MESSAGE_MAP(CMyDialogInt, CDialogEx)
END_MESSAGE_MAP()


// CMyDialogInt 메시지 처리기
