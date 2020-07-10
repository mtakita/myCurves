#pragma once

typedef struct HermiteCurveControlPoint {
	POINT point;
	POINT velocity;
} tHermiteCurveControlPoint;

enum class ECState {
    idle,
    choosing_stating_point,
    choosing_ending_point,
};

namespace hermite_curve{

    void drawNameOfAlgorithm(HWND hWnd, HDC hdc);
    void hermitecurve_drawControlPoint(HWND hWnd, HDC hdc);
    void hermitecurve_drawHermiteCurve(HWND hWnd, HDC hdc);
    void hermitecurve_draw(HWND hWnd, HDC hdc);
    void hermitecurve_add_control_point(const POINT& point);
    void hermitecurve_choose_point_end(const POINT& point);
    void hermitecurve_update_velocity(const POINT& point);
    void hermitecurve_initialize_work();

}


