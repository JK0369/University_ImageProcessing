// CMyDialogMinMax.cpp: 구현 파일
//

#include "stdafx.h"
#include "ImagePro.h"
#include "CMyDialogMinMax.h"
#include "afxdialogex.h"


// CMyDialogMinMax 대화 상자

IMPLEMENT_DYNAMIC(CMyDialogMinMax, CDialogEx)

CMyDialogMinMax::CMyDialogMinMax(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, m_min(0)
	, m_max(0)
{

}

CMyDialogMinMax::~CMyDialogMinMax()
{
}

void CMyDialogMinMax::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_min);
	DDV_MinMaxDouble(pDX, m_min, 0, 255);
	DDX_Text(pDX, IDC_EDIT1, m_max);
	DDV_MinMaxDouble(pDX, m_max, 0, 255);
}


BEGIN_MESSAGE_MAP(CMyDialogMinMax, CDialogEx)
END_MESSAGE_MAP()


// CMyDialogMinMax 메시지 처리기
