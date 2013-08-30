#pragma once
#include "afxdlgs.h"
#include "attributespage.h"
#include "materialspage.h"



// COperateView ������ͼ

class COperateView : public CFormView
{
	DECLARE_DYNCREATE(COperateView)

protected:
	COperateView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~COperateView();

public:
	enum { IDD = IDD_DIALOG_OPERATE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
protected:
	CPropertySheet m_sheet;
	CAttributesPage m_attributesPage;
	CMaterialsPage m_materialsPage;
};

