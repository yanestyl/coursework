#include <Windows.h>
#include <string>
#include <Winuser.h>
#include "resource.h"
#include "SoftwareDefinitions.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)),
	L"MainWndClass", SoftwareMainProcedure);
	
	if (!RegisterClassW(&SoftwareMainClass)) { return -1; }

	MSG SoftwareMainMessage = { 0 };

	CreateWindow(L"MainWndClass", L"application destroyer", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 900, 550, NULL, NULL, NULL, NULL);
	while (GetMessage(&SoftwareMainMessage, NULL, NULL, NULL)) {
		TranslateMessage(&SoftwareMainMessage);
		DispatchMessage(&SoftwareMainMessage);
	}
	return 0;

}

WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {
	WNDCLASS NWC = { 0 };

	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}

// - - - - - - - - - - О Б Р А Б О Т Ч И К - С О Б Ы Т И Й - - - - - - - - - - //
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_CREATE:
		MainWndAddMenues(hWnd);
		MainWndAddWidgets(hWnd);
		MainWndStart0(hWnd);
		break;
	case WM_COMMAND:
		switch (wp) {
		case OnReloadSoftware:
			MainWndStart0(hWnd);
			break;
		case OnSaveFile:
			SaveDate("C:\\Users\\yaros\\source\\repos\\Project1\\output.txt");
			break;
		case OnExitSoftware:
			PostQuitMessage(0);
			break;
		case OnAboutIt:

			break;
		default: break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
}


// - - - - - - - - - - М Е Н Ю - - - - - - - - - - //
void MainWndAddMenues(HWND hWnd) {
	
	HMENU RootMenu = CreateMenu();

	HMENU SubMenu1 = CreateMenu();
	HMENU SubMenu2 = CreateMenu();

	AppendMenu(SubMenu1, MF_STRING, OnReloadSoftware, L"Reload");
	AppendMenu(SubMenu1, MF_STRING, OnSaveFile, L"Save");
	AppendMenu(SubMenu1, MF_SEPARATOR, NULL, NULL);
	AppendMenu(SubMenu1, MF_STRING, OnExitSoftware, L"Exit");

	AppendMenu(SubMenu2, MF_STRING, OnAboutIt, L"About it");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu1, L"File");
	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu2, L"Help");

	SetMenu(hWnd, RootMenu);
}


// - - - - - - - - - - В И Д Ж Е Т Ы - - - - - - - - - - //
void MainWndAddWidgets(HWND hWnd) {
	hListBox = CreateWindowA("LISTBOX", NULL, WS_CHILD | WS_VSCROLL | WS_BORDER | WS_VISIBLE | LBS_SORT | LB_ADDSTRING, 30, 30, 820, 350, hWnd, NULL, NULL, NULL);

	CreateWindowA("button", "Save", WS_VISIBLE | WS_CHILD | ES_CENTER, 30, 425, 150, 30, hWnd, (HMENU)OnSaveFile, NULL, NULL);
	CreateWindowA("button", "Delete", WS_VISIBLE | WS_CHILD | ES_CENTER, 250, 425, 150, 30, hWnd, (HMENU)DeleteApp, NULL, NULL);
	CreateWindowA("button", "new", WS_VISIBLE | WS_CHILD | ES_CENTER, 470, 425, 150, 30, hWnd, NULL, NULL, NULL);
	CreateWindowA("button", "Exit", WS_VISIBLE | WS_CHILD | ES_CENTER, 700, 425, 150, 30, hWnd, (HMENU)OnExitSoftware, NULL, NULL);

}


// - - - - - - - - - - С О Х Р А Н Е Н И Е - В - Ф А Й Л - - - - - - - - - - //
void SaveDate(LPCSTR path) {
	HANDLE FileToSave = CreateFileA(
		path,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	int saveLenth = GetWindowTextLength(hListBox) + 1;
	char* data = new char[saveLenth];

	saveLenth = GetWindowTextA(hListBox, data, saveLenth);

	DWORD bytesIterated;
	WriteFile(FileToSave, data, saveLenth, &bytesIterated, NULL);

	CloseHandle(FileToSave);
	delete[] data;
}


// - - - - - - - - - - У Д А Л Е Н И Е - - - - - - - - - - //
void DeleteApp() {

	//----------------------------------------------

}


// - - - - - - - - - - С Ч И Т Ы В А Н И Е - Р Е Е С Т Р А - - - - - - - - - - //
void MainWndStart(HWND hWnd) {

	SendMessage(hListBox, LB_RESETCONTENT, 0, 0);

	DWORD dwIndex = 0;

	LONG ret;
	DWORD cbName = 256;
	TCHAR szSubKeyName[256];
	char szDisplayName[256];
	DWORD dwSize;
	DWORD dwType;
	HKEY hKey;
	
	
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_READ | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
				return;
			// Поочередно проходим по каждому вложенному разделу
			while ((ret = RegEnumKeyEx(hKey, dwIndex, szSubKeyName, &cbName, NULL, NULL, NULL, NULL)) != ERROR_NO_MORE_ITEMS)
			{
				// открываем вложенный раздел и ищем в нем ключ DisplayName
				HKEY hItem;
				if ((RegOpenKeyEx(hKey, szSubKeyName, 0, KEY_READ | KEY_WOW64_64KEY, &hItem) != ERROR_SUCCESS))
					continue;
				// отображаем на экране название установленной программы
				dwSize = sizeof(szDisplayName);
				DWORD dwValue = 0;
				DWORD dwSizeValue = sizeof(dwValue);
				if (
					RegQueryValueEx(hItem, L"ReleaseType", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_FILE_NOT_FOUND and
					RegQueryValueEx(hItem, L"ParentDisplayName", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_FILE_NOT_FOUND and
					RegQueryValueEx(hItem, L"SystemComponent", NULL, NULL, (LPBYTE)&dwValue, &dwSizeValue) and
					RegQueryValueEx(hItem, L"DisplayName", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_SUCCESS) {

					SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)(LPSTR)szDisplayName);

				}
				RegCloseKey(hItem);
				dwIndex++;
				cbName = 256;
			}
			RegCloseKey(hKey);
	

	}


void MainWndStart0(HWND hWnd) {
	MainWndStart1(hWnd);
	MainWndStart2(hWnd);
	MainWndStart3(hWnd);
}


void MainWndStart1(HWND hWnd) {

	SendMessage(hListBox, LB_RESETCONTENT, 0, 0);

	DWORD dwIndex = 0;

	LONG ret;
	DWORD cbName = 256;
	TCHAR szSubKeyName[256];
	char szDisplayName[256];
	DWORD dwSize;
	DWORD dwType;
	HKEY hKey;


	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_READ | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
		return;
	// Поочередно проходим по каждому вложенному разделу
	while ((ret = RegEnumKeyEx(hKey, dwIndex, szSubKeyName, &cbName, NULL, NULL, NULL, NULL)) != ERROR_NO_MORE_ITEMS)
	{
		// открываем вложенный раздел и ищем в нем ключ DisplayName
		HKEY hItem;
		if ((RegOpenKeyEx(hKey, szSubKeyName, 0, KEY_READ | KEY_WOW64_64KEY, &hItem) != ERROR_SUCCESS))
			continue;
		// отображаем на экране название установленной программы
		dwSize = sizeof(szDisplayName);
		DWORD dwValue = 0;
		DWORD dwSizeValue = sizeof(dwValue);
		if (
			RegQueryValueEx(hItem, L"ReleaseType", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_FILE_NOT_FOUND and
			RegQueryValueEx(hItem, L"ParentDisplayName", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_FILE_NOT_FOUND and
			RegQueryValueEx(hItem, L"SystemComponent", NULL, NULL, (LPBYTE)&dwValue, &dwSizeValue) and
			RegQueryValueEx(hItem, L"DisplayName", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_SUCCESS) {

			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)(LPSTR)szDisplayName);

		}
		RegCloseKey(hItem);
		dwIndex++;
		cbName = 256;
	}
	RegCloseKey(hKey);
}

void MainWndStart2(HWND hWnd) {

	DWORD dwIndex = 0;

	LONG ret;
	DWORD cbName = 256;
	TCHAR szSubKeyName[256];
	char szDisplayName[256];
	DWORD dwSize;
	DWORD dwType;
	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_READ | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
		return;
	// Поочередно проходим по каждому вложенному разделу
	while ((ret = RegEnumKeyEx(hKey, dwIndex, szSubKeyName, &cbName, NULL, NULL, NULL, NULL)) != ERROR_NO_MORE_ITEMS)
	{
		// открываем вложенный раздел и ищем в нем ключ DisplayName
		HKEY hItem;
		if ((RegOpenKeyEx(hKey, szSubKeyName, 0, KEY_READ | KEY_WOW64_64KEY, &hItem) != ERROR_SUCCESS))
			continue;
		// отображаем на экране название установленной программы
		dwSize = sizeof(szDisplayName);
		DWORD dwValue = 0;
		DWORD dwSizeValue = sizeof(dwValue);
		if (
			RegQueryValueEx(hItem, L"ReleaseType", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_FILE_NOT_FOUND and
			RegQueryValueEx(hItem, L"ParentDisplayName", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_FILE_NOT_FOUND and
			RegQueryValueEx(hItem, L"SystemComponent", NULL, NULL, (LPBYTE)&dwValue, &dwSizeValue) and
			RegQueryValueEx(hItem, L"DisplayName", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_SUCCESS) {

			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)(LPSTR)szDisplayName);

		}
		RegCloseKey(hItem);
		dwIndex++;
		cbName = 256;
	}
	RegCloseKey(hKey);
}

void MainWndStart3(HWND hWnd){

	DWORD dwIndex = 0;

	LONG ret;
	DWORD cbName = 256;
	TCHAR szSubKeyName[256];
	char szDisplayName[256];
	DWORD dwSize;
	DWORD dwType;
	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, L"installer\\Products", 0, KEY_READ | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
		return;
	// Поочередно проходим по каждому вложенному разделу
	while ((ret = RegEnumKeyEx(hKey, dwIndex, szSubKeyName, &cbName, NULL, NULL, NULL, NULL)) != ERROR_NO_MORE_ITEMS)
	{
		// открываем вложенный раздел и ищем в нем ключ DisplayName
		HKEY hItem;
		if ((RegOpenKeyEx(hKey, szSubKeyName, 0, KEY_READ | KEY_WOW64_64KEY, &hItem) != ERROR_SUCCESS))
			continue;
		// отображаем на экране название установленной программы
		dwSize = sizeof(szDisplayName);
		DWORD dwValue = 0;
		DWORD dwSizeValue = sizeof(dwValue);
		if (
			RegQueryValueEx(hItem, L"ReleaseType", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_FILE_NOT_FOUND and
			RegQueryValueEx(hItem, L"ParentDisplayName", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_FILE_NOT_FOUND and
			RegQueryValueEx(hItem, L"SystemComponent", NULL, NULL, (LPBYTE)&dwValue, &dwSizeValue) and
			RegQueryValueEx(hItem, L"ProductName", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_SUCCESS) {

			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)(LPSTR)szDisplayName);

		}
		RegCloseKey(hItem);
		dwIndex++;
		cbName = 256;
	}
	RegCloseKey(hKey);


}
