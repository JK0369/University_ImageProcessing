// CMyDialogZoomIn.cpp: 구현 파일
//

#include "stdafx.h"
#include "ImagePro.h"
#include "CMyDialogZoomIn.h"
#include "afxdialogex.h"


// CMyDialogZoomIn 대화 상자

IMPLEMENT_DYNAMIC(CMyDialogZoomIn, CDialogEx)

CMyDialogZoomIn::CMyDialogZoomIn(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_zoom_in_scale(0)
{

}

CMyDialogZoomIn::~CMyDialogZoomIn()
{
}

void CMyDialogZoomIn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_zoom_in_scale);
	DDV_MinMaxFloat(pDX, m_zoom_in_scale, 1, FLT_MAX);
}


BEGIN_MESSAGE_MAP(CMyDialogZoomIn, CDialogEx)
END_MESSAGE_MAP()


// CMyDialogZoomIn 메시지 처리기
