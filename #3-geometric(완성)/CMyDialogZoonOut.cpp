// CMyDialogZoonOut.cpp: 구현 파일
//

#include "stdafx.h"
#include "ImagePro.h"
#include "CMyDialogZoonOut.h"
#include "afxdialogex.h"


// CMyDialogZoonOut 대화 상자

IMPLEMENT_DYNAMIC(CMyDialogZoonOut, CDialogEx)

CMyDialogZoonOut::CMyDialogZoonOut(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_zoom_out_scale(0)
{

}

CMyDialogZoonOut::~CMyDialogZoonOut()
{
}

void CMyDialogZoonOut::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_zoom_out_scale);
	DDV_MinMaxFloat(pDX, m_zoom_out_scale, 0, 1);
}


BEGIN_MESSAGE_MAP(CMyDialogZoonOut, CDialogEx)
END_MESSAGE_MAP()


// CMyDialogZoonOut 메시지 처리기
