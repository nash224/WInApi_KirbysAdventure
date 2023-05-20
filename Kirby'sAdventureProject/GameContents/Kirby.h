#pragma once
#include "ActorUtils.h"

enum class KirbyMode
{
	Basic,
	Fat,
	Max,
};

enum class KirbyState
{
	Idle,
	Run,
	Jump,
	Max,
};

enum class KirbyDir
{
	Right,
	Left,
	Max,
};

class Kirby : public ActorUtils
{
public:
	// constructor desstructor
	Kirby();
	~Kirby();

	// delete Function
	Kirby(const Kirby& _Other) = delete;
	Kirby(Kirby&& _Other) noexcept = delete;
	Kirby& operator=(const Kirby& _Other) = delete;
	Kirby& operator=(Kirby&& _Other) noexcept = delete;

	GameEngineRenderer* MainRenderer = nullptr;


	static Kirby* GetMainKirby()
	{
		return MainKirby;
	}

protected:
	static Kirby* MainKirby;



	void StateUpdate(float _Delta);
	void IdleStart();
	void RunStart();
	void JumpStart();

	void IdleUpdate(float _Delta);
	void RunUpdate(float _Delta);
	void JumpUpdate(float _Delta);

	void DirCheck();
	void ChangeState(KirbyState State);
	void ChangeAnimationState(const std::string& _StateName);
	float4 GetKirbyScale();

private:
	KirbyMode Mode = KirbyMode::Max;
	KirbyState State = KirbyState::Max;
	KirbyDir Dir = KirbyDir::Right;
	std::string CurState = "";

	bool IsJump = false;

	void Start() override;
	void Update(float _Delta) override;

	void LevelStart() override;
};