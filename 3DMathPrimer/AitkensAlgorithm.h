#pragma once

#ifndef _AITKENSALGORITHM_
#define _AITKENSALGORITHM_

#include <vector>
#include "Funccomm.h"

using std::vector;

namespace aitkinsAlgorithm {

vector<tCoordinate>  interpolation(const std::vector<tCoordinate> controlPoints);

/*
	Do one iteration for Aitken's algorithm.
*/
tCoordinate aitkensAlgoritm(int i, int j, float t, const std::vector<tCoordinate> controlPoints, vector<float> knotVector);
void drawNameOfAlgorithm(HWND hWnd, HDC hdc);


void aitkinsalgorithm_initialize_work();
void aitkinsalgorithm_draw(HWND hWnd, HDC hdc);
void aitkinsalgorithm_addControlPoint(const POINT& point);

void drawInterpolated(HWND hWnd, HDC hdc);
void drawControlPoint(HWND hWnd, HDC hdc);

}

#endif
