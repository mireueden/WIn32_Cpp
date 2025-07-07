#include "Tool.h"

#define MAX_LOADSTRING 100

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
bool run;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    WCHAR szTitle[20] = L"Tool";
    WCHAR szWindowClass[20] = L"Tool";
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOL));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TOOL);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TOOL));

    run = true;
    loadApp(hWnd);

    MSG msg;
    while (run)
    {
        if (PeekMessage(&msg,hWnd, 0,0,PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            drawApp(0.0f);
        }
     
    }

    freeApp();

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DROPFILES:
    {
        HDROP hDrop = (HDROP)wParam;

        wchar_t wPath[512];
        DragQueryFile(hDrop, 0, wPath, 512);

        char* path = utf16_to_utf8(wPath);
        printf("path = %s\n", path);
        delete path;
        break;
    }

    case WM_COMMAND:
        {
        //wParam, lParam;
        updateCtrl(wParam);

            // 메뉴 선택을 구문 분석합니다:
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(NULL, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_CLOSE:
        if (MessageBox(NULL, L"할까요?", L"종료?", MB_YESNO) == IDYES)
        {
            run = false;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    //default:
    //    return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


HWND* hBtn;
HWND hList;
HWND* hEdit;
HWND hEditMulti;

iArray* array;

void loadApp(HWND hwnd)
{
    hWnd = hwnd;
    DragAcceptFiles(hwnd, true);

    loadCtrl();

    const char* strBtn[5] = { "로드", "저장", "추가", "업뎃", "삭제" };
    hBtn = new HWND[5];
    for (int i = 0; i < 5; i++)
        hBtn[i] = createWndButton(10 + 55* i, 10, 50, 30, methodBtn, strBtn[i]);

    const char* strList[1] = { "End of List", };
    hList = createWndListBox(10, 45, 150, 100, methodList, strList, 1);

    const char* strInfo[5] = { "이름", "HP", "MP", "ATK", "DEF" };
    hEdit = new HWND[5];
    for (int i = 0; i < 5; i++)
    {
        createWndStatic(170, 45 + 30 * i, 50, 25, NULL, strInfo[i]);
        hEdit[i] = createWndEdit(220, 45 + 30 * i, 50, 25,
            i==0 ? WndEditAll : WndEditInt, methodEdit, "0");
    }

    hEditMulti = createWndEditfMultiline(10, 200, 270, 50,
        methodEdit, "설명");

    array = new iArray(methodArray);

}

void freeApp()
{
    freeCtrl();

    delete hBtn;
    delete hEdit;

    delete array;
}
void drawApp(float dt)
{

}

void methodBtn(HWND hwnd)
{
    int i;
    for (i = 0; i < 5; i++)
    {
        if (hBtn[i] == hwnd)
            break;
    }

    const char* strBtn[5] = { "로드", "저장", "추가", "업뎃", "삭제" };
    printf("btn = %s\n", strBtn[i]);
    if (i == 0)
    {
        const char* path = openFileDlg(true, ITM_FILTER);
        if (path)
        {
            FILE* pf = fopen(path, "rb");

            int num;
            fread(&num, sizeof(int), 1, pf);
            for (int i = 0; i < num; i++)
            {
                Item* item = new Item;
                int len;
                fread(&len, sizeof(int), 1, pf);
                char* s = new char[len + 1];
                fread(s, sizeof(char), len, pf);
                s[len] = 0;
                item->name = s;
                fread(&item->hp, sizeof(int), 1, pf);
                fread(&item->mp, sizeof(int), 1, pf);
                fread(&item->atk, sizeof(int), 1, pf);
                fread(&item->def, sizeof(int), 1, pf);
                fread(&len, sizeof(int), 1, pf);
                s = new char[len + 1];
                fread(s, sizeof(char), len, pf);
                s[len] = 0;
                item->exp = s;
                array->add(item);

                addWndListBox(hList, i, s);
            }

            fclose(pf);
        }
    }
    else if (i == 1)
    {
        const char* path = openFileDlg(false, ITM_FILTER);
        if (path)
        {
            FILE* pf = fopen(path, "wb");

            fwrite(&array->count, sizeof(int), 1, pf);
            for (int i = 0; i < array->count; i++)
            {
                Item* item = (Item*)array->at(i);
                int len = strlen(item->name);
                fwrite(&len, sizeof(int), 1, pf);
                fwrite(item->name, sizeof(char), len, pf);
                fwrite(&item->hp, sizeof(int), 1, pf);
                fwrite(&item->mp, sizeof(int), 1, pf);
                fwrite(&item->atk, sizeof(int), 1, pf);
                fwrite(&item->def, sizeof(int), 1, pf);
                len = strlen(item->exp);
                fwrite(&len, sizeof(int), 1, pf);
                fwrite(item->exp, sizeof(char), len, pf);
            }

            fclose(pf);
        }
    }
    else if (i == 2)
    {
        char* name = getWndText(hEdit[0]);
        int index = indexWndListBox(hList);
        addWndListBox(hList, index, name);
        setWndListBox(hList, index);
        
        Item* item = new Item;
        item->name = name;
        item->hp = getWndInt(hEdit[1]);
        item->mp = getWndInt(hEdit[2]);
        item->atk = getWndInt(hEdit[3]);
        item->def = getWndInt(hEdit[4]);
        item->exp = getWndText(hEditMulti);
        array->add(index, item);
    }
    else if (i == 3)
    {
        int index = indexWndListBox(hList);
        Item* item = (Item*)array->at(index);
        delete item->name;
        item->name = getWndText(hEdit[0]);
        item->hp = getWndInt(hEdit[1]);
        item->mp = getWndInt(hEdit[2]);
        item->atk = getWndInt(hEdit[3]);
        item->def = getWndInt(hEdit[4]);
        delete item->exp;
        item->exp = getWndText(hEditMulti);
    }
    else if (i == 4)
    {

    }
}

void methodList(HWND hwnd)
{
    int index = indexWndListBox(hList);
    int count = countWndListBox(hList);
    if (index == count - 1)
        return;

    Item* item = (Item*)array->at(index);
    setWndText(hEdit[0], item->name);
    setWndText(hEdit[1], "%d", item->hp);
    setWndText(hEdit[2], "%d", item->mp);
    setWndText(hEdit[3], "%d", item->atk);
    setWndText(hEdit[4], "%d", item->def);
    setWndText(hEditMulti, item->exp);
}

void methodEdit(HWND hwnd)
{

}
void methodEditMulti(HWND hwnd)
{

}

void methodArray(void* data)
{
    Item* i = (Item*)data;
    delete i->name;
    delete i;
}