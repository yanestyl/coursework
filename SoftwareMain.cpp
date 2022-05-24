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
		MainWndStart(hWnd, NULL, ProgramName);
		break;
	case WM_COMMAND:
		switch (wp) {
		case OnReloadSoftware:
			MainWndStart(hWnd, NULL, ProgramName);
			break;
		case OnSaveFile:
			SaveDate("output.txt");
			break;
		case OnExitSoftware:
			PostQuitMessage(0);
			break;
		case OnAboutIt:
			AboutIt(hWnd);
			break;
		case OnDelete:
			DeleteApp(hWnd);
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
	hListBox = CreateWindowA("LISTBOX", NULL, WS_CHILD  | WS_VISIBLE  | LB_ADDSTRING | LBS_STANDARD, 30, 30, 820, 350, hWnd, NULL, NULL, NULL);

	CreateWindowA("button", "Save", WS_VISIBLE | WS_CHILD | ES_CENTER, 30, 425, 150, 30, hWnd, (HMENU)OnSaveFile, NULL, NULL);
	CreateWindowA("button", "Delete", WS_VISIBLE | WS_CHILD | ES_CENTER, 250, 425, 150, 30, hWnd, (HMENU)OnDelete, NULL, NULL);
	CreateWindowA("button", "Reload", WS_VISIBLE | WS_CHILD | ES_CENTER, 470, 425, 150, 30, hWnd, (HMENU)OnReloadSoftware, NULL, NULL);
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
	
	int i = 0;
	int count = SendMessageA(hListBox, LB_GETCOUNT, NULL, NULL);
	

	while (i < count) 
	{
		int size = SendMessageA(hListBox, LB_GETTEXT, (WPARAM)i, NULL)+1;
		wchar_t* data = new wchar_t[size];
		int dbg = SendMessageA(hListBox, LB_GETTEXT, i, (LPARAM)data);

		WriteFile(FileToSave, data, dbg, NULL, NULL);
		WriteFile(FileToSave, "\n", 1, NULL, NULL);
		i++;
		delete[] data;
	}




	CloseHandle(FileToSave);
	
}


// - - - - - - - - - - У Д А Л Е Н И Е - - - - - - - - - - //
void DeleteApp(HWND hWnd) {

	int uSelectedItem;
	
	
	// Определяем номер выделенной строки
	uSelectedItem = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0);
	// Если в списке есть выделенная строка,
		
	if (uSelectedItem != LB_ERR)
	{
		// Получаем выделенную строку
		SendMessageA(hListBox, LB_GETTEXT, uSelectedItem, (LPARAM)ProgramName);
		uninstall = true;
		
	}
	MainWndStart(hWnd, uninstall, ProgramName);
}


// - - - - - - - - - - С Ч И Т Ы В А Н И Е - Р Е Е С Т Р А - - - - - - - - - - //



void MainWndStart(HWND hWnd, bool uninstall, char ProgramName[]) {
	SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
	
	MainWndStart1(hWnd, uninstall, ProgramName);
	MainWndStart2(hWnd, uninstall, ProgramName);
	MainWndStart3(hWnd, uninstall, ProgramName);
}


void MainWndStart1(HWND hWnd, bool uninstall, char ProgramName[]) {
	
	DWORD dwIndex = 0;

	LONG ret;
	DWORD cbName = 256;
	TCHAR szSubKeyName[256];
	char szDisplayName[256];
	char szReleaseType[256];
	char szParentDisplayName[256];
	char delName[256];
	DWORD delSize;
	DWORD pdnSize;
	DWORD rtSize;
	DWORD dwSize;
	DWORD dwType;
	HKEY hKey;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_READ | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
		return;
	// Поочередно проходим по каждому вложенному разделу
	while ((ret = RegEnumKeyEx(hKey, dwIndex, szSubKeyName, &cbName, NULL, NULL, NULL, NULL)) != ERROR_NO_MORE_ITEMS)
	{
		// открываем вложенный раздел и ищем в нем ключи
		HKEY hItem;
		if ((RegOpenKeyEx(hKey, szSubKeyName, 0, KEY_READ | KEY_WOW64_64KEY, &hItem) != ERROR_SUCCESS))
			continue;
		// отображаем на экране название установленной программы
		dwSize = sizeof(szDisplayName);
		rtSize = sizeof(szReleaseType);
		pdnSize = sizeof(szParentDisplayName);
		delSize = sizeof(delName);
		DWORD dwValue = 0;
		DWORD dwSizeValue = sizeof(dwValue);
		if (
			
			RegQueryValueExA(hItem, "ReleaseType", NULL, &dwType, (LPBYTE)&szReleaseType, &rtSize) == ERROR_FILE_NOT_FOUND and
			RegQueryValueExA(hItem, "ParentDisplayName", NULL, &dwType, (LPBYTE)&szParentDisplayName, &pdnSize) == ERROR_FILE_NOT_FOUND and
			RegQueryValueExA(hItem, "SystemComponent", NULL, NULL, (LPBYTE)&dwValue, &dwSizeValue) and
			RegQueryValueExA(hItem, "UninstallString", NULL, &dwType, (LPBYTE)&delName, &delSize) == ERROR_SUCCESS and
			RegQueryValueExA(hItem, "DisplayName", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_SUCCESS) 
		{
			
			std::string NameReestr = std::string(szDisplayName);
			std::string NameListbox = std::string(ProgramName);

			std::string UninstallString = std::string(delName);

			SendMessageA(hListBox, LB_ADDSTRING, 0, (LPARAM)(LPSTR)szDisplayName);
			if (uninstall and (NameReestr == NameListbox))
			{
				uninstall = false;
				system(delName);
			
			}
			

		}
		RegCloseKey(hItem);
		dwIndex++;
		cbName = 256;
	}
	
	RegCloseKey(hKey);
		
}

void MainWndStart2(HWND hWnd, bool uninstall, char ProgramName[]) {

	DWORD dwIndex = 0;

	LONG ret;
	DWORD cbName = 256;
	TCHAR szSubKeyName[256];
	char szDisplayName[256];
	char szReleaseType[256];
	char szParentDisplayName[256];
	char delName[256];
	DWORD delSize;
	DWORD pdnSize;
	DWORD rtSize;
	DWORD dwSize;
	DWORD dwType;
	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_READ | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
		return;
	// Поочередно проходим по каждому вложенному разделу
	while ((ret = RegEnumKeyEx(hKey, dwIndex, szSubKeyName, &cbName, NULL, NULL, NULL, NULL)) != ERROR_NO_MORE_ITEMS)
	{
		// открываем вложенный раздел и ищем в нем ключи
		HKEY hItem;
		if ((RegOpenKeyEx(hKey, szSubKeyName, 0, KEY_READ | KEY_WOW64_64KEY, &hItem) != ERROR_SUCCESS))
			continue;
		// отображаем на экране название установленной программы
		dwSize = sizeof(szDisplayName);
		rtSize = sizeof(szReleaseType);
		pdnSize = sizeof(szParentDisplayName);
		delSize = sizeof(delName);
		DWORD dwValue = 0;
		DWORD dwSizeValue = sizeof(dwValue);
		if (

			RegQueryValueExA(hItem, "ReleaseType", NULL, &dwType, (LPBYTE)&szReleaseType, &rtSize) == ERROR_FILE_NOT_FOUND and
			RegQueryValueExA(hItem, "ParentDisplayName", NULL, &dwType, (LPBYTE)&szParentDisplayName, &pdnSize) == ERROR_FILE_NOT_FOUND and
			RegQueryValueExA(hItem, "SystemComponent", NULL, NULL, (LPBYTE)&dwValue, &dwSizeValue) and
			RegQueryValueExA(hItem, "UninstallString", NULL, &dwType, (LPBYTE)&delName, &pdnSize) == ERROR_SUCCESS and
			RegQueryValueExA(hItem, "DisplayName", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_SUCCESS)
		{	
			std::string NameReestr = std::string(szDisplayName);
			std::string NameListbox = std::string(ProgramName);

			std::string UninstallString = std::string(delName);

			SendMessageA(hListBox, LB_ADDSTRING, 0, (LPARAM)(LPSTR)szDisplayName);
			if (uninstall and (NameReestr == NameListbox))
			{
				uninstall = false;
				system(delName);

			}
			
			

		}
		RegCloseKey(hItem);
		dwIndex++;
		cbName = 256;
	}
	RegCloseKey(hKey);
}

void MainWndStart3(HWND hWnd, bool uninstall, char ProgramName[]){

	DWORD dwIndex = 0;

	LONG ret;
	DWORD cbName = 256;
	TCHAR szSubKeyName[256];
	char szDisplayName[256];
	char szReleaseType[256];
	char szParentDisplayName[256];
	char delName[256];
	DWORD delSize;
	DWORD pdnSize;
	DWORD rtSize;
	DWORD dwSize;
	DWORD dwType;
	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, L"installer\\Products", 0, KEY_READ | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
		return;
	// Поочередно проходим по каждому вложенному разделу
	while ((ret = RegEnumKeyEx(hKey, dwIndex, szSubKeyName, &cbName, NULL, NULL, NULL, NULL)) != ERROR_NO_MORE_ITEMS)
	{
		// открываем вложенный раздел и ищем в нем ключи
		HKEY hItem;
		if ((RegOpenKeyEx(hKey, szSubKeyName, 0, KEY_READ | KEY_WOW64_64KEY, &hItem) != ERROR_SUCCESS))
			continue;
		// отображаем на экране название установленной программы
		dwSize = sizeof(szDisplayName);
		rtSize = sizeof(szReleaseType);
		pdnSize = sizeof(szParentDisplayName);
		delSize = sizeof(delName);
		DWORD dwValue = 0;
		DWORD dwSizeValue = sizeof(dwValue);
		if (

			RegQueryValueExA(hItem, "ReleaseType", NULL, &dwType, (LPBYTE)&szReleaseType, &rtSize) == ERROR_FILE_NOT_FOUND and
			RegQueryValueExA(hItem, "ParentDisplayName", NULL, &dwType, (LPBYTE)&szParentDisplayName, &pdnSize) == ERROR_FILE_NOT_FOUND and
			RegQueryValueExA(hItem, "SystemComponent", NULL, NULL, (LPBYTE)&dwValue, &dwSizeValue) and
			RegQueryValueExA(hItem, "UninstallString", NULL, &dwType, (LPBYTE)&delName, &delSize) == ERROR_SUCCESS and
			RegQueryValueExA(hItem, "DisplayName", NULL, &dwType, (LPBYTE)&szDisplayName, &dwSize) == ERROR_SUCCESS)
		{
			std::string NameReestr = std::string(szDisplayName);
			std::string NameListbox = std::string(ProgramName);

			std::string UninstallString = std::string(delName);

			SendMessageA(hListBox, LB_ADDSTRING, 0, (LPARAM)(LPSTR)szDisplayName);
			if (uninstall and (NameReestr == NameListbox))
			{
				uninstall = false;
				system(delName);

			}

		}
		RegCloseKey(hItem);
		dwIndex++;
		cbName = 256;
	}
	RegCloseKey(hKey);


}



void AboutIt(HWND hWnd) 
{
	MessageBoxA(hWnd, "application destroyer - приложение для управления \nустановленными прграммами.\n\nИнформация по кнопкам: \nSave - Сохранить список программ в текстовый файл(output.txt в корневой папке); \nDelete - удалить программу; \nReload - обновить список в реальном времени; \nExit - выйти из приложения. \n\n\nРазработал студент КФ МГТУ им. Н.Э. Баумана \nгр. ИУК5-41Б \nСтародуб Ярослав", "About It", MB_OK | MB_ICONINFORMATION);
}
