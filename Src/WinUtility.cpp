#include "WinUtility.h"
#include "../resource.h"
#include <CoreManager.h>
#include <tchar.h>
#include <cassert>
#include <cstring>
#include <UtfConvert.h>
#include <InputManager.h>

std::wstring WinUtility::GetOpenFilePath(const wchar_t* fileType)
{
	HWND hWnd = Management->setting.hWnd;
	if (hWnd)
	{
		OPENFILENAME ofn;
		wchar_t filter[30];
		const size_t len = wcslen(fileType);
		wcscpy_s(filter, _ARRAYSIZE(filter), fileType);
		wchar_t temp[20] = L"\0*.";
		for (size_t i = 0; i < len + 1; i++)
		{
			temp[i + 3] = fileType[i]; //널문자를 포함하는 문자열 이여서 전부 순회해야함
		}
		for (size_t i = 0; i < _ARRAYSIZE(temp) - 1; i++)
		{
			if (unsigned long long(i + len) == _ARRAYSIZE(filter))
				break;
			filter[i + len] = temp[i]; //널문자를 포함하는 문자열 이여서 전부 순회해야함
		}

		wchar_t szFile[256]; // 선택된 파일 경로를 저장할 배열
		ZeroMemory(&ofn, sizeof(ofn));
		ZeroMemory(szFile, sizeof(szFile));

		// 파일 경로 선택 창 초기화
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;// 필터링할 파일 타입 지정
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		// 파일 경로 선택 창 띄우기
		if (GetOpenFileName(&ofn) == TRUE)
		{
			return ofn.lpstrFile; // 선택한 파일 경로 반환
		}
		else
		{
			return L""; // 사용자가 취소한 경우 빈 문자열 반환
		}
	}

	assert(!"hWnd가 존재하지 않습니다.");
	return L""; 
}

std::wstring WinUtility::GetOpenFilePath(HWND hWnd, std::vector<std::pair<std::wstring, std::wstring>> fileFilter)
{
	if (hWnd)
	{
		OPENFILENAME ofn;
		wchar_t szFile[256]; // 선택된 파일 경로를 저장할 배열
		ZeroMemory(&ofn, sizeof(ofn));
		ZeroMemory(szFile, sizeof(szFile));
		std::wstring combineFilter;
		for (auto& filter : fileFilter)
		{
			combineFilter += filter.first + L'\0' + filter.second + L'\0';
		}

		// 파일 경로 선택 창 초기화
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = combineFilter.c_str();// 필터링할 파일 타입 지정
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		// 파일 경로 선택 창 띄우기
		if (GetOpenFileName(&ofn) == TRUE)
		{
			return ofn.lpstrFile; // 선택한 파일 경로 반환
		}
		else
		{
			return L""; // 사용자가 취소한 경우 빈 문자열 반환
		}
	}

	assert(!"hWnd가 존재하지 않습니다.");
	return L""; 
}

std::wstring WinUtility::GetSaveAsFilePath(const wchar_t* fileType)
{
	HWND hWnd = Management->setting.hWnd;
	if (hWnd)
	{
		OPENFILENAME ofn;
		wchar_t filter[30];
		const size_t len = wcslen(fileType);
		wcscpy_s(filter, _ARRAYSIZE(filter), fileType);
		wchar_t temp[20] = L"\0*.";
		for (size_t i = 0; i < len + 1; i++)
		{
			temp[i + 3] = fileType[i]; //널문자를 포함하는 문자열 이여서 전부 순회해야함
		}
		for (size_t i = 0; i < _ARRAYSIZE(temp) - 1; i++)
		{
			if (unsigned long long(i + len) == _ARRAYSIZE(filter))
				break;
			filter[i + len] = temp[i]; //널문자를 포함하는 문자열 이여서 전부 순회해야함
		}

		wchar_t szFile[256]; // 선택된 파일 경로를 저장할 배열
		ZeroMemory(&ofn, sizeof(ofn));
		ZeroMemory(szFile, sizeof(szFile));

		// 파일 경로 선택 창 초기화
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;// 필터링할 파일 타입 지정
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.lpstrTitle = L"다른 이름으로 저장";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// 파일 경로 선택 창 띄우기
		if (GetOpenFileName(&ofn) == TRUE)
		{
			return ofn.lpstrFile; // 선택한 파일 경로 반환
		}
		else
		{
			return L""; // 사용자가 취소한 경우 빈 문자열 반환
		}
	}

	assert(!"hWnd가 존재하지 않습니다.");
	return L"";
}

std::wstring WinUtility::GetSaveAsFilePath(HWND hWnd, std::vector<std::pair<std::wstring, std::wstring>> fileFilter)
{
	if (hWnd)
	{
		OPENFILENAME ofn;
		wchar_t szFile[256]; // 선택된 파일 경로를 저장할 배열
		ZeroMemory(&ofn, sizeof(ofn));
		ZeroMemory(szFile, sizeof(szFile));
		std::wstring combineFilter;
		for (auto& filter : fileFilter)
		{
			combineFilter += filter.first + L'\0' + filter.second + L'\0';
		}

		// 파일 경로 선택 창 초기화
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = combineFilter.c_str();// 필터링할 파일 타입 지정
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.lpstrTitle = L"다른 이름으로 저장";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// 파일 경로 선택 창 띄우기
		if (GetOpenFileName(&ofn) == TRUE)
		{
			return ofn.lpstrFile; // 선택한 파일 경로 반환
		}
		else
		{
			return L""; // 사용자가 취소한 경우 빈 문자열 반환
		}
	}

	assert(!"hWnd가 존재하지 않습니다.");
	return L"";
}

bool WinUtility::ShowConfirmationDialog(const wchar_t* title, const wchar_t* text)
{
	// MessageBox 함수 호출
	int result = MessageBox
	(
		NULL,                // 부모 창 핸들 (NULL은 부모 창이 없음을 의미)
		text,    // 메시지 박스에 표시할 텍스트
		title,     // 메시지 박스의 제목
		MB_OKCANCEL | MB_ICONQUESTION // 확인 및 취소 버튼을 포함한 메시지 박스 스타일
	);

	// 사용자가 누른 버튼에 따라 true 또는 false 반환
	if (result == IDOK) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

struct DialogDataInt 
{
	LPCWSTR title;
	LPCWSTR message;
};

INT_PTR CALLBACK DialogProcInt(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	DialogDataInt* data;
	if (message == WM_INITDIALOG)
	{
		InputMgr->ReturnKeyboard()->Unacquire();
		data = (DialogDataInt*)lParam;
		SetWindowText(hDlg, data->title); // 다이얼로그 제목 설정
		SetDlgItemText(hDlg, IDC_STATIC_TEXT, data->message); // 메시지 설정
		WinUtility::WindowToScrrenCenter(hDlg);
		return (INT_PTR)TRUE;
	}
	switch (message)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK) {
				TCHAR buffer[10];
				GetDlgItemText(hDlg, IDC_EDIT1, buffer, 10);
				int value = _ttoi(buffer);
				EndDialog(hDlg, value);
			}
			else {
				EndDialog(hDlg, -1);
			}
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int WinUtility::GetIntFromUser(LPCWSTR title, LPCWSTR text)
{
	DialogDataInt data = { title, text };
	int result = (int)DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INPUT_INT_DIALOG), Management->setting.hWnd, DialogProcInt, (LPARAM)&data);
	if (auto error = GetLastError())
	{
		assert(false);
	}
	return result;
}

struct DialogDataFloat
{
	LPCWSTR title;
	LPCWSTR message;
	float* data;
};

INT_PTR CALLBACK DialogProcFloat(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static float* temp = nullptr;
	DialogDataFloat* data;
	if (message == WM_INITDIALOG)
	{
		data = (DialogDataFloat*)lParam;
		temp = data->data;
		SetWindowText(hDlg, data->title); // 다이얼로그 제목 설정
		SetDlgItemText(hDlg, IDC_STATIC_TEXT, data->message); // 메시지 설정
		WinUtility::WindowToScrrenCenter(hDlg);
		return (INT_PTR)TRUE;
	}
	switch (message)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK) {
				TCHAR buffer1[30];
				GetDlgItemText(hDlg, IDC_EDIT1, buffer1, 10);
				float input = static_cast<float>(_ttof(buffer1));
				if (temp)
				{
					*temp = input;
					EndDialog(hDlg, 1);
				}
				else
					EndDialog(hDlg, -1);
			}
			else {
				EndDialog(hDlg, -1);
			}
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

float WinUtility::GetFloatFromUser(LPCWSTR title, LPCWSTR text)
{
	float temp;
	DialogDataFloat data = { title , text, &temp };

	bool reslut = DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INPUT_FLOAT_DIALOG), Management->setting.hWnd, DialogProcFloat, (LPARAM)&data);
	if (!reslut)
	{
		assert(false);
	}
	return temp;
}


struct  DialogDataString
{
	LPCWSTR title;
	LPCWSTR message;
	std::wstring* data;
};

INT_PTR CALLBACK DialogProcString(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static std::wstring* temp = nullptr;
	DialogDataString* data;
	if (message == WM_INITDIALOG)
	{
		data = (DialogDataString*)lParam;
		temp = data->data;
		SetWindowText(hDlg, data->title); // 다이얼로그 제목 설정
		SetDlgItemText(hDlg, IDC_STATIC_TEXT, data->message); // 메시지 설정
		WinUtility::WindowToScrrenCenter(hDlg);
		return (INT_PTR)TRUE;
	}
	switch (message)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK) {
				TCHAR buffer1[50];
				GetDlgItemText(hDlg, IDC_EDIT1, buffer1, 50);
				std::wstring input{buffer1};
				if (temp)
				{
					*temp = input;
					EndDialog(hDlg, 1);
				}
				else
					EndDialog(hDlg, -1);
			}
			else {
				EndDialog(hDlg, -1);
			}
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

std::wstring WinUtility::GetStringFromUser(LPCWSTR title, LPCWSTR text)
{
	std::wstring temp;
	DialogDataString data = { title , text, &temp };

	bool reslut = DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INPUT_FLOAT_DIALOG), Management->setting.hWnd, DialogProcString, (LPARAM)&data);
	if (!reslut)
	{
		assert(false);
	}
	return temp;
}



struct DialogDataVector
{
	LPCWSTR title;
	Mathf::Vector2* vector2;
};

INT_PTR CALLBACK DialogProcVector(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Mathf::Vector2* temp = nullptr;
	DialogDataVector* data;	
	if (message == WM_INITDIALOG)
	{		
		data = (DialogDataVector*)lParam;
		temp = data->vector2;
		SetWindowText(hDlg, data->title); // 다이얼로그 제목 설정
		SetDlgItemText(hDlg, IDC_EDIT1, L"x");
		SetDlgItemText(hDlg, IDC_EDIT2, L"y");
		WinUtility::WindowToScrrenCenter(hDlg);
		return (INT_PTR)TRUE;
	}
	switch (message)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK) {
				TCHAR buffer1[30];
				TCHAR buffer2[30];
				GetDlgItemText(hDlg, IDC_EDIT1, buffer1, 10);
				GetDlgItemText(hDlg, IDC_EDIT2, buffer2, 10);
				float x = static_cast<float>(_ttof(buffer1));
				float y = static_cast<float>(_ttof(buffer2));
				if (temp)
				{
					temp->x = x;
					temp->y = y;
					EndDialog(hDlg, 1);
				}
				else
					EndDialog(hDlg, -1);		
			}
			else {
				EndDialog(hDlg, -1);
			}
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

Mathf::Vector2 WinUtility::GetVector2FromUser(LPCWSTR title)
{
	Mathf::Vector2 temp;
	DialogDataVector data = { title , &temp };
	
	bool reslut = DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INPUT_VECTOR2_DIALOG), Management->setting.hWnd, DialogProcVector, (LPARAM)&data);
	if (!reslut)
	{
		assert(false);
	}
	return temp;
}

void WinUtility::WindowToScrrenCenter(HWND hwnd)
{
	int x, y, width, height;
	RECT rtDesk, rtWindow;
	GetWindowRect(GetDesktopWindow(), &rtDesk);
	GetWindowRect(hwnd, &rtWindow);
	width = rtWindow.right - rtWindow.left;
	height = rtWindow.bottom - rtWindow.top;
	x = (rtDesk.right - width) / 2;
	y = (rtDesk.bottom - height) / 2;
	MoveWindow(hwnd, x, y, width, height, TRUE);
}

std::vector<std::wstring> WinUtility::FindFilesWithExtension(const wchar_t* folderPath, const wchar_t* extension)
{
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// 폴더 경로와 확장자를 조합하여 검색 패턴을 생성합니다.
	std::wstring searchPath = std::wstring(folderPath) + L"\\*." + std::wstring(extension);

	// FindFirstFile로 검색을 시작합니다.
	hFind = FindFirstFile(searchPath.c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return std::vector<std::wstring>{};
	}

	std::vector<std::wstring> fileData;
	do 
	{
		// 현재 항목이 파일인지 확인합니다.
		if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			fileData.push_back(findFileData.cFileName);
		}
	} while (FindNextFile(hFind, &findFileData) != 0);
	FindClose(hFind);
	return fileData;
}