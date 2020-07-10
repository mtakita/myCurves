
#define _USE_MATH_DEFINES
#include <math.h>
#include "mymath.h"

#include <vector>

#include "framework.h"
#include "DeCasteljuRecurrenceRelation.h"
#include "funccomm.h"

using namespace mymath;
using std::vector;

// work area for De Castelju Recurrence Relation.
static std::vector<tCoordinate*> g_controlPoints;
static tCoordinate* g_selectedControllPoint = nullptr;
static float g_offset_x;
static float g_offset_y;
static bool g_mouseDownFlag;

Vectorf2 deCasteljuRecurrenceRelation::calcT(float inT, HWND hWnd, HDC hdc) {

	int n = g_controlPoints.size();
	Vectorf2* pControlPoints = new Vectorf2[n];

	for (int i = 0; i < n; i++) {
		pControlPoints[i][0] = g_controlPoints[i]->x;
		pControlPoints[i][1] = g_controlPoints[i]->y;
	}

	while (n > 1) {
		--n;
		for (int i = 0; i < n; i++) {
			pControlPoints[i] = pControlPoints[i] * inT + pControlPoints[i + 1] * (1 - inT);
		}
	}

	Vectorf2 ret = pControlPoints[0];
	delete[] pControlPoints;

	return ret;
}

void deCasteljuRecurrenceRelation::drawInterpolated(HWND hWnd, HDC hdc) {

	Vectorf2* pInterpolated = new Vectorf2[100];

	float t = 0.0f;
	for (int i = 0; i < 99; i++, t = t + 0.01f) {
		pInterpolated[i] = deCasteljuRecurrenceRelation::calcT( t, hWnd, hdc);
	}

	//
	// draw last control point.
	//

	t = 1.0f;
	pInterpolated[99] = deCasteljuRecurrenceRelation::calcT(t, hWnd, hdc);

	//
	// draw a curve.
	//

	POINT pt;

	MoveToEx(hdc, pInterpolated[0][0], pInterpolated[0][1], &pt);

	for( int i = 1; i < 100; i++ ) {

		LineTo(hdc, pInterpolated[i][0], pInterpolated[i][1]);
		MoveToEx(hdc, pInterpolated[i][0], pInterpolated[i][1], &pt);

	}

}

void deCasteljuRecurrenceRelation::drawControlPoint(HWND hWnd, HDC hdc) {
	for (tCoordinate* ptCoordinate : g_controlPoints) {

		drawCrossedHair(hWnd, hdc, ptCoordinate->x, ptCoordinate->y);
	}
}
void deCasteljuRecurrenceRelation::drawNameOfAlgorithm(HWND hWnd, HDC hdc) {

	const LPCWSTR name{ _T("Bezier Curve(De Castelju Recurrence Relation)") };

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

void deCasteljuRecurrenceRelation::bezier_curve_decastlju_recurrence_relation_draw(HWND hWnd, HDC hdc) {

	HPEN hRedPen;
	HPEN hBlackPen;
	hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	hBlackPen = CreatePen(PS_SOLID, 1, RGB(1, 1, 1));

	SelectObject(hdc, hRedPen);

	drawNameOfAlgorithm(hWnd, hdc);

	deCasteljuRecurrenceRelation::drawInterpolated(hWnd, hdc);

	// Draw control points.

	SelectObject(hdc, hBlackPen);
	deCasteljuRecurrenceRelation::drawControlPoint(hWnd, hdc);


	DeleteObject(hRedPen);
	DeleteObject(hBlackPen);

}

bool deCasteljuRecurrenceRelation::isOverlappped(const POINT& inPoint, tCoordinate* inPtCoordinate) {

	float delX = inPtCoordinate->x - inPoint.x;
	float delY = inPtCoordinate->y - inPoint.y;

	float r = (10.0f * 10.0f);

	if (r >= powf(delX, 2.0f) + powf(delY, 2.0f)){

		return true;
	}

	return false;
}

void deCasteljuRecurrenceRelation::bezier_curve_decastlju_recurrence_relation_moveControlPoint( const POINT& point) {

	if (g_selectedControllPoint == nullptr || g_mouseDownFlag == false)
		return;

	g_selectedControllPoint->x = point.x + g_offset_x;
	g_selectedControllPoint->y = point.y + g_offset_y;

	return;
}

void deCasteljuRecurrenceRelation::bezier_curve_decastlju_recurrence_relation_lButtonUp(const POINT& inPoint) {
	g_mouseDownFlag = false;
}


void deCasteljuRecurrenceRelation::bezier_curve_decastlju_recurrence_relation_addOrSelectControlPoint(const POINT& point) {

	g_mouseDownFlag = true;
	bool overlapped = false;

	for (tCoordinate* ptCoordinate : g_controlPoints) {

		if (deCasteljuRecurrenceRelation::isOverlappped(point, ptCoordinate)) {
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

void deCasteljuRecurrenceRelation::bezier_curve_decastlju_recurrence_relation_initialize_work() {
}
