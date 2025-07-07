#include "iWindow.h"

wchar_t* utf8_to_utf16(const char* szFormat, ...)
{
    char szText[512];
    va_start_end(szFormat, szText);

    int len = MultiByteToWideChar(CP_UTF8, 0, szText, -1, NULL, 0);
    wchar_t* wStr = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, szText, sizeof(szText), wStr, len);

    return wStr;
}

char* utf16_to_utf8(const wchar_t* wStr)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, wStr, lstrlen(wStr), NULL, 0, 0, NULL);
    char* str = new char[len + 1];
    WideCharToMultiByte(CP_UTF8, 0, wStr, lstrlen(wStr), str, len, 0, NULL);
    str[len] = 0;

    return str;
}

HWND hWnd;

#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

WndCtrl* wndCtrl;
int wndCtrlNum;

void loadCtrl()
{
    InitCommonControls();

    wndCtrl = new WndCtrl[wndCtrlMax];
    wndCtrlNum = 0;
}

void freeCtrl()
{
    delete wndCtrl;
}

void updateCtrl(WPARAM wParam)
{
    int id = LOWORD(wParam);

    for (int i = 0; i < wndCtrlNum; i++)
    {
        WndCtrl* wc = &wndCtrl[i];
        if (wc->id == id)
        {
            if (wc->style == WndButton ||
                wc->style == WndRadio)
            {
                if (wc->m)
                    wc->m(wc->hwnd);
            }
            else if (wc->style == WndCheckBox)
            {
                bool on = getCheckBox(wc->hwnd);
                setCheckBox(wc->hwnd, !on);
                if (wc->m)
                    wc->m(wc->hwnd);
            }
            else if (wc->style == WndComboBox)
            {
                if (HIWORD(wParam) == CBN_SELCHANGE)
                {
                    printf("O\n");
                    if (wc->m)
                        wc->m(wc->hwnd);
                }
                else
                {
                    printf("X\n");
                }
            }
            else if (wc->style == WndListBox)
            {
                switch (HIWORD(wParam)) {
                case LBN_SELCHANGE:
                    if (wc->m)
                        wc->m(wc->hwnd);
                    break;
                case LBN_DBLCLK:
                    printf("DB CLICK\n");
                    break;
                }
            }
            break;
        }
    }
}

HWND createWndStatic(
    int x, int y, int width, int height,
    MethodHwnd m, const char* szFormat, ...)
{
    char szText[512];
    va_start_end(szFormat, szText);

    wchar_t* ws = utf8_to_utf16(szText);
    HWND hwnd = CreateWindow(WC_STATIC, ws,
        WS_CHILD | WS_VISIBLE | ES_CENTER,
        x, y, width, height, hWnd, (HMENU)wndCtrlNum, NULL, NULL);
    delete ws;

    WndCtrl* wc = &wndCtrl[wndCtrlNum];
    wc->style = WndStatic;
    wc->id = wndCtrlNum;
    wc->hwnd = hwnd;
    wc->m = m;
    wndCtrlNum++;

    return hwnd;
}

HWND createWndButton(
    int x, int y, int width, int height,
    MethodHwnd m, const char* szFormat, ...)
{
    char szText[512];
    va_start_end(szFormat, szText);

    wchar_t* ws = utf8_to_utf16(szText);
    HWND hwnd = CreateWindow(WC_BUTTON, ws,
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        x, y, width, height, hWnd, (HMENU)wndCtrlNum, NULL, NULL);
    delete ws;

    WndCtrl* wc = &wndCtrl[wndCtrlNum];
    wc->style = WndButton;
    wc->id = wndCtrlNum;
    wc->hwnd = hwnd;
    wc->m = m;
    wndCtrlNum++;

    return hwnd;
}

HWND createWndCheckBox(
    int x, int y, int width, int height,
    MethodHwnd m, const char* szFormat, ...)
{
    char szText[512];
    va_start_end(szFormat, szText);

    wchar_t* ws = utf8_to_utf16(szText);
    HWND hwnd = CreateWindow(WC_BUTTON, ws,
        WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
        x, y, width, height, hWnd, (HMENU)wndCtrlNum, NULL, NULL);
    delete ws;

    WndCtrl* wc = &wndCtrl[wndCtrlNum];
    wc->style = WndCheckBox;
    wc->id = wndCtrlNum;
    wc->hwnd = hwnd;
    wc->m = m;
    wndCtrlNum++;

    return hwnd;
}
bool getCheckBox(HWND hwnd)
{
    return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}
void setCheckBox(HWND hwnd, bool on)
{
    SendMessage(hwnd, BM_SETCHECK, on ? BST_CHECKED : BST_UNCHECKED, 0);
}

HWND createWndComboBox(
    int x, int y, int width, int height,
    MethodHwnd m, const char** str, int strNum)
{
    HWND hwnd = CreateWindow(WC_COMBOBOX, NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN | CBS_HASSTRINGS,
        x, y, width, height, hWnd, (HMENU)wndCtrlNum, NULL, NULL);

    for (int i = 0; i < strNum; i++)
        addWndComboBox(hwnd, i, str[i]);
    setWndComboBox(hwnd, 0);

    WndCtrl* wc = &wndCtrl[wndCtrlNum];
    wc->style = WndComboBox;
    wc->id = wndCtrlNum;
    wc->hwnd = hwnd;
    wc->m = m;
    wndCtrlNum++;

    return hwnd;
}
void addWndComboBox(HWND hwnd, int index, const char* str)
{
    wchar_t* s = utf8_to_utf16(str);
    SendMessage(hwnd, CB_INSERTSTRING, (WPARAM)index, (LPARAM)s);
    delete s;
}

int indexWndComboBox(HWND hwnd)
{
    return SendMessage(hwnd, CB_GETCURSEL, 0, 0);
}
int countWndComboBox(HWND hwnd)
{
    return SendMessage(hwnd, CB_GETCOUNT, 0, 0);
}

char* getWndComboBox(HWND hwnd, int index)
{
    wchar_t ws[256];
    SendMessage(hwnd, CB_GETLBTEXT, index, (LPARAM)ws);
    return utf16_to_utf8(ws);
}
void setWndComboBox(HWND hwnd, int index)
{
    SendMessage(hwnd, CB_SETCURSEL, index, 0);
}

HWND createWndListBox(
    int x, int y, int width, int height,
    MethodHwnd m, const char** str, int strNum)
{
    HWND hwnd = CreateWindow(WC_LISTBOX, NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_HSCROLL | WS_VSCROLL,
        x, y, width, height, hWnd, (HMENU)wndCtrlNum, NULL, NULL);

    for (int i = 0; i < strNum; i++)
        addWndListBox(hwnd, i, str[i]);
    setWndListBox(hwnd, 0);

    WndCtrl* wc = &wndCtrl[wndCtrlNum];
    wc->style = WndListBox;
    wc->id = wndCtrlNum;
    wc->hwnd = hwnd;
    wc->m = m;
    wndCtrlNum++;

    return hwnd;
}

void addWndListBox(HWND hwnd, int index, const char* str)
{
    wchar_t* s = utf8_to_utf16(str);
    SendMessage(hwnd, LB_INSERTSTRING, (WPARAM)index, (LPARAM)s);
    delete s;
}
int indexWndListBox(HWND hwnd)
{
    return SendMessage(hwnd, LB_GETCURSEL, 0, 0);
}
int countWndListBox(HWND hwnd)
{
    return SendMessage(hwnd, LB_GETCOUNT, 0, 0);
}
char* getWndListBox(HWND hwnd, int index)
{
    wchar_t ws[256];
    SendMessage(hwnd, LB_GETTEXT, index, (LPARAM)ws);
    return utf16_to_utf8(ws);
}
void setWndListBox(HWND hwnd, int index)
{
    SendMessage(hwnd, LB_SETCURSEL, index, 0);
}

HWND createWndRadio(
    int x, int y, int width, int height,
    MethodHwnd m, const char* str)
{
    wchar_t* ws = utf8_to_utf16(str);
    HWND hwnd = CreateWindow(WC_BUTTON, ws,
        WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
        x, y, width, height, hWnd, (HMENU)wndCtrlNum, NULL, NULL);
    delete ws;

    WndCtrl* wc = &wndCtrl[wndCtrlNum];
    wc->style = WndRadio;
    wc->id = wndCtrlNum;
    wc->hwnd = hwnd;
    wc->m = m;
    wndCtrlNum++;

    return hwnd;
}
bool getWndRadio(HWND hwnd)
{
    return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}
void setWndRadio(HWND hwnd, bool on)
{
    SendMessage(hwnd, BM_SETCHECK, on ? BST_CHECKED : BST_UNCHECKED, 0);
}

LRESULT CALLBACK editAllWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK editIntWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK editFloatWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND createWndEdit(
    int x, int y, int width, int height,
    WndEditStyle s, MethodHwnd m, const char* str, int strLengthMax)
{
    wchar_t* ws = utf8_to_utf16(str);
    HWND hwnd = CreateWindow(WC_EDIT, ws,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
        x, y, width, height, hWnd, (HMENU)wndCtrlNum, NULL, NULL);
    delete ws;
    if (strLengthMax)
        SendMessage(hwnd, EM_LIMITTEXT, strLengthMax, 0);

    SetWindowLongPtr(hwnd, GWLP_USERDATA, GetWindowLongPtr(hwnd, GWLP_WNDPROC));
    typedef LRESULT(CALLBACK* METHOD_EDIT)(HWND, UINT, WPARAM, LPARAM);
    METHOD_EDIT method[3] = { editAllWndProc , editIntWndProc , editFloatWndProc };
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)method[s]);

    WndCtrl* wc = &wndCtrl[wndCtrlNum];
    wc->style = WndEdit;
    wc->id = wndCtrlNum;
    wc->hwnd = hwnd;
    wc->m = m;
    wndCtrlNum++;

    return hwnd;
}

LRESULT CALLBACK editWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, WndEditStyle style)
{
    WNDPROC wpOld = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (wpOld)
    {
        switch (message)
        {
        case WM_NCDESTROY:
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)wpOld);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
            break;

        case WM_CHAR:
            if (style == WndEditAll)
                ;// ok
            else if (style == WndEditInt)
            {
                if (wParam == VK_RETURN || wParam == VK_BACK ||
                    wParam == '-' || (wParam >= '0' && wParam <= '9'))
                    ;// ok
                else
                    return 0;
            }
            else// if (style == WndEditFloat)
            {
                if (wParam == '.')
                {
                    wchar_t str[1024];
                    int length = GetWindowTextLength(hWnd) + 1;
                    GetWindowTextW(hWnd, str, length);
                    for (int i = 0; i < length; i++)
                    {
                        if (str[i] == '.')
                            return 0;
                    }
                }
                else if (wParam == VK_RETURN || wParam == VK_BACK ||
                    wParam == '-' || (wParam >= '0' && wParam <= '9'))
                    ;// ok
                else
                    return 0;
            }
            break;

        default:
            break;
        }
    }

    return CallWindowProc(wpOld, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK editAllWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return editWndProc(hWnd, message, wParam, lParam, WndEditAll);
}
LRESULT CALLBACK editIntWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return editWndProc(hWnd, message, wParam, lParam, WndEditInt);
}
LRESULT CALLBACK editFloatWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return editWndProc(hWnd, message, wParam, lParam, WndEditFloat);
}

LRESULT CALLBACK editMultilineWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND createWndEditfMultiline(
    int x, int y, int width, int height,
    MethodHwnd m, const char* str, int strLengthMax)
{
    wchar_t* ws = utf8_to_utf16(str);
    HWND hwnd = CreateWindow(WC_EDIT, ws,
        WS_CHILD | WS_VISIBLE | WS_BORDER |
        ES_MULTILINE | ES_WANTRETURN | ES_NOHIDESEL | ES_AUTOHSCROLL | WS_VSCROLL,
        x, y, width, height, hWnd, (HMENU)wndCtrlNum, NULL, NULL);
    delete ws;
    if (strLengthMax)
        SendMessage(hwnd, EM_LIMITTEXT, strLengthMax, 0);

    SetWindowLongPtr(hwnd, GWLP_USERDATA, GetWindowLongPtr(hwnd, GWLP_WNDPROC));
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)editMultilineWndProc);

    WndCtrl* wc = &wndCtrl[wndCtrlNum];
    wc->style = WndEdit;
    wc->id = wndCtrlNum;
    wc->hwnd = hwnd;
    wc->m = m;
    wndCtrlNum++;

    return hwnd;
}

LRESULT CALLBACK editMultilineWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WNDPROC wpOld = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (wpOld)
    {
        switch (message)
        {
        case WM_NCDESTROY:
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)wpOld);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
            break;

        case WM_GETDLGCODE:
        {
            LRESULT ret = CallWindowProc(wpOld, hWnd, message, wParam, lParam);
            ret &= ~(DLGC_HASSETSEL | DLGC_WANTTAB);
            if (lParam &&
                ((LPMSG)lParam)->message == WM_KEYDOWN &&
                ((LPMSG)lParam)->wParam == VK_TAB)
                ret &= ~DLGC_WANTMESSAGE;
            return ret;//break;
        }
        }
    }

    return CallWindowProc(wpOld, hWnd, message, wParam, lParam);
}

// ---------------------------------------------------
// Common Function
// ---------------------------------------------------

void setWndText(HWND hwnd, const char* szFormat, ...)
{
    char szText[1024];
    va_start_end(szFormat, szText);

    wchar_t* ws = utf8_to_utf16(szText);
    SetWindowText(hwnd, ws);
    delete ws;
}

char* getWndText(HWND hwnd)
{
    wchar_t ws[1024];
    int length = GetWindowTextLengthA(hwnd) + 1;
    GetWindowText(hwnd, ws, length);
    return utf16_to_utf8(ws);
}

int getWndInt(HWND hwnd)
{
    char* s = getWndText(hwnd);
    int n = atoi(s);
    delete s;
    return n;
}
float getWndFloat(HWND hwnd)
{
    char* s = getWndText(hwnd);
    int n = atof(s);
    delete s;
    return n;
}

char* pathOpenFileDlg = NULL;
const char* openFileDlg(bool open, LPCWSTR filter)
{
    wchar_t ws[1024];

    OPENFILENAME ofn;
    memset(&ofn, 0x00, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrFile = ws;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    wchar_t wPath[FILENAME_MAX];
    GetCurrentDirectory(FILENAME_MAX, wPath); // back up

    bool success = false;
    if (open) success = GetOpenFileName(&ofn);
    else      success = GetSaveFileName(&ofn);

    SetCurrentDirectory(wPath); // restore
    
    if (success)
    {
        wsprintf(ws, TEXT("%s"), ofn.lpstrFile);
        if(pathOpenFileDlg)
            delete pathOpenFileDlg;
        pathOpenFileDlg = utf16_to_utf8(ws);
        return pathOpenFileDlg;
    }
    return NULL;
}
