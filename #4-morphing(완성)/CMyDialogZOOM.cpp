// CMyDialogZOOM.cpp: 구현 파일
//

#include "stdafx.h"
#include "ImagePro.h"
#include "CMyDialogZOOM.h"
#include "afxdialogex.h"


// CMyDialogZOOM 대화 상자

IMPLEMENT_DYNAMIC(CMyDialogZOOM, CDialogEx)

CMyDialogZOOM::CMyDialogZOOM(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
	, m_zoom_size(0)
{

}

CMyDialogZOOM::~CMyDialogZOOM()
{
}

void CMyDialogZOOM::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_zoom_size);
	DDV_MinMaxFloat(pDX, m_zoom_size, 0, FLT_MAX);
}


BEGIN_MESSAGE_MAP(CMyDialogZOOM, CDialogEx)
END_MESSAGE_MAP()


// CMyDialogZOOM 메시지 처리기
