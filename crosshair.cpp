/**
 * Crosshair - A crosshair for Windows
 *
 * This program creates a simple crosshair in that floats above other windows.
 *
 * Copyright 2019 InsaneJetman
 */

#define WINVER 0x0500
#include <windows.h>

const char g_szClassName[] = "crosshairWindowClass";
HBITMAP cross;

void ShowImage(HWND hwnd, HBITMAP hbmp, POINT *pOrigin);

// The Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        case WM_PAINT:
            result = DefWindowProc(hwnd, msg, wParam, lParam);
            ShowImage(hwnd, cross, 0);
        break;
        case WM_NCHITTEST:
            result = DefWindowProc(hwnd, msg, wParam, lParam);
            if (result == HTCLIENT) result = HTCAPTION;
        break;
        default:
            result = DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return result;
}

void ShowCenteredImage(HWND hwnd, HBITMAP hbmp) {
    // get the size of the bitmap
    BITMAP bm; GetObject(hbmp, sizeof(bm), &bm);
    SIZE imgSize = {bm.bmWidth, bm.bmHeight};

    // get the primary monitor's info
    POINT ptZero = { 0 };
    HMONITOR hmonPrimary = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO monitorinfo = { 0 };
    monitorinfo.cbSize = sizeof(monitorinfo);
    GetMonitorInfo(hmonPrimary, &monitorinfo);

    // center the splash screen in the middle of the primary work area
    const RECT & rcWork = monitorinfo.rcWork;
    POINT origin;
    origin.x = rcWork.left + (rcWork.right - rcWork.left - imgSize.cx) / 2;
    origin.y = rcWork.top + (rcWork.bottom - rcWork.top - imgSize.cy) / 2;

    ShowImage(hwnd, hbmp, &origin);
}

// Calls UpdateLayeredWindow to set a bitmap (with alpha) as the content of the splash window.
void ShowImage(HWND hwnd, HBITMAP hbmp, POINT *pOrigin)
{
    // get the size of the bitmap
    BITMAP bm; GetObject(hbmp, sizeof(bm), &bm);
    SIZE imgSize = {bm.bmWidth, bm.bmHeight};
    POINT ptZero = { 0 };

    // create a memory DC holding the splash bitmap
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmpOld = (HBITMAP) SelectObject(hdcMem, hbmp);

    // use the source image's alpha channel for blending
    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;
    blend.AlphaFormat = AC_SRC_ALPHA;

    // paint the window (in the right location) with the alpha-blended bitmap
    UpdateLayeredWindow(hwnd, hdcScreen, pOrigin, &imgSize,
        hdcMem, &ptZero, RGB(0, 0, 0), &blend, ULW_ALPHA);

    // delete temporary objects
    SelectObject(hdcMem, hbmpOld);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    // hMyIcon = LoadIcon(hInstance, "crosshair");
    HICON cross16 = static_cast<HICON>(LoadImage(hInstance, "crosshair", IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
    HICON cross32 = static_cast<HICON>(LoadImage(hInstance, "crosshair", IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));
    HICON cross64 = static_cast<HICON>(LoadImage(hInstance, "crosshair", IMAGE_ICON, 64, 64, LR_DEFAULTCOLOR));
    ICONINFO iconinfo; GetIconInfo(cross64, &iconinfo);
    cross = iconinfo.hbmColor;
    // NOTE: the 64x64 icon has already been pre-multiplied for transparency

    // register the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = cross32;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = cross16;

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create the Window
    hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST,
        g_szClassName, "Crosshair",
        WS_POPUP | WS_VISIBLE | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 128, 128,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    ShowCenteredImage(hwnd, cross);

    // The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
