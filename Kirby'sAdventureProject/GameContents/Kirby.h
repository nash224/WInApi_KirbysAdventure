#pragma once
#include "ActorUtils.h"

#define WALKMAXSPEED 300.0f
#define RUNMAXSPEED 425.0f
#define FLYMAXSPEED 200.0f

#define WALKSPEED 0.8f
#define RUNSPEED 1.2f
#define FLYSPEED 0.6f

#define JUMPPOWER 310.0f
#define FLYPOWER 280.0f

#define DECELERATIONSPEED 0.8f
#define BRAKESPEED 1.2f

#define HITTHEMAPTIME 0.08f
#define EXHALEATTACKTIME 0.08f
#define TAKEOFFTIME 0.4f

enum class KirbyBodyState
{
	Little,
	Fat,
	Lower,
	Max,
};

enum class KirbyState
{
	Idle,
	Walk,
	Run,
	Turn,
	Jump,
	AerialMotion,
	Fall,
	AccelerateDown,
	Bounce,
	Landing,
	LowerPosture,
	LowerAttack,
	HittheWall,
	HittheCeiling,
	TakeOff,
	Fly,
	ExhaleAttack,
	Max,
};

enum class KirbyMode
{
	Normal,
	Spark,
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

	KirbyMode Mode = KirbyMode::Max;


	void ChangeState(KirbyState State);
	void ChangeAnimationState(const std::string& _StateName);

	void DirCheck();
	void MoveHorizontal(float _Speed, float _Delta);
	void DecelerationUpdate(float _Delta);
	void DecelerationUpdate(float _Delta, float _Speed);
	void MoveUpdate(float _Delta);
	float4 GetKirbyScale();


	void StateUpdate(float _Delta);
	void IdleStart();
	void WalkStart();
	void RunStart();
	void TurnStart();
	void JumpStart();
	void AerialMotionStart();
	void FallStart();
	void AccelerateDownStart();
	void BounceStart();
	void LandingStart();
	void LowerPostureStart();
	void LowerAttackStart();
	void HittheWallStart();
	void HittheCeilingStart();
	void TakeOffStart();
	void FlyStart();
	void ExhaleAttackStart();


	void IdleUpdate(float _Delta);
	void WalkUpdate(float _Delta);
	void RunUpdate(float _Delta);
	void TurnUpdate(float _Delta);
	void JumpUpdate(float _Delta);
	void AerialMotionUpdate(float _Delta);
	void FallUpdate(float _Delta);
	void AccelerateDownUpdate(float _Delta);
	void BounceUpdate(float _Delta);
	void LandingUpdate(float _Delta);
	void LowerPostureUpdate(float _Delta);
	void LowerAttackUpdate(float _Delta);
	void HittheWallUpdate(float _Delta);
	void HittheCeilingUpdate(float _Delta);
	void TakeOffUpdate(float _Delta);
	void FlyUpdate(float _Delta);
	void ExhaleAttackUpdate(float _Delta);


private:
	KirbyBodyState BodyState = KirbyBodyState::Max;
	KirbyState State = KirbyState::Max;
	std::string CurState = "";

	bool IsChangeState = true;
	bool IsBounce = false;
	bool AbleJump = true;


	float StateTime = 0.0f;
	float MaxSpeed = 350.0f;
	float DecelerationSpeed = 1.0f;
	float CurrentJumpPower = 0.0f;
	float FallDistance = 0.0f;


	void Start() override;
	void Update(float _Delta) override;
	void Render(float _Detla) override;

	void LevelStart() override;
};