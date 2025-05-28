#include "HelioUnreal.h"

#include "iStd.h"
#include "Game.h"

bool runApp;
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{

    WCHAR szTitle[32] = L"Hello Unreal";
    WCHAR szWindowClass[32] = L"HU";

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HELIOUNREAL));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_HELLOUNREAL);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    runApp = false;
    int x = 0, y = 0, w = 640, h = 480;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        0, 0, 1980, 1080, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    LoadApp(hWnd, loadGame, freeGame, drawGame, KeyGame);

    MoveWindow(hWnd, x, y, w + 1, h, true);

    MSG msg;
    runApp = true;
    while (runApp)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            drawApp(iFPS::share()->update());
            keydown = keydown_none;
        }
    }

    freeApp();

    return (int)msg.wParam;
}

#include <stdio.h>

bool mouseMoving = false;
void ctrlKey(bool pressed, int& keydown, int key)
{
    int keys[][2] = {
        {87, keydown_w}, {65, keydown_a}, {83, keydown_s}, {68, keydown_d},
        //{87, keydown_w}, {65, keydown_a}, {83, keydown_s}, {68, keydown_d},
        //{87, keydown_w}, {65, keydown_a}, {83, keydown_s}, {68, keydown_d},
        {32, keydown_space},
    };
    int nKey = sizeof(keys) / sizeof(int) / 2;

    if (pressed)
    {
        for (int i = 0; i < nKey; i++)
        {
            if (key == keys[i][0])
            {
                keydown |= keys[i][1];
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < nKey; i++)
        {
            if (key == keys[i][0])
            {
                keydown &= ~keys[i][1];
                break;
            }
        }
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        printf("WM_CREATE\n");
        break;

    case WM_CLOSE:
        printf("WM_CLOSE\n");
        if (MessageBox(NULL, L"종료할까요?", L"정말?", MB_YESNO) == IDYES)
        {
            printf("예\n");
            runApp = false;
        }
        else
        {
            printf("아니오\n");
            return 0;
        }
        break;

    case WM_KEYDOWN:
        printf("WM_KEYDOWN %d\n", wParam);
        ctrlKey(true, keystat, wParam);
        ctrlKey(true, keydown, wParam);
        break;
    case WM_KEYUP:
        printf("WM_KEYUP %d\n", wParam);
        ctrlKey(true, keystat, wParam);
        break;

    case WM_LBUTTONDOWN:
        printf("WM_LBUTTONDOWN %d (%d, %d)\n", wParam, LOWORD(lParam), HIWORD(lParam));
        keyApp(iKeyStatBegan, convertCoord(LOWORD(lParam), HIWORD(lParam)));
        mouseMoving = true;
        break;
    case WM_LBUTTONUP:
        printf("WM_LBUTTONDOWN %d (%d, %d)\n", wParam, LOWORD(lParam), HIWORD(lParam));
        keyApp(iKeyStatEnded, convertCoord(LOWORD(lParam), HIWORD(lParam)));
        mouseMoving = false;
        break;
    case WM_MOUSEMOVE:
        //if (mouseMoving)
            keyApp(iKeyStatMoved, convertCoord(LOWORD(lParam), HIWORD(lParam)));
        break;

    case WM_MOVE:
        printf("WM_MOVE\n");
        break;
    case WM_SIZING:
    {
        RECT* rt = reinterpret_cast<LPRECT>(lParam);
        //RECT* rt = (RECT*)lParam;

        printf("WM_SIZING(%d, %d)\n",
            rt->right - rt->left,
            rt->bottom - rt->top);
    }
        break;
    case WM_SIZE:
        printf("WM_SIZE(%d, %d)\n", LOWORD(lParam), HIWORD(lParam));
        resize(LOWORD(lParam), HIWORD(lParam));
        if (runApp)
            drawApp(iFPS::share()->update());
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        printf("WM_DESTROY\n");
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void resize(int width, int height)
{
    float& dw = devSize.width;
    float& dh = devSize.height;
    float& x = viewport.origin.x;
    float& y = viewport.origin.y;
    float& vw = viewport.size.width;
    float& vh = viewport.size.height;

    // viewport
    float r0 = width / dw;
    float r1 = height / dh;
    if (r0 < r1)
    {
        // width 기준
        x = 0;
        vw = width;

        // vw : vh = dw : dh
        // vh * dw = vw * dh
        vh = vw * dh / dw;
        y = (height - vh) / 2;

    }
    else // if(r0 >= r1)
    {
        // height 기준
        y = 0;
        vh = height;

        // vw : vh = dw : dh
        // vh * dw = vw * dh
        vw = dw * vh / dh;
        x = (width - vw) / 2;

    }

    // vWidth : vHeight = devSize.width : devSize.height
}

iPoint convertCoord(float x, float y)
{
    // x, y, in Window Client Area
    iPoint p;
    p.x = (x - viewport.origin.x) / viewport.size.width * devSize.width;
    p.y = (y - viewport.origin.y) / viewport.size.height * devSize.height;
    printf("(%.0f,%.0f) => (%.0f,%.0f)\n", x, y, p.x, p.y);
    return p;
}