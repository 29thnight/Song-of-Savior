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
			temp[i + 3] = fileType[i]; //�ι��ڸ� �����ϴ� ���ڿ� �̿��� ���� ��ȸ�ؾ���
		}
		for (size_t i = 0; i < _ARRAYSIZE(temp) - 1; i++)
		{
			if (unsigned long long(i + len) == _ARRAYSIZE(filter))
				break;
			filter[i + len] = temp[i]; //�ι��ڸ� �����ϴ� ���ڿ� �̿��� ���� ��ȸ�ؾ���
		}

		wchar_t szFile[256]; // ���õ� ���� ��θ� ������ �迭
		ZeroMemory(&ofn, sizeof(ofn));
		ZeroMemory(szFile, sizeof(szFile));

		// ���� ��� ���� â �ʱ�ȭ
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;// ���͸��� ���� Ÿ�� ����
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		// ���� ��� ���� â ����
		if (GetOpenFileName(&ofn) == TRUE)
		{
			return ofn.lpstrFile; // ������ ���� ��� ��ȯ
		}
		else
		{
			return L""; // ����ڰ� ����� ��� �� ���ڿ� ��ȯ
		}
	}

	assert(!"hWnd�� �������� �ʽ��ϴ�.");
	return L""; 
}

std::wstring WinUtility::GetOpenFilePath(HWND hWnd, std::vector<std::pair<std::wstring, std::wstring>> fileFilter)
{
	if (hWnd)
	{
		OPENFILENAME ofn;
		wchar_t szFile[256]; // ���õ� ���� ��θ� ������ �迭
		ZeroMemory(&ofn, sizeof(ofn));
		ZeroMemory(szFile, sizeof(szFile));
		std::wstring combineFilter;
		for (auto& filter : fileFilter)
		{
			combineFilter += filter.first + L'\0' + filter.second + L'\0';
		}

		// ���� ��� ���� â �ʱ�ȭ
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = combineFilter.c_str();// ���͸��� ���� Ÿ�� ����
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		// ���� ��� ���� â ����
		if (GetOpenFileName(&ofn) == TRUE)
		{
			return ofn.lpstrFile; // ������ ���� ��� ��ȯ
		}
		else
		{
			return L""; // ����ڰ� ����� ��� �� ���ڿ� ��ȯ
		}
	}

	assert(!"hWnd�� �������� �ʽ��ϴ�.");
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
			temp[i + 3] = fileType[i]; //�ι��ڸ� �����ϴ� ���ڿ� �̿��� ���� ��ȸ�ؾ���
		}
		for (size_t i = 0; i < _ARRAYSIZE(temp) - 1; i++)
		{
			if (unsigned long long(i + len) == _ARRAYSIZE(filter))
				break;
			filter[i + len] = temp[i]; //�ι��ڸ� �����ϴ� ���ڿ� �̿��� ���� ��ȸ�ؾ���
		}

		wchar_t szFile[256]; // ���õ� ���� ��θ� ������ �迭
		ZeroMemory(&ofn, sizeof(ofn));
		ZeroMemory(szFile, sizeof(szFile));

		// ���� ��� ���� â �ʱ�ȭ
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;// ���͸��� ���� Ÿ�� ����
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.lpstrTitle = L"�ٸ� �̸����� ����";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// ���� ��� ���� â ����
		if (GetOpenFileName(&ofn) == TRUE)
		{
			return ofn.lpstrFile; // ������ ���� ��� ��ȯ
		}
		else
		{
			return L""; // ����ڰ� ����� ��� �� ���ڿ� ��ȯ
		}
	}

	assert(!"hWnd�� �������� �ʽ��ϴ�.");
	return L"";
}

std::wstring WinUtility::GetSaveAsFilePath(HWND hWnd, std::vector<std::pair<std::wstring, std::wstring>> fileFilter)
{
	if (hWnd)
	{
		OPENFILENAME ofn;
		wchar_t szFile[256]; // ���õ� ���� ��θ� ������ �迭
		ZeroMemory(&ofn, sizeof(ofn));
		ZeroMemory(szFile, sizeof(szFile));
		std::wstring combineFilter;
		for (auto& filter : fileFilter)
		{
			combineFilter += filter.first + L'\0' + filter.second + L'\0';
		}

		// ���� ��� ���� â �ʱ�ȭ
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = combineFilter.c_str();// ���͸��� ���� Ÿ�� ����
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.lpstrTitle = L"�ٸ� �̸����� ����";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// ���� ��� ���� â ����
		if (GetOpenFileName(&ofn) == TRUE)
		{
			return ofn.lpstrFile; // ������ ���� ��� ��ȯ
		}
		else
		{
			return L""; // ����ڰ� ����� ��� �� ���ڿ� ��ȯ
		}
	}

	assert(!"hWnd�� �������� �ʽ��ϴ�.");
	return L"";
}

bool WinUtility::ShowConfirmationDialog(const wchar_t* title, const wchar_t* text)
{
	// MessageBox �Լ� ȣ��
	int result = MessageBox
	(
		NULL,                // �θ� â �ڵ� (NULL�� �θ� â�� ������ �ǹ�)
		text,    // �޽��� �ڽ��� ǥ���� �ؽ�Ʈ
		title,     // �޽��� �ڽ��� ����
		MB_OKCANCEL | MB_ICONQUESTION // Ȯ�� �� ��� ��ư�� ������ �޽��� �ڽ� ��Ÿ��
	);

	// ����ڰ� ���� ��ư�� ���� true �Ǵ� false ��ȯ
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
		SetWindowText(hDlg, data->title); // ���̾�α� ���� ����
		SetDlgItemText(hDlg, IDC_STATIC_TEXT, data->message); // �޽��� ����
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
		SetWindowText(hDlg, data->title); // ���̾�α� ���� ����
		SetDlgItemText(hDlg, IDC_STATIC_TEXT, data->message); // �޽��� ����
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
		SetWindowText(hDlg, data->title); // ���̾�α� ���� ����
		SetDlgItemText(hDlg, IDC_STATIC_TEXT, data->message); // �޽��� ����
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
		SetWindowText(hDlg, data->title); // ���̾�α� ���� ����
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

	// ���� ��ο� Ȯ���ڸ� �����Ͽ� �˻� ������ �����մϴ�.
	std::wstring searchPath = std::wstring(folderPath) + L"\\*." + std::wstring(extension);

	// FindFirstFile�� �˻��� �����մϴ�.
	hFind = FindFirstFile(searchPath.c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return std::vector<std::wstring>{};
	}

	std::vector<std::wstring> fileData;
	do 
	{
		// ���� �׸��� �������� Ȯ���մϴ�.
		if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			fileData.push_back(findFileData.cFileName);
		}
	} while (FindNextFile(hFind, &findFileData) != 0);
	FindClose(hFind);
	return fileData;
}