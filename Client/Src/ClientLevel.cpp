#include "ClientLevel.h"
#include "NoteManager.h"
#include "BGMManager.h"	
#include "BossManager.h"	
#include "TestActor.h"
#include "Dragon.h"
#include "Warrior.h"
#include "Thief.h"
#include "Sorcerer.h"
#include "Bard.h"

using Vector2 = Mathf::Vector2;

bool Client::ClientLevel::BeginPlay()
{
	SpawnActor(LAYER::OBJECT, "NoteManager", NoteManager::Create(), Vector2(500.f, 500.f));
	SpawnActor(LAYER::OBJECT, "BGMManager", BGMManager::Create(), Vector2(500.f, 500.f));

	SpawnActor(LAYER::OBJECT,"Dragon", Dragon::Create(),Vector2{1500.f, 700.f});
	SpawnActor(LAYER::OBJECT, "Warrior", Warrior::Create(),Vector2{900.f, 700.f });
	SpawnActor(LAYER::OBJECT, "Thief", Thief::Create(), Vector2{ 700.f, 700.f });
	SpawnActor(LAYER::OBJECT, "Sorcerer", Sorcerer::Create(), Vector2{ 500.f, 700.f });
	SpawnActor(LAYER::OBJECT, "Bard", Bard::Create(), Vector2{ 300.f, 700.f });

	SpawnActor(LAYER::OBJECT, "BossManager", BossManager::Create(), Vector2(500.f, 500.f));
	return true;
}
