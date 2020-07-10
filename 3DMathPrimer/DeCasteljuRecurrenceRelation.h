#pragma once
#ifndef _DE_CASTELJU_RECURRENCE_RELATION_

#define _DE_CASTELJU_RECURRENCE_RELATION_

#define _USE_MATH_DEFINES
#include <math.h>
#include "mymath.h"

#include "Funccomm.h"

using namespace mymath;

namespace deCasteljuRecurrenceRelation {

	void drawInterpolated(HWND hWnd, HDC hdc);
	Vectorf2 calcT(float inT, HWND hWnd, HDC hdc);

	void drawNameOfAlgorithm(HWND hWnd, HDC hdc);
	void drawControlPoint(HWND hWnd, HDC hdc);
	void bezier_curve_decastlju_recurrence_relation_initialize_work();
	void bezier_curve_decastlju_recurrence_relation_draw(HWND hWnd, HDC hdc);
	void bezier_curve_decastlju_recurrence_relation_addOrSelectControlPoint(const POINT& point);
	void bezier_curve_decastlju_recurrence_relation_moveControlPoint( const POINT& point);
	void bezier_curve_decastlju_recurrence_relation_lButtonUp(const POINT& inPoint);

	bool isOverlappped(const POINT& inPoint, tCoordinate* inPtCoordinate);

}

#endif
