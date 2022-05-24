#pragma once

#define OnReloadSoftware	1
#define OnSaveFile			2
#define OnExitSoftware		3

#define OnAboutIt			4

#define OnDelete				5


#define TextBufferSize		1024

char Buffer[TextBufferSize];
int CharsRead;

char ProgramName[256];
bool uninstall = false;

HWND hListBox;
HWND mesbox;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

void MainWndAddMenues(HWND hWnd);
void MainWndAddWidgets(HWND hWnd);
void MainWndStart(HWND hWnd, bool uninstall, char ProgramName[]);
void MainWndStart1(HWND hWnd, bool uninstall, char ProgramName[]);
void MainWndStart2(HWND hWnd, bool uninstall, char ProgramName[]);
void MainWndStart3(HWND hWnd, bool uninstall, char ProgramName[]);
void SaveDate(LPCSTR path);
void DeleteApp(HWND hWnd);
void AboutIt(HWND hWnd);
