#pragma once
#include <MainEntry.h>
#include <Actor.h>
#include <Random.h>

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	enum ENoteRate
	{
		Perfect,
		Great,
		Good,
		Miss,
		End
	};

	class Note;
	class NoteManager : public Engine::Actor
	{
		friend class TimelineManager;
		friend class NoteEditor;
		using Base = Engine::Actor;
	private:
		NoteManager() = default;
		virtual ~NoteManager() = default;

	public:
		struct NoteSpawnTable
		{
			int			_spawnIndex{};
			long double _noteTime{};
			int			_noteIndex{};
		};

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;

	public:
		void InitializeKeySetting();
		void InitializeNoteSpawnData();

	public:
		long double GetCurrNotePosition();
		const std::deque<NoteSpawnTable>& GetNoteSpawnQueue() { return _noteSpawnQueue; }

	public:
		/* LimitTime = 제한시간, TargetCount = 눌러야하는 횟수*/
		void SetSmashingMode(float LimitTime, int TargetCount);
		void SetNormalMode();
		void ChangeSmashingMode();
		void ChangeNormalMode();
		void Smashing();
		void Perfect(_ldouble hitPoint);
		void Great(_ldouble hitPoint);
		void Good(_ldouble hitPoint);
		void ChangeKeySetting(_uint hitKey);

		void currentStageNoteDataLoad();
		
		void DongDong(ENoteRate rating);

		//stage set smashing note demend count
		void SetSmashingDemention(int smashingDemention) { _SmashingDemention = smashingDemention; }
	public:
		void SetNoteSpawnData(const std::string& fileName);

	public:
		template <typename T>
		void AddSmashSuccessEvent(T* _this, void(T::* _event)());

		template <typename T>
		void AddSmashFailEvent(T* _this, void(T::* _event)());

		void SetSmashingPosition(std::vector<long double> smashingPosition);

	public:
		void SmashingFade();

	public:
		ReadOnly_Property(bool, IsPerfect);
		_Get(IsPerfect) { return _isPerfect; }

		ReadOnly_Property(bool, IsGreat);
		_Get(IsGreat) { return _isGreat; }

		ReadOnly_Property(bool, IsGood);
		_Get(IsGood) { return _isGood; }

		ReadOnly_Property(bool, IsMiss);
		_Get(IsMiss) { return _isMiss; }

	public:
		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override;

	public:
		static NoteManager* Create() { return new NoteManager; }
		void Destroy() override;

	private:
		Engine::TimerComponent*			_pTimerComponent{ nullptr };
		Engine::BitmapComponent*		_pNoteRateLineBitmap{ nullptr };
		Engine::BitmapComponent*		_pNoteDoubleLineBitmap{ nullptr };
		Engine::InputComponent*			_pInputComponent{ nullptr };
		Engine::TextRenderComponent*	_pTextRenderComponent{ nullptr };

	private:
		std::unordered_map<std::string, _uint> _convertKeyMap;
		std::vector<Note*>				_noteObjectPool;
		std::vector<NoteSpawnTable>		_noteSpawnData;
		std::deque<NoteSpawnTable>		_noteSpawnQueue;

	private:
		Mathf::Vector2 _startPosition{ 0.f, 0.f };

	private:
		Delegate<> _smashSuccessEvent;
		Delegate<> _smashFailureEvent;

	private:
		Random<int> _random{ 1, 3 };
		Random<int> _randomSmashingSound{ 0, 3 };
		std::vector<long double>	   _SmashingPosition;

	private:
		int         _previousIndex{};
		int			_noteIndex{};
		int         _spawnCount{};
		int         _noteMaxCount{ 10 };

		int			_SmashingDemention{ 20 };  //스매싱 모드 필요 노트 갯수
		int 		_SmashingCount{ 0 };	 //스매싱 모드 hit count
		int			_SmashingPositionIndex{0};

		float		_smashingElapsedTime{ 0 }; //스매싱 모드 진행 시간
		float		_smashingLimitTime{ 0 }; //스매싱 모드 제한 시간
	private:
		long double _elapsedPosition{};
		long double _noteSpawnOffset{};
		long double _notePerfectOffset{};
		long double _noteGreatOffset{};
		long double _noteGoodOffset{};

	private:
		int _chargePerfectMana{};
		int _chargeGreatMana{};
		int _chargeGoodMana{};

	private:
		_uint _keyboardHit{ DIK_RETURN };
		_uint _gamePadHit{ DIP_B };

	private:
		bool _isPerfect{ false };
		bool _isGreat{ false };
		bool _isGood{ false };
		bool _isMiss{ false };
		bool _isSmashingMode{ false };
		bool _isNormalMode{ false };
		bool _isSmashing{ false };
		bool _isEnd{ false };
	};

	template<typename T>
	inline void NoteManager::AddSmashSuccessEvent(T* _this, void(T::* _event)())
	{
		_smashSuccessEvent.AddDynamic(_this, _event);
	}

	template<typename T>
	inline void NoteManager::AddSmashFailEvent(T* _this, void(T::* _event)())
	{
		_smashFailureEvent.AddDynamic(_this, _event);
	}
}