#include <Windows.h>
#include <tchar.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <atlstr.h>

static TCHAR szWindowClass[] = _T("Project1");
static TCHAR szTitle[] = _T("Windows  Desktop Guided Tour Application");
TCHAR curfile[MAX_PATH];
HINSTANCE hInstance;
HWND main_window;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
#define IDC_SAVE_BUTTON 3804
#define IDC_TEXTBOX 3805
#define IDC_LOAD_BUTTON 3806
#define IDC_FORMAT_BUTTON 3807
#define IDC_VIEW_BUTTON 3808
#define IDC_HELP_BUTTON 3809

LPCTSTR lpFileName = _T("");
//LPCTSTR lpFileName = _T("\\vmware-host\\Shared Folders\\Desktop\\notepad_try.txt");
void loadFileToEdit(HWND hWnd, LPCTSTR filename);
void saveToFile(HWND hWnd, LPCTSTR);
void AppendText(HWND hWnd, LPCTSTR);

void AppendText(HWND hWnd, LPCTSTR text) {
	int idx = GetWindowTextLength(hWnd);
	SendMessage(hWnd, EM_SETSEL, (WPARAM)idx, (LPARAM)idx);
	SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM)text);
}/*
void loadFileToEdit(HWND hWnd, LPCTSTR filename) {
	HANDLE fileHandle = CreateFile(
		lpFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (!fileHandle) {
		MessageBox(NULL,
			_T("File opening failed !!"),
			_T("In while true"),
			NULL);
	}
	
	DWORD bufferSize = 256;
	LPWSTR fileDataBuffer = (LPWSTR)GlobalAlloc(GPTR, bufferSize);
	DWORD lpNumberOfBytesRead = SetFilePointer(fileHandle, 0, NULL, FILE_BEGIN);
	
	while (TRUE) {
		BOOL success = ReadFile(
			fileHandle,
			fileDataBuffer,
			bufferSize-1,
			&lpNumberOfBytesRead,
			NULL
		);
		if (!success) {
			//error in reading this block
			MessageBox(NULL,
				_T("Readfile Failed !!"),
				_T("In while true"),
				NULL);
		}
		else {
			MessageBox(NULL,
				_T("Readfile Passed !!" + lpNumberOfBytesRead),
				_T("In while true"),
				NULL);
			if (lpNumberOfBytesRead > FILE_END) {
				//reading complete
				break;
			}
			else {
				lpNumberOfBytesRead += 256;
				AppendText(hWnd, fileDataBuffer);
			}
		}
	}
	

}
*/

void pickFile() {
	curfile[0] = '\0';
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = main_window;
	ofn.lpstrFilter = L"Text Files(*.txt)\0*.txt\0All File(*.*)\0*.*\0";
	ofn.lpstrDefExt = L"txt";
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrFile = curfile;
	bool success = GetOpenFileName(&ofn);
	
	if (!success) {
		MessageBox(NULL,
			_T("File picker failed !!"),
			_T("In pickFile()"),
			NULL);
	}
	lpFileName = ofn.lpstrFile;
}
void loadFileToEdit(HWND hWnd, LPCTSTR filename) {
	bool success = false;
	HANDLE fileHandle = CreateFile(
		filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (!fileHandle) {
		MessageBox(NULL,
			_T("File opening failed !!"),
			_T("In while true"),
			NULL);
	}
	else {
		MessageBox(NULL,
			_T("File opening succeeded !!"),
			_T("In while true"),
			NULL);
	}
	DWORD dwFileSize = GetFileSize(fileHandle, NULL);
	if (dwFileSize != 0xFFFFFFFF) {
		LPWSTR text = (TCHAR*)GlobalAlloc(GPTR, dwFileSize + 1);
		DWORD dwRead;
		if (ReadFile(fileHandle, text, dwFileSize, &dwRead, NULL)) {
			MessageBox(NULL,
				_T("Read file succeeded !!"),
				_T("In read file"),
				NULL);
		}
		text[dwFileSize] = 0;
		if (SetWindowText(hWnd, text)) {
			MessageBox(NULL,
				_T(" succeeded !!"),
				_T("In setwindow text"),
				NULL);
		}
		GlobalFree(text);
		CloseHandle(fileHandle);
	}
	else {
		MessageBox(NULL,
			_T("Exceeds 0x FFFFFFFF!!"),
			_T("dwFilesize"),
			NULL);
	}
	

}
void saveToFile(HWND hWnd, LPCTSTR filename) {
	if (_tcslen(filename) == 0) {
		pickFile();
		filename = lpFileName;
	}
	HANDLE fileHandle = CreateFile(
		filename,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	//Check if file creation failed
	//
	//

	DWORD dwTextLength = GetWindowTextLength(hWnd);
	DWORD dwBytesToWrite = (dwTextLength + 1) * sizeof(TCHAR);

	DWORD dwPtr = SetFilePointer(fileHandle, 0, NULL, FILE_END);
	//LPWSTR data = (LPWSTR)malloc(512);
	LPWSTR editText = (LPWSTR)GlobalAlloc(GPTR, dwBytesToWrite);
	
	GetWindowText(hWnd, editText, dwTextLength+1);
	
	BOOL success = WriteFile(
		fileHandle,
		editText,
		dwBytesToWrite,
		&dwPtr,
		NULL
	);
	GlobalFree(editText);
	CloseHandle(fileHandle);
	
	if (success) {
		MessageBox(NULL,
			lpFileName,
			_T("writing successfull !!"),
			NULL);
		return;
	}
	else {
		MessageBox(NULL,
			lpFileName,
			_T("writing unsuccessfull !!"),
			NULL);
		return;
	}
	
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);
		return 1;
	}

	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 500,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	main_window = hWnd;

	CreateWindow(L"BUTTON", L"Load", WS_CHILD | WS_VISIBLE, 0, 0, 80, 25, hWnd, (HMENU)IDC_LOAD_BUTTON, hInstance, 0);
	CreateWindow(L"BUTTON", L"Save", WS_CHILD | WS_VISIBLE, 80, 0, 80, 25, hWnd, (HMENU)IDC_SAVE_BUTTON, hInstance, 0);
	CreateWindow(L"BUTTON", L"Format", WS_CHILD | WS_VISIBLE, 160, 0, 80, 25, hWnd, (HMENU)IDC_FORMAT_BUTTON, hInstance, 0);
	CreateWindow(L"BUTTON", L"View", WS_CHILD | WS_VISIBLE, 240, 0, 80, 25, hWnd, (HMENU)IDC_VIEW_BUTTON, hInstance, 0);
	CreateWindow(L"BUTTON", L"Help", WS_CHILD | WS_VISIBLE, 320, 0, 80, 25, hWnd, (HMENU)IDC_HELP_BUTTON, hInstance, 0);
	CreateWindow(L"Edit", L"initial_test", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL, 40, 40, 400, 400, hWnd, HMENU(IDC_TEXTBOX), hInstance, 0);
	if (!hWnd) {
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Window Desktop Guided Tour"),
			NULL);
		return 1;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)>0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, Windows desktop");
	switch (message) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_SAVE_BUTTON:
		{
			MessageBox(NULL,
				_T("Save Button clicked!"),
				_T("Save Button box"),
				NULL);
			const HWND text_box = GetDlgItem(hWnd, IDC_TEXTBOX);
			if(_tcslen(lpFileName)==0)
				pickFile();
			saveToFile(text_box, lpFileName);
			break;
		}
		case IDC_LOAD_BUTTON:
		{
			MessageBox(NULL,
				_T("Load Button clicked!"),
				_T("Load Button box"),
				NULL);
			const HWND text_box = GetDlgItem(hWnd, IDC_TEXTBOX);
			pickFile();
			loadFileToEdit(text_box, lpFileName);
			break;
		}
		}
	case WM_KEYDOWN:
		
		TCHAR c[2];
		c[0]= wParam;
		c[1] = '\0';
		OutputDebugString(L"<");
		OutputDebugString(c);
		OutputDebugString(L">\n ");
		switch (wParam) {
			
		case 's':
			if (GetAsyncKeyState(VK_CONTROL)) {
				const HWND text_box = GetDlgItem(hWnd, IDC_TEXTBOX);
				if (_tcslen(lpFileName) == 0)
					pickFile();
				saveToFile(text_box, lpFileName);
			}
			break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
