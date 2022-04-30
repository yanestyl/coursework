#pragma once

#define OnReloadSoftware	1
#define OnSaveFile			2
#define OnExitSoftware		3

#define OnAboutIt			4

#define OnDelet				5


#define TextBufferSize		1024

char Buffer[TextBufferSize];
int CharsRead;



HWND hListBox;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

void MainWndAddMenues(HWND hWnd);
void MainWndAddWidgets(HWND hWnd);
void MainWndStart0(HWND hWnd);
void MainWndStart(HWND hWnd);
void MainWndStart1(HWND hWnd);
void MainWndStart2(HWND hWnd);
void MainWndStart3(HWND hWnd);
void SaveDate(LPCSTR path);
void DeleteApp();
