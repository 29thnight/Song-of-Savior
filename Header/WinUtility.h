#pragma once
#include <Vector2.h>
#include <string>

namespace WinUtility
{
    /* 
      ������ â�� ���� ��θ� ������ �����ɴϴ�. 
      �Ű������� ���ͷ� ������ Ȯ���� �Դϴ�.
    */
    std::wstring GetOpenFilePath(const wchar_t* fileType = L"*");
    //��뿹�� fileTypes = { {L"Ȯ���� ����", L"Ȯ����"}, {L"aasset���� (*.aasset)", L"*.aasset"} };
	std::wstring GetOpenFilePath(HWND hWnd, std::vector<std::pair<std::wstring, std::wstring>> fileFilter);
    /*
      ������ â�� ���� ��θ� ������ �����ɴϴ�.
      �Ű������� ���ͷ� ������ Ȯ���� �Դϴ�.
    */
    std::wstring GetSaveAsFilePath(const wchar_t* fileType = L"*");
    //��뿹�� fileTypes = { {L"Ȯ���� ����", L"Ȯ����"}, {L"aasset���� (*.aasset)", L"*.aasset"} };
	std::wstring GetSaveAsFilePath(HWND hWnd, std::vector<std::pair<std::wstring, std::wstring>> fileFilter);

    /*
      �޽��� �ڽ��� ���� Ȯ���� ���� �޽��� �ڽ��� ���ϴ�.
      ����ڰ� Ȯ�� ������ true�� ��ȯ�ϰ�, ��Ҹ� ������ false�� ��ȯ�մϴ�.
      * title : �޽��� �ڽ��� ����
      * text : �˸�â�� �� �ȳ���
    */
    bool ShowConfirmationDialog(const wchar_t* title, const wchar_t* text);

    /*
    * ����ҷ��� ������Ʈ�� D2DEngine.rc ������ �߰� �ؾ� �մϴ�.
      int�� �Է¹޴� â�� ���ϴ�.
      �߸��� ���� �Է��ϸ� -1�� ��ȯ�մϴ�.
      * title : �ڽ��� ����
      * text : ����
    */
    int GetIntFromUser(LPCWSTR title, LPCWSTR text);

    /*
     * ����ҷ��� ������Ʈ�� D2DEngine.rc ������ �߰� �ؾ� �մϴ�.
     float�� �Է¹޴� â�� ���ϴ�.
     �߸��� ���� �Է��ϸ� -1�� ��ȯ�մϴ�.
     * title : �ڽ��� ����
     * text : ����
    */
    float GetFloatFromUser(LPCWSTR title, LPCWSTR text);

    /*
     * ����ҷ��� ������Ʈ�� D2DEngine.rc ������ �߰� �ؾ� �մϴ�.
     String�� �Է¹޴� â�� ���ϴ�.
     �߸��� ���� �Է��ϸ� ""�� ��ȯ�մϴ�.
     * title : �ڽ��� ����
     * text : ����
    */
    std::wstring GetStringFromUser(LPCWSTR title, LPCWSTR text);

    /*
    * ����ҷ��� ������Ʈ�� ObjBaseEngine.rc ������ �߰� �ؾ� �մϴ�.
      float 2���� �Է¹޴� â�� ���ϴ�.
      �߸��� ���� �Է��ϸ� {0, 0}�� ��ȯ�մϴ�.
      * title : �ڽ��� ����
    */
    Mathf::Vector2 GetVector2FromUser(LPCWSTR title);

    /*
     * ���޹��� hWnd â�� ȭ�� ����� ��ġ��ŵ�ϴ�.
    */
    void WindowToScrrenCenter(HWND hwnd);

    /*
        * Ư�� ������ Ư�� Ȯ���ڸ� ���� ������ ���� ã�Ƽ� �̸����� vector�� ��ȯ�ϴ� �Լ��Դϴ�.
        * folderPath : �˻��� ����
        * extension : ���͸� �� Ȯ����. (�⺻�� : *)
    */
    std::vector<std::wstring> FindFilesWithExtension(const wchar_t* folderPath, const wchar_t* extension = L"*");
};