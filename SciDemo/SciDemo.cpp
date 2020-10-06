// SciDemo.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SciDemo.h"
#include "SciCall.h"
#include "SciLexer.h"

#define MAX_LOADSTRING 100
#define WINDOW_SIZE     450
#define IDC_EDIT        0xFB03

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HANDLE g_hScintilla = NULL;
int iDefaultCodePage = 0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
HWND                EditCreate(HWND hwndParent);
void                InitDemoSettings(HWND hwnd);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SCIDEMO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCIDEMO));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SCIDEMO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SCIDEMO);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   Scintilla_RegisterClasses(hInstance);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
      CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_SIZE, WINDOW_SIZE, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   HWND hwndEdit = EditCreate(hWnd);
   InitScintillaHandle(hwndEdit);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

HWND EditCreate(HWND hwndParent)
{
   const HWND hwnd = CreateWindowEx(
      WS_EX_CLIENTEDGE,
      L"Scintilla",
      NULL,
      WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
      0, 0, 0, 0,
      hwndParent,
      (HMENU)IDC_EDIT,
      hInst,
      NULL);

   SendMessage(hwnd, SCI_SETCODEPAGE, iDefaultCodePage, 0);
   SendMessage(hwnd, SCI_SETEOLMODE, SC_EOL_CRLF, 0);
   SendMessage(hwnd, SCI_SETPASTECONVERTENDINGS, 1, 0);
   SendMessage(hwnd, SCI_USEPOPUP, FALSE, 0);
   SendMessage(hwnd, SCI_SETSCROLLWIDTH, 2048, 0);
   SendMessage(hwnd, SCI_SETSCROLLWIDTHTRACKING, TRUE, 0);
   SendMessage(hwnd, SCI_SETENDATLASTLINE, TRUE, 0);
   SendMessage(hwnd, SCI_SETCARETSTICKY, FALSE, 0);
   SendMessage(hwnd, SCI_SETXCARETPOLICY, CARET_SLOP | CARET_EVEN, 50);
   SendMessage(hwnd, SCI_SETYCARETPOLICY, CARET_EVEN, 0);
   SendMessage(hwnd, SCI_SETMULTIPLESELECTION, FALSE, 0);
   SendMessage(hwnd, SCI_SETADDITIONALSELECTIONTYPING, FALSE, 0);
   SendMessage(hwnd, SCI_SETVIRTUALSPACEOPTIONS, SCVS_NONE, 0);
   SendMessage(hwnd, SCI_SETADDITIONALCARETSBLINK, FALSE, 0);
   SendMessage(hwnd, SCI_SETADDITIONALCARETSVISIBLE, FALSE, 0);

   InitDemoSettings(hwnd);

   SetFocus(hwnd);

   return (hwnd);
}

void InitDemoSettings(HWND hwnd)
{
   SendMessage(hwnd, SCI_SETCODEPAGE, CP_UTF8, 0);
   SendMessage(hwnd, SCI_SETWRAPMODE, SC_WRAP_WORD, 0);
   const int iStyle = STYLE_DEFAULT;
   SendMessage(hwnd, SCI_STYLESETFONT, iStyle, (LPARAM)"Segoe UI");
   SendMessage(hwnd, SCI_STYLESETSIZE, iStyle, (LPARAM)16);
   SendMessage(hwnd, SCI_SETLEXER, SCLEX_NULL, 0);
   SendMessage(hwnd, SCI_SETTECHNOLOGY, SC_TECHNOLOGY_DEFAULT/*SC_TECHNOLOGY_DIRECTWRITE*/, 0);
   SendMessage(hwnd, SCI_SETFONTQUALITY, SC_EFF_QUALITY_LCD_OPTIMIZED, 0);
   
   // "日本zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzP TOPTOPTOPTOPTOPTOPZZZZZ IIIIzzzzzI!");
   SendMessage(hwnd, SCI_STYLECLEARALL, 0, 0);
   SendMessage(hwnd, SCI_REPLACESEL, 0, (LPARAM)"\xe6\x97\xa5\xe6\x9c\xac\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x50\x20\x54\x4f\x50\x54\x4f\x50\x54\x4f\x50\x54\x4f\x50\x54\x4f\x50\x54\x4f\x50\x5a\x5a\x5a\x5a\x5a\x20\x49\x49\x49\x49\x7a\x7a\x7a\x7a\x7a\x49\x21");
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_SIZE:
       {
         const auto res = DefWindowProc(hWnd, message, wParam, lParam);
         RECT rc = { 0 };
         GetClientRect(hWnd, &rc);
         SetWindowPos(GetDlgItem(hWnd, IDC_EDIT), NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOOWNERZORDER);
         return res;
       }
       break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
