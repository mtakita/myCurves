#pragma once

#ifndef _FUNCCOMM_
#define _FUNCCOMM_

typedef struct Coordinate {
	float x;
	float y;

	struct Coordinate operator / (float& divisor) {
		x = x / divisor;
		y = y / divisor;

		return *this;
	};
	struct Coordinate operator + (const struct Coordinate& coord) {
		x = x + coord.x;
		y = y + coord.y;

		return *this;
	};
} tCoordinate;

static tCoordinate operator * (float knot, tCoordinate& coord) {
	tCoordinate ret = { knot * coord.x, knot * coord.y };
	return ret;
};



enum class ECTypeLine {
    aitkins_algoritm,
    hermite_curve,
	bezier_curve_de_castelju_recurrence_relation,
	bezier_curve_arbitrary_degree,
};

#define LINE_LENGTH 10

void drawCrossedHair(HWND hWnd, HDC hdc, float x, float y);

#endif
