
#include "framework.h"
#include "Funccomm.h"



void drawCrossedHair(HWND hWnd, HDC hdc, float x, float y) {

    POINT pt;

    MoveToEx(hdc, x - LINE_LENGTH, y, &pt);
    LineTo(hdc, x + LINE_LENGTH, y);

    MoveToEx(hdc, x, y - LINE_LENGTH, &pt);
    LineTo(hdc, x, y + LINE_LENGTH);



}

