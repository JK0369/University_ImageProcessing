// CMyDialogThreshold.cpp: 구현 파일
//

#include "stdafx.h"
#include "ImagePro.h"
#include "CMyDialogThreshold.h"
#include "afxdialogex.h"


// CMyDialogThreshold 대화 상자

IMPLEMENT_DYNAMIC(CMyDialogThreshold, CDialogEx)

CMyDialogThreshold::CMyDialogThreshold(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
	, m_threshold(0)
{

}

CMyDialogThreshold::~CMyDialogThreshold()
{
}

void CMyDialogThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_threshold);
	DDV_MinMaxDouble(pDX, m_threshold, 0, 255);
}


BEGIN_MESSAGE_MAP(CMyDialogThreshold, CDialogEx)
END_MESSAGE_MAP()


// CMyDialogThreshold 메시지 처리기
