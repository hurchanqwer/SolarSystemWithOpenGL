
// MFCWithOpenGLView.cpp: CMFCWithOpenGLView 클래스의 구현
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "planet.h"

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCWithOpenGL.h"
#endif

#include "MFCWithOpenGLDoc.h"
#include "MFCWithOpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define _USE_MATH_DEFINES
#endif


//원그리는 변수_
float pi = M_PI;
float u = 0.05;
float v = 0.05;
float db = u * 2 * pi;
float da = v * pi;

//Texture
GLuint solarImg = 0;
GLuint mercuryImg = 0;
GLuint venusImg = 0;
GLuint earthImg = 0;
GLuint moonImg = 0;
GLuint marsImg = 0;
GLuint spaceImg = 0;


//variable
float camX = 0, camY = 0, zoom = 10;
enum ViewMode {Point,Lines, polygon};
ViewMode viewmode = polygon;
bool isClick = false;
bool isLight= true;
float WorldSpeed = 15;
float pauseSpeed;

//Planet 생성자
Planet Solar(1, 0);
Planet Mercury(0.15, 2.5f);
Planet Venus(0.2, 3.5f);
Planet Earth(0.2, 5.f);
Planet Moon(0.1, 0.5f);
Planet Mars(0.15, 6.5f);


//Light
GLfloat AmbientLightValue[] = { 1, 1, 1, 1 };
GLfloat DiffuseLightValue[] = { 1, 1, 1, 1 };
GLfloat SpecularLightValue[] = { 1, 1, 1, 1 };
GLfloat PositionLightValue[] = { 0,0,0,1 };

void Lighting() {

	glEnable(GL_LIGHTING); //조명을 사용
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLightValue);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLightValue);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLightValue);
	glLightfv(GL_LIGHT0, GL_POSITION, PositionLightValue);
}


//텍스쳐
GLuint LoadBMP(const char* imagepath) {

	GLuint textureID;
	unsigned char header[54]; 
	unsigned int width, height;
	unsigned int imageSize;   // = 너비*높이*3
	// 실제 RGB 데이터
	unsigned char* data;

	FILE* file = fopen(imagepath, "rb"); // 전달된 파일 경로 사용
	if (!file) {
		printf("파일을 열 수 없습니다: %s\n", imagepath);
		return 0;
	}

	// 파일 헤더로부터 너비와 높이 추출
	fread(header, 1, 54, file);
	memcpy(&width, &header[18], sizeof(int));
	memcpy(&height, &header[22], sizeof(int));
	imageSize = width * height * 3;

	data = new unsigned char[imageSize];

	// 파일에서 데이터를 버퍼로 읽기
	fread(data, imageSize,1, file);

	// 파일 닫기
	fclose(file);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}


void Planet::drawPlanet()

{
	glPushMatrix(); 
	 
	glRotatef(revo, 0, 1.f, 0);
	glTranslatef(0,0,position);
	glRotatef(rot, 0, 1.f, 0);

	float r = radius;

	int minus=1;
	if(this->radius == 1) minus = -1;
	
	for (float i = 0; i < 1.0; i += u) //horizonal
		for (float j = 0; j < 1.0; j += v) //vertical
		{
			
			float b = i * 2 * pi;      //0     to  2pi
			float a = (j - 0.5) * pi;  //-pi/2 to pi/2
		
			switch (viewmode) {
			case Point: glBegin(GL_POINTS);
				break;
			case Lines: glBegin(GL_LINE_STRIP);
				break;
			case polygon: glBegin(GL_POLYGON);
				break;
			}
			//normal
			glNormal3f(
				minus * cos(a) * cos(b),
				minus * sin(a),
				minus * cos(a) * sin(b));
			/*first*/
			glTexCoord2f(i, j);
			glVertex3f(
				r * cos(a) * cos(b),
				r * sin(a),
				r * cos(a) * sin(b));

			//normal
			glNormal3f(
				minus * cos(a) * cos(b + db),
				minus * sin(a),
				minus * cos(a) * sin(b + db));
			//Second
			glTexCoord2f(i + u, j);
			glVertex3f(
				r * cos(a) * cos(b + db),
				r * sin(a),
				r * cos(a) * sin(b + db));

			//normal
			glNormal3f(
				minus * cos(a + da) * cos(b + db),
				minus * sin(a + da),
				minus * cos(a + da) * sin(b + db));
			//third
			glTexCoord2f(i + u, j + v);
			glVertex3f(
				r * cos(a + da) * cos(b + db),
				r * sin(a + da),
				r * cos(a + da) * sin(b + db));

			//normal
			glNormal3f(
				minus * cos(a + da) * cos(b),
				minus * sin(a + da),
				minus * cos(a + da) * sin(b));
			//fourth
			glTexCoord2f(i, j + v);
			glVertex3f(
				r * cos(a + da) * cos(b),
				r * sin(a + da),
				r * cos(a + da) * sin(b));
			
			glEnd();
			
		}
		
}

void DrawSpace() {

	float pi = M_PI;
	float u = 0.05;
	float v = 0.05;
	float db = u * 2 * pi;
	float da = v * pi;
	float r = 100;

	for (float i = 0; i < 1.0; i += u) //horizonal
		
		for (float j = 0; j < 1.0; j += v) //vertical
		{
			float b = i * 2 * pi;      
			float a = (j - 0.5) * pi;  
			
			glBegin(GL_POLYGON);
			
			//first
			glTexCoord2f(i, j);
			glVertex3f(
				r * cos(a) * cos(b),
				r * sin(a),
				r * cos(a) * sin(b));
			//Second
			glTexCoord2f(i + u, j);
			glVertex3f(
				r * cos(a) * cos(b + db),
				r * sin(a),
				r * cos(a) * sin(b + db));
			//third
			glTexCoord2f(i + u, j + v);
			glVertex3f(
				r * cos(a + da) * cos(b + db),
				r * sin(a + da),
				r * cos(a + da) * sin(b + db));
			//fourth
			glTexCoord2f(i, j + v);
			glVertex3f(
				r * cos(a + da) * cos(b),
				r * sin(a + da),
				r * cos(a + da) * sin(b));
			glEnd();

	}
}


void CameraRot(double xAngle, double yAngle, double zoom) {
	double xx = zoom * cos(yAngle) * cos(xAngle);
	double yy = zoom * sin(yAngle);
	double zz = zoom * cos(yAngle) * sin(xAngle);
	int yoption = 1;

	if ((cos(yAngle) > 0 && sin(xAngle)) < 0 ||
		(cos(yAngle) < 0 && sin(xAngle)) > 0) {
		yoption = -1;
	}
	
	gluLookAt(xx, yy, zz, 0, 0, 0, 0, yoption, 0);
}
// CMFCWithOpenGLView

IMPLEMENT_DYNCREATE(CMFCWithOpenGLView, CView)

BEGIN_MESSAGE_MAP(CMFCWithOpenGLView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCWithOpenGLView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMFCWithOpenGLView 생성/소멸

CMFCWithOpenGLView::CMFCWithOpenGLView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCWithOpenGLView::~CMFCWithOpenGLView()
{
}

BOOL CMFCWithOpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCWithOpenGLView 그리기

void CMFCWithOpenGLView::OnDraw(CDC* /*pDC*/)
{
	CMFCWithOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	
		DrawGLScene();
}

void CMFCWithOpenGLView::DrawGLScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	CameraRot(camX, camY, zoom);

	Lighting();
	if (!isLight) { glDisable(GL_LIGHTING); }

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, spaceImg);
	DrawSpace();

	glBindTexture(GL_TEXTURE_2D, solarImg);
	Solar.drawPlanet();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, mercuryImg);
	Mercury.drawPlanet();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, venusImg);
	Venus.drawPlanet();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, earthImg);
	Earth.drawPlanet();
	glRotatef(-Earth.rot, 0, 1.f, 0);

	glBindTexture(GL_TEXTURE_2D, moonImg);
	Moon.drawPlanet();
	glPopMatrix();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, marsImg);
	Mars.drawPlanet();
	glPopMatrix();

	SwapBuffers(m_hDC);
}



// CMFCWithOpenGLView 인쇄
void CMFCWithOpenGLView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCWithOpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCWithOpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCWithOpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFCWithOpenGLView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCWithOpenGLView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCWithOpenGLView 진단

#ifdef _DEBUG
void CMFCWithOpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCWithOpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCWithOpenGLDoc* CMFCWithOpenGLView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCWithOpenGLDoc)));
	return (CMFCWithOpenGLDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CMFCWithOpenGLView::SetDevicePixelFormat(HDC hdc) {
	int pixelformat;

	PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW |
	PFD_SUPPORT_OPENGL |
	PFD_GENERIC_FORMAT |
	PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32,
	0, 0 ,0 ,0 ,0 ,0,
	8,
	0,
	8,
	0,0,0,0,
	16,
	0,
	0,
	PFD_MAIN_PLANE,
	0,
	0, 0, 0
	};

	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE) {
		MessageBox(LPCTSTR("ChoosePixelFormat failed"), LPCTSTR("Error"), MB_OK);
		return FALSE;
	}
	
	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE){
		MessageBox(LPCTSTR("SetPixelFormat failed"), LPCTSTR("Error"), MB_OK);
	}

	return TRUE;
}
float x;
// CMFCWithOpenGLView 메시지 처리기



int CMFCWithOpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_hDC = GetDC()->m_hDC;

	if (!SetDevicePixelFormat(m_hDC)) {
		return-1;
	}

	m_hglRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hglRC);

	InitGL();
	return 0;
}


void CMFCWithOpenGLView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	wglMakeCurrent(m_hDC, NULL);
	wglDeleteContext(m_hglRC);
}
void CMFCWithOpenGLView::InitGL(GLvoid) {
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	SetTimer(1, 1000 / 60, NULL);

	//텍스쳐 로딩
	spaceImg = LoadBMP("res/space.bmp");
	solarImg = LoadBMP("res/solar.bmp");
	mercuryImg = LoadBMP("res/mercury.bmp");
	venusImg = LoadBMP("res/venus.bmp");
	earthImg = LoadBMP("res/earth.bmp");
	moonImg = LoadBMP("res/moon.bmp");
	marsImg = LoadBMP("res/mars.bmp");

}



void CMFCWithOpenGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ReSizeGLScene(cx, cy);
}
void CMFCWithOpenGLView::ReSizeGLScene(GLsizei width, GLsizei height) {
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}



void CMFCWithOpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	
	
	switch (nChar) {
	
	case VK_F1:
		viewmode = Point;
		break;
	case VK_F2: 
		viewmode = Lines;
		break;
	case VK_F3:
		viewmode = polygon;
		break;
	case VK_F4:
		isLight = !isLight;
		break;

	case 80:
		if (pauseSpeed == 0) {
			pauseSpeed = WorldSpeed;
			WorldSpeed = 0;
		}
		else {
			WorldSpeed = pauseSpeed;
			pauseSpeed=0;
		}
		break;

	case VK_UP:
		if (pauseSpeed != 0) return;

		WorldSpeed += 3;
		break;
	case VK_DOWN:
		if (pauseSpeed != 0) return;

		if (WorldSpeed > 0)
		WorldSpeed -= 3;
		break;
	}
}


BOOL CMFCWithOpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (zDelta > 0) {
		zoom -= 1.f;
	}
	else { zoom += 1.f; }
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

CPoint prevPoint;
void CMFCWithOpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (isClick) {
		float xChange = point.x - prevPoint.x;
		float yChange = point.y - prevPoint.y;

		camY += 0.01f * yChange;
		camX += 0.01f * xChange;
		prevPoint = point;
	}
	
	CView::OnMouseMove(nFlags, point);
}


void CMFCWithOpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	isClick = true;
	prevPoint = point;
	CView::OnLButtonDown(nFlags, point);
	
}

void CMFCWithOpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	isClick = false;
	CView::OnLButtonUp(nFlags, point);
	
}


void CMFCWithOpenGLView::OnTimer(UINT_PTR nIDEvent)
{
		//공전
		Mercury.revo += 1.f / 88 * WorldSpeed;
		if (Mercury.revo > 360.f) { Mercury.revo = 0; }

		Venus.revo += 1.f / 225 * WorldSpeed;
		if (Venus.revo > 360.f) { Venus.revo = 0; }

		Earth.revo += 1.f / 365 * WorldSpeed;
		if (Earth.revo > 360.f) { Earth.revo = 0; }

		Moon.revo += 1.f / 27.32 * WorldSpeed;
		if (Moon.revo > 360.f) { Moon.revo = 0; }

		Mars.revo += 1.f / 687 * WorldSpeed;
		if (Mars.revo > 360.f) { Mars.revo = 0; }

		//자전
		Solar.rot += 1.f / 25 * WorldSpeed;
		if (Solar.rot > 360.f) { Solar.rot = 0; }

		Mercury.rot += 1.f / 58 * WorldSpeed;
		if (Mercury.rot > 360.f) { Mercury.rot = 0; }

		Venus.rot -= 1.f / 243 * WorldSpeed;
		if (Venus.rot < -360.f) { Venus.rot = 0; }

		Earth.rot += 1.f * WorldSpeed;
		if (Earth.rot > 360.f) { Earth.rot = 0; }

		Moon.rot += 1.f / 27.32 * WorldSpeed;
		if (Moon.rot > 360.f) { Moon.rot = 0; }

		Mars.rot += 1.3f * WorldSpeed;
		if (Mars.rot > 360.f) { Mars.rot = 0; }

		DrawGLScene();
}
