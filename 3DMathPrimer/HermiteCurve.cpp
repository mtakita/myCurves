
#define _USE_MATH_DEFINES
#include <math.h>
#include "mymath.h"

using namespace mymath;

#include <queue>
#include "framework.h"
#include "HermiteCurve.h"
#include "funccomm.h"

// work area for hermite curve.
std::queue<tHermiteCurveControlPoint> g_hermitecurve_controlPoints;



ECState g_state;

void hermitecurve_drawStartingControlPoint(HWND hWnd, HDC hdc) {

    if (g_hermitecurve_controlPoints.size() == 0) {
        return;
    }

    const tHermiteCurveControlPoint& controlPointPoint1st = g_hermitecurve_controlPoints.back();
    const tHermiteCurveControlPoint& controlPointPoint2nd = g_hermitecurve_controlPoints.front();

    const POINT& point1st = controlPointPoint1st.point;
    const POINT& point2nd = controlPointPoint2nd.point;

    drawCrossedHair(hWnd, hdc, point1st.x, point1st.y);
    drawCrossedHair(hWnd, hdc, point2nd.x, point2nd.y);

}

void hermitecurve_drawEndingControlPoint(HWND hWnd, HDC hdc) {

    if (g_hermitecurve_controlPoints.size() == 0) {
        return;
    }

    const tHermiteCurveControlPoint& controlPointPoint1st = g_hermitecurve_controlPoints.back();
    const tHermiteCurveControlPoint& controlPointPoint2nd = g_hermitecurve_controlPoints.front();

    const POINT& point1st = controlPointPoint1st.point;
    const POINT& velocity1st = controlPointPoint1st.velocity;

    const POINT& point2nd = controlPointPoint2nd.point;
    const POINT& velocity2nd = controlPointPoint2nd.velocity;

    drawCrossedHair(hWnd, hdc, point1st.x + velocity1st.x, point1st.y + velocity1st.y);
    drawCrossedHair(hWnd, hdc, point2nd.x + velocity2nd.x, point2nd.y + velocity2nd.y);

}

void hermite_curve::hermitecurve_drawHermiteCurve(HWND hWnd, HDC hdc) {

    if (g_hermitecurve_controlPoints.size() != 2) {
        return;
    }

    const tHermiteCurveControlPoint& controlPointPoint2nd = g_hermitecurve_controlPoints.back();
    const tHermiteCurveControlPoint& controlPointPoint1st = g_hermitecurve_controlPoints.front();

    const POINT& point1st = controlPointPoint1st.point;
    const POINT& velocity1st = controlPointPoint1st.velocity;

    const POINT& point2nd = controlPointPoint2nd.point;
    const POINT& velocity2nd = controlPointPoint2nd.velocity;

    Vectorf2 p0{ (float)point1st.x, (float)point1st.y };
    Vectorf2 v0{ (float)velocity1st.x, (float)velocity1st.y };
    Vectorf2 p1{ (float)point2nd.x, (float)point2nd.y };
    Vectorf2 v1{ (float)velocity2nd.x,(float)velocity2nd.y };

    Matrixf2x4 p{ p0, v0, v1, p1 };

    std::vector<Vectorf2> drawingPoints;

    float t = 0.0f;
    for (int i = 0; i < 100; i++, t = t + 0.01f) {
        float h0 = 1.0f - 3.0f * powf(t, 2.0f) + 2.0f * powf(t, 3.0f);
        float h1 = t - 2.0f * powf(t, 2.0f) + powf(t, 3.0f);
        float h2 = -powf(t, 2.0f) + powf(t, 3.0f);
        float h3 = 3.0f * powf(t, 2.0f) - 2.0f * powf(t, 3.0f);

        Vectorf4 vecH{ h0, h1, h2, h3 };

        Vectorf2 drawingPoint = p * vecH;
        drawingPoints.push_back(drawingPoint);
   }

    //
    // draw last control point.
    //

    t = 1.0f;
    float h0 = 1.0f - 3.0f * powf(t, 2.0f) + 2.0f * powf(t, 3.0f);
    float h1 = t - 2.0f * powf(t, 2.0f) + powf(t, 3.0f);
    float h2 = -powf(t, 2.0f) + powf(t, 3.0f);
    float h3 = 3.0f * powf(t, 2.0f) - 2.0f * powf(t, 3.0f);

    Vectorf4 vecH{ h0, h1, h2, h3 };

    Vectorf2 drawingPoint = p * vecH;
    drawingPoints.push_back(drawingPoint);


    if (drawingPoints.size() < 2) {
        return;
    }

    POINT pt;

    MoveToEx(hdc, drawingPoints[0][0], drawingPoints[0][1], &pt);

    std::vector<Vectorf2>::iterator itor = drawingPoints.begin();
    Vectorf2 point = *itor++;
    MoveToEx(hdc, point[0], point[1], &pt);

    while (itor != drawingPoints.end()) {

        Vectorf2 point = *itor;

        LineTo(hdc, point[0], point[1]);
        MoveToEx(hdc, point[0], point[1], &pt);

        itor++;
    }
}

void hermite_curve::drawNameOfAlgorithm(HWND hWnd, HDC hdc) {

    const LPCWSTR name{ _T("Hermite Curve") };

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
void hermite_curve::hermitecurve_draw(HWND hWnd, HDC hdc) {

    HPEN hRedPen;
    HPEN hBlackPen;
    hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    hBlackPen = CreatePen(PS_SOLID, 1, RGB(1, 1, 1));

    SelectObject(hdc, hRedPen);

    hermite_curve::drawNameOfAlgorithm(hWnd, hdc);

    // Interpolate control points.
    hermite_curve::hermitecurve_drawHermiteCurve(hWnd, hdc);


    // Draw control points.

    // Starting point.
    SelectObject(hdc, hBlackPen);
    hermitecurve_drawStartingControlPoint(hWnd, hdc);

    // Ending point.
    SelectObject(hdc, hRedPen);
    hermitecurve_drawEndingControlPoint(hWnd, hdc);

    DeleteObject(hRedPen);
    DeleteObject(hBlackPen);
}

void hermite_curve::hermitecurve_add_control_point(const POINT& point) {
    g_state = ECState::choosing_ending_point;

    tHermiteCurveControlPoint controlPoint;
    controlPoint.point = point;

    POINT velocity{ 0, 0 };
    controlPoint.velocity = velocity;

    g_hermitecurve_controlPoints.push(controlPoint);

    if (g_hermitecurve_controlPoints.size() > 2) {
        g_hermitecurve_controlPoints.pop();
    }
}
void hermite_curve::hermitecurve_choose_point_end(const POINT& point) {
    g_state = ECState::idle;

}

void hermite_curve::hermitecurve_update_velocity(const POINT& point) {

    tHermiteCurveControlPoint& controlPoint = g_hermitecurve_controlPoints.back();
    POINT startPoint = controlPoint.point;

    POINT velocity{ point.x - startPoint.x, point.y - startPoint.y };
    controlPoint.velocity = velocity;
}

void hermite_curve::hermitecurve_initialize_work() {
}
