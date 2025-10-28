#include <CoreDefine.h>
#include <Object.h>
#include <CoreManager.h>

void Engine::Object::OnCompleteDestroyMark()
{
	Management->ScheduleDestroyPoint();
	_isMark = true; 
	_isCompleteDestroy = true;
}

void Engine::Object::CreateObjectDirectory()
{
	string convertName = _name.c_str();
	_pwstring basePath = L"Assets";
	CreateDirectory(basePath, nullptr);

	_pwstring path = (string)basePath + L"\\" + convertName;
	if(CreateDirectory(path, nullptr))
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
			return;
	}
}

_uint Engine::Object::ID = 0;


