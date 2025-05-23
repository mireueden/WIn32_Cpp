#include "HelioUnreal.h"



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

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    LoadApp(hWnd, loadGame, freeGame, drawGame, KeyGame);
    void test();
    test();

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
            static DWORD prev = GetTickCount();
            DWORD now = GetTickCount();
            float delta = (now - prev) / 1000.0f;
            prev = now;

            drawApp(delta);
        }
    }

    freeApp();

    return (int)msg.wParam;
}

#include <stdio.h>

void check(int key);

bool mouseMoving = false;

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
        switch (wParam) {
        case 87:// w
            keydown |= keydown_w;
            break;
        case 65:// a
            keydown |= keydown_a;
            break;
        case 83:// s
            keydown |= keydown_s;
            break;
        case 68:// d
            keydown |= keydown_d;
            break;
        case 32:// space
            keydown |= keydown_space;
            break;
        }
        check(keydown);
        break;
    case WM_KEYUP:
        printf("WM_KEYUP %d\n", wParam);
        switch (wParam) {
        case 87:// w
            keydown &= ~keydown_w;
            break;
        case 65:// a
            keydown &= ~keydown_a;
            break;
        case 83:// s
            keydown &= ~keydown_s;
            break;
        case 68:// d
            keydown &= ~keydown_d;
            break;
        case 32:// space
            keydown &= ~keydown_space;
            break;
        }
        check(keydown);
        break;

    case WM_LBUTTONDOWN:
        printf("WM_LBUTTONDOWN %d (%d, %d)\n", wParam, LOWORD(lParam), HIWORD(lParam));
        keyApp(iKeyStatBegan, iPointMake(LOWORD(lParam), HIWORD(lParam)));
        mouseMoving = true;
        break;
    case WM_LBUTTONUP:
        printf("WM_LBUTTONDOWN %d (%d, %d)\n", wParam, LOWORD(lParam), HIWORD(lParam));
        keyApp(iKeyStatEnded, iPointMake(LOWORD(lParam), HIWORD(lParam)));
        mouseMoving = false;
        break;
    case WM_MOUSEMOVE:
        //if (mouseMoving)
            keyApp(iKeyStatMoved, iPointMake(LOWORD(lParam), HIWORD(lParam)));
        break;

    case WM_MOVE:
        printf("WM_MOVE\n");
        break;
    case WM_SIZE:
        printf("WM_SIZE\n");
        break;
    case WM_SIZING:
        printf("WM_SIZING\n");
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

#include <stdio.h>
#include "iPoint.h"
#include "iColor.h"
#include "iString.h"

void check(int key)
{
    if (key & keydown_w)
    {
        printf("w ");
    }
    if (key & keydown_a)
    {
        printf("a ");
    }
    printf("\n");
}


void test()
{
#if 0
    iString s, s2;
    s = "Hi";
    s = s2;
    const char* s1 = "World";
    if (s == s1)
        ;
    if (s == s2)
        ;
    const char* s = s0 + "Earth";
#endif
}

