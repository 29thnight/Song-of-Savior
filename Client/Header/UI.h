#pragma once
#include <Actor.h>

namespace Engine
{
	CORECLASS();
}

namespace Client
{
	class UI : public Engine::Actor
	{
	friend class UIEditerManager;
	friend class UIManager;
	protected:
		explicit UI() = default;
		virtual ~UI() = default;

	public:
		void BeginPlay() override;
		void Tick(_duration deltaSeconds) override;
		void Fixed() override;
		void EndPlay() override;

	public:
		static UI* Create() { return new UI; }
		void Destroy() override;

	public:
		void ReloadTextures();

	public:
		template <typename T>
		void AddEnterEvent(T* _this, void(T::* _event)()); //버튼 누를때 실행할 함수

		void UnbindEnterEvent(); //버튼 누를때 실행할 함수

	public:
		bool _isFocus{false};

	public:
		UI& operator=(UI&& rhs) noexcept;

	protected:
		Engine::BitmapComponent* _pPrimaryBitmap{ nullptr };
		Engine::BitmapComponent* _pSecondaryBitmap{ nullptr };
		//Engine::BoxComponent*	 _pBoxComponent{ nullptr };

	private:
		Delegate<> _onEnterEvent;
		void CallEnterEvent(); //연결된 이벤트 함수 호출
	};

	template<typename T>
	inline void UI::AddEnterEvent(T* _this, void(T::* _event)())
	{
		_onEnterEvent.AddDynamic(_this, _event);
	}
}
