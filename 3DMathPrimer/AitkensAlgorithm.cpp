
#include <vector>

#include "framework.h"
#include "AitkensAlgorithm.h"
#include "funccomm.h"

using std::vector;

// work area for aitkin's algorithm.
std::vector<tCoordinate> g_coordinatesInterpolated;
std::vector<tCoordinate> g_controlPoints;


vector<tCoordinate>  aitkinsAlgorithm::interpolation(const std::vector<tCoordinate> controlPoints ) {

	if (controlPoints.size() < 2) {
		return vector<tCoordinate>{};
	}

	// Pairs of cooridnate x and y.
//	float cp_y[] = { 100, 250, 450, 600, 900 };
//	float cp_x[] = { 50, 100, 150, 200, 250 };
//	float knotVector[] = { 0, 0.25, 0.5, 0.75, 1 };

	// Prepare knotVector.  
	int numOfKnotVector = controlPoints.size();
	float interval = 1.0f / numOfKnotVector;


	float t = 0.0f;

	vector<float> knotVector;

	for (int i = 0; i < numOfKnotVector; i++) {
		if (i == (numOfKnotVector-1)) {
			knotVector.push_back(1.0f);
		}else {
			knotVector.push_back(t);
		}
		t = t + interval;
	}




	vector<tCoordinate> retCoordsInterpolated;

	int j = controlPoints.size() - 1;
	for (float t = 0.0f; t < 1.0f; t += 0.01f) {
		tCoordinate xyoft = aitkensAlgoritm(0, j, t, controlPoints, knotVector);
		retCoordsInterpolated.push_back(xyoft);
	}

	return retCoordsInterpolated;


}

// Do one iteration for Aitken's algorithm.

tCoordinate aitkinsAlgorithm::aitkensAlgoritm(int i, int j, float t, const std::vector<tCoordinate> controlPoints, vector<float> knotVector) {

	if (j == 0) {
		return controlPoints[i];
	}

	tCoordinate xyjminus1ioft = aitkensAlgoritm(i, j - 1, t, controlPoints, knotVector);
	tCoordinate xyjminus1iplus1oft = aitkensAlgoritm(i + 1, j - 1, t, controlPoints, knotVector);

	tCoordinate numerator = (knotVector[i + j] - t) * xyjminus1ioft + (t - knotVector[i]) * xyjminus1iplus1oft;
	float denominator = knotVector[i + j] - knotVector[i];

	return numerator / denominator;

}

/*
vector<tCoordinate>  interpolation() {

	// Pairs of cooridnate x and y.
	float cp_y[] = { 100, 250, 450, 600, 900 };
	float cp_x[] = { 50, 100, 150, 200, 250 };
	float knotVector[] = { 0, 0.25, 0.5, 0.75, 1 };



	vector<tCoordinate> retCoordsInterpolated;

	int j = (sizeof(cp_y) / sizeof(float))-1;
	for (float t = 0.0f; t < 1.0f; t += 0.01f) {
		float xoft = aitkensAlgoritm(1, j, t, cp_x, knotVector);
		float yoft = aitkensAlgoritm(1, j, t, cp_y, knotVector);

		tCoordinate pair = { xoft, yoft };
		retCoordsInterpolated.push_back(pair);
	}

	return retCoordsInterpolated;


}

// Do one iteration for Aitken's algorithm.

float aitkensAlgoritm( int i, int j, float t, float cp[], float knotVector[] ) {

	if (j == 0) {
		return cp[i];
	}

	float yjminus1ioft = aitkensAlgoritm(i, j - 1, t, cp, knotVector);
	float yjminus1iplus1oft = aitkensAlgoritm(i + 1, j - 1, t, cp, knotVector);

	float numerator = (knotVector[i + j] - t) * yjminus1ioft + (t - knotVector[i]) * yjminus1iplus1oft;
	float denominator = knotVector[i + j] - knotVector[i];

	return numerator / denominator;

}
*/

void aitkinsAlgorithm::drawInterpolated(HWND hWnd, HDC hdc) {

	if (g_controlPoints.size() < 2) {
		return;
	}

	g_coordinatesInterpolated = interpolation(g_controlPoints);
	POINT pt;

	MoveToEx(hdc, g_coordinatesInterpolated[0].x, g_coordinatesInterpolated[0].y, &pt);

	for (tCoordinate coordinate : g_coordinatesInterpolated) {

		LineTo(hdc, coordinate.x, coordinate.y);
		MoveToEx(hdc, coordinate.x, coordinate.y, &pt);

	}
}

void aitkinsAlgorithm::drawControlPoint(HWND hWnd, HDC hdc) {

	for (tCoordinate coord : g_controlPoints) {
		drawCrossedHair(hWnd, hdc, coord.x, coord.y);
	}
}

void aitkinsAlgorithm::drawNameOfAlgorithm( HWND hWnd, HDC hdc ) {

	const LPCWSTR name{ _T("Aitkin's Algorithm") };

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


void aitkinsAlgorithm::aitkinsalgorithm_draw(HWND hWnd, HDC hdc) {

	HPEN hRedPen;
	HPEN hBlackPen;
	hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	hBlackPen = CreatePen(PS_SOLID, 1, RGB(1, 1, 1));

	SelectObject(hdc, hRedPen);

	// display name of the algorithm.
	drawNameOfAlgorithm(hWnd, hdc);

	// Interpolate control points.
	drawInterpolated(hWnd, hdc);


	// Draw control points.

	SelectObject(hdc, hBlackPen);

	drawControlPoint(hWnd, hdc);


	DeleteObject(hRedPen);
	DeleteObject(hBlackPen);
}
void aitkinsAlgorithm::aitkinsalgorithm_addControlPoint(const POINT& point) {

	tCoordinate coord = { point.x, point.y };
	g_controlPoints.push_back(coord);

}
void aitkinsAlgorithm::aitkinsalgorithm_initialize_work() {
}
