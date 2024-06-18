
// MFCWithOpenGLView.h: CMFCWithOpenGLView 클래스의 인터페이스
//
#include <gl/GL.h>
#include<gl/GLU.h>
#include<math.h>
#include <cstdio>

#pragma comment(lib, "OPENGL32.LIB")
#pragma comment(lib, "GLU32.LIB")

class CMFCWithOpenGLView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFCWithOpenGLView() noexcept;
	DECLARE_DYNCREATE(CMFCWithOpenGLView)

// 특성입니다.
public:
	CMFCWithOpenGLDoc* GetDocument() const;

// 작업입니다.
public:
	HDC m_hDC;
	HGLRC m_hglRC;
	bool b_mouseMove;
public:
	BOOL SetDevicePixelFormat(HDC hdc);
	void InitGL(void);
	void ReSizeGLScene(GLsizei width, GLsizei height);
	void DrawGLScene(void);
	
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFCWithOpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // MFCWithOpenGLView.cpp의 디버그 버전
inline CMFCWithOpenGLDoc* CMFCWithOpenGLView::GetDocument() const
   { return reinterpret_cast<CMFCWithOpenGLDoc*>(m_pDocument); }
#endif

