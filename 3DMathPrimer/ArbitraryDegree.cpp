
#define _USE_MATH_DEFINES
#include <math.h>
#include "mymath.h"

#include <vector>

#include "framework.h"
#include "arbitraryDegree.h"
#include "funccomm.h"

using namespace mymath;
using std::vector;

// work area for Arbitrary Degree.
static std::vector<tCoordinate*> g_controlPoints;
static tCoordinate* g_selectedControllPoint = nullptr;
static float g_offset_x;
static float g_offset_y;
static bool g_mouseDownFlag;


int arbitraryDegree::factorial( int inN) {

	if (inN == 0)
		return 1;

	return inN * factorial(inN - 1);
}

float arbitraryDegree::binomial_coefficient(int inN, int inK) {

	return factorial(inN) / (factorial(inK) * factorial(inN - inK));

}

float arbitraryDegree::B(int inN, int inI, float inT) {
	float ttoi = powf(inT, inI);
	float oneminusttonminui = powf((1.0f - inT), inN - inI);
	return binomial_coefficient(inN, inI)* ttoi * oneminusttonminui;
}

Vectorf2 arbitraryDegree::bezier_curve(float inT, HWND hWnd, HDC hdc) {

	int n = g_controlPoints.size();

	Vectorf2* pControlPoints = new Vectorf2[n];

	for (int i = 0; i < n; i++) {
		pControlPoints[i][0] = g_controlPoints[i]->x;
		pControlPoints[i][1] = g_controlPoints[i]->y;
	}

	Vectorf2 bn0{ 0.0f, 0.0f };

	for (int i = 0; i <= n-1; i++) {
		bn0 = bn0 + pControlPoints[i] * B(n-1, i, inT);
	}

	delete[] pControlPoints;

	return bn0;
}

void arbitraryDegree::drawInterpolated(HWND hWnd, HDC hdc) {

	Vectorf2* pInterpolated = new Vectorf2[100];

	float t = 0.0f;
	for (int i = 0; i < 99; i++, t = t + 0.01f) {
		pInterpolated[i] = arbitraryDegree::bezier_curve(t, hWnd, hdc);
	}

	//
	// draw last control point.
	//

	t = 1.0f;
	pInterpolated[99] = arbitraryDegree::bezier_curve(t, hWnd, hdc);

	//
	// draw a curve.
	//

	POINT pt;

	MoveToEx(hdc, pInterpolated[0][0], pInterpolated[0][1], &pt);

	for (int i = 1; i < 100; i++) {

		LineTo(hdc, pInterpolated[i][0], pInterpolated[i][1]);
		MoveToEx(hdc, pInterpolated[i][0], pInterpolated[i][1], &pt);

	}

}

void arbitraryDegree::drawControlPoint(HWND hWnd, HDC hdc) {
	for (tCoordinate* ptCoordinate : g_controlPoints) {

		drawCrossedHair(hWnd, hdc, ptCoordinate->x, ptCoordinate->y);
	}
}
void arbitraryDegree::drawNameOfAlgorithm(HWND hWnd, HDC hdc) {

	const LPCWSTR name{ _T("Bezier Curve in Arbitrary Degree") };

	RECT rect;
	GetClientRect(hWnd, &rect);

	DrawText(
		hdc,
		name,
		wcslen(name),
		&rect,
		DT_CENTER | DT_TOP
	);
}

void arbitraryDegree::bezier_curve_arbitrary_degree_draw(HWND hWnd, HDC hdc) {

	HPEN hRedPen;
	HPEN hBlackPen;
	hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	hBlackPen = CreatePen(PS_SOLID, 1, RGB(1, 1, 1));

	SelectObject(hdc, hRedPen);

	drawNameOfAlgorithm(hWnd, hdc);

	arbitraryDegree::drawInterpolated(hWnd, hdc);

	// Draw control points.

	SelectObject(hdc, hBlackPen);
	arbitraryDegree::drawControlPoint(hWnd, hdc);


	DeleteObject(hRedPen);
	DeleteObject(hBlackPen);

}

bool arbitraryDegree::isOverlappped(const POINT& inPoint, tCoordinate* inPtCoordinate) {

	float delX = inPtCoordinate->x - inPoint.x;
	float delY = inPtCoordinate->y - inPoint.y;

	float r = (10.0f * 10.0f);

	if (r >= powf(delX, 2.0f) + powf(delY, 2.0f)) {

		return true;
	}

	return false;
}

void arbitraryDegree::bezier_curve_arbitrary_degree_lButtonUp(const POINT& inPoint) {
	g_mouseDownFlag = false;
}

void arbitraryDegree::bezier_curve_arbitrary_degree_moveControlPoint(const POINT& point) {

	if (g_selectedControllPoint == nullptr || g_mouseDownFlag == false )
		return;

	g_selectedControllPoint->x = point.x + g_offset_x;
	g_selectedControllPoint->y = point.y + g_offset_y;

	return;
}


void arbitraryDegree::bezier_curve_arbitrary_degree_addOrSelectControlPoint(const POINT& point) {

	g_mouseDownFlag = true;
	bool overlapped = false;

	for (tCoordinate* ptCoordinate : g_controlPoints) {

		if (arbitraryDegree::isOverlappped(point, ptCoordinate)) {
			g_selectedControllPoint = ptCoordinate;
			g_offset_x = ptCoordinate->x - point.x;
			g_offset_y = ptCoordinate->y - point.y;

			overlapped = true;
			break;
		}
	}

	if (overlapped == false) {

		// deselect.
		g_selectedControllPoint = nullptr;

		//
		// add nwe control point.
		//

		tCoordinate* newPtCoordinate = new tCoordinate();
		newPtCoordinate->x = point.x;
		newPtCoordinate->y = point.y;

		g_controlPoints.push_back(newPtCoordinate);
	}

}

void arbitraryDegree::bezier_curve_arbitrary_degree_initialize_work() {
}
