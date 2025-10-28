#include <ClientEntry.h>
#include <ClientWorld.h>
#include <PrototypeEditerWorld.h>
#include <CoreManager.h>

//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif

DEFINE_WWINMAIN_RELEASE("GameSettings.ini", NULL);

void Client::ClientEntry::CreateWorld()
{
	_pWorld = ClientWorld::Create();
}