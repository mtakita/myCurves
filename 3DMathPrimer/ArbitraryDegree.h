#pragma once

#ifndef _ARBITRARY_DEGREE_

#define _ARBITRARY_DEGREE_

#define _USE_MATH_DEFINES
#include <math.h>
#include "mymath.h"

#include "Funccomm.h"

using namespace mymath;

namespace arbitraryDegree {

	void drawInterpolated(HWND hWnd, HDC hdc);

	void drawNameOfAlgorithm(HWND hWnd, HDC hdc);
	void drawControlPoint(HWND hWnd, HDC hdc);
	void bezier_curve_arbitrary_degree_initialize_work();
	void bezier_curve_arbitrary_degree_draw(HWND hWnd, HDC hdc);
	void bezier_curve_arbitrary_degree_addOrSelectControlPoint(const POINT& inPoint);
	void bezier_curve_arbitrary_degree_moveControlPoint(const POINT& inPoint);
	void bezier_curve_arbitrary_degree_lButtonUp(const POINT& inPoint);

	bool isOverlappped(const POINT& inPoint, tCoordinate* inPtCoordinate);

	int factorial(int inN);
	float binomial_coefficient(int inN, int inK);
	float B(int inN, int inI, float inT);
	Vectorf2 bezier_curve(float inT, HWND hWnd, HDC hdc);
}

#endif
