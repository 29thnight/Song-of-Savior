#pragma once
#include <Vector2.h>
#include <string>

namespace WinUtility
{
    /* 
      윈도우 창을 열어 경로를 선택해 가져옵니다. 
      매개변수는 필터로 지정할 확장자 입니다.
    */
    std::wstring GetOpenFilePath(const wchar_t* fileType = L"*");
    //사용예시 fileTypes = { {L"확장자 설명", L"확장자"}, {L"aasset파일 (*.aasset)", L"*.aasset"} };
	std::wstring GetOpenFilePath(HWND hWnd, std::vector<std::pair<std::wstring, std::wstring>> fileFilter);
    /*
      윈도우 창을 열어 경로를 선택해 가져옵니다.
      매개변수는 필터로 지정할 확장자 입니다.
    */
    std::wstring GetSaveAsFilePath(const wchar_t* fileType = L"*");
    //사용예시 fileTypes = { {L"확장자 설명", L"확장자"}, {L"aasset파일 (*.aasset)", L"*.aasset"} };
	std::wstring GetSaveAsFilePath(HWND hWnd, std::vector<std::pair<std::wstring, std::wstring>> fileFilter);

    /*
      메시지 박스를 열어 확인을 묻는 메시지 박스를 띄웁니다.
      사용자가 확인 누르면 true를 반환하고, 취소를 누르면 false를 반환합니다.
      * title : 메시지 박스의 제목
      * text : 알림창에 들어갈 안내문
    */
    bool ShowConfirmationDialog(const wchar_t* title, const wchar_t* text);

    /*
    * 사용할려면 프로젝트에 D2DEngine.rc 파일을 추가 해야 합니다.
      int를 입력받는 창을 띄웁니다.
      잘못된 값을 입력하면 -1을 반환합니다.
      * title : 박스의 제목
      * text : 설명
    */
    int GetIntFromUser(LPCWSTR title, LPCWSTR text);

    /*
     * 사용할려면 프로젝트에 D2DEngine.rc 파일을 추가 해야 합니다.
     float를 입력받는 창을 띄웁니다.
     잘못된 값을 입력하면 -1을 반환합니다.
     * title : 박스의 제목
     * text : 설명
    */
    float GetFloatFromUser(LPCWSTR title, LPCWSTR text);

    /*
     * 사용할려면 프로젝트에 D2DEngine.rc 파일을 추가 해야 합니다.
     String을 입력받는 창을 띄웁니다.
     잘못된 값을 입력하면 ""을 반환합니다.
     * title : 박스의 제목
     * text : 설명
    */
    std::wstring GetStringFromUser(LPCWSTR title, LPCWSTR text);

    /*
    * 사용할려면 프로젝트에 ObjBaseEngine.rc 파일을 추가 해야 합니다.
      float 2개를 입력받는 창을 띄웁니다.
      잘못된 값을 입력하면 {0, 0}을 반환합니다.
      * title : 박스의 제목
    */
    Mathf::Vector2 GetVector2FromUser(LPCWSTR title);

    /*
     * 전달받은 hWnd 창을 화면 가운데로 위치시킵니다.
    */
    void WindowToScrrenCenter(HWND hwnd);

    /*
        * 특정 폴더에 특정 확장자를 가진 파일을 전부 찾아서 이름들을 vector로 반환하는 함수입니다.
        * folderPath : 검색할 폴더
        * extension : 필터를 걸 확장자. (기본값 : *)
    */
    std::vector<std::wstring> FindFilesWithExtension(const wchar_t* folderPath, const wchar_t* extension = L"*");
};