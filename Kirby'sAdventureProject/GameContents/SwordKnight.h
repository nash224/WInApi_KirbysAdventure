#pragma once
#include "PowerEnemies.h"

#define SWORDKNIGHTSPEED 80.0f
#define SWORDKNIGHTSLASHCOOLDOWN 2.0f

#define SWORDKNIGHTRANGEDETECTION 200.0f

#define SWORDKNIGHTRAISESWORDTIME 0.2f
#define SWORDKNIGHTSLASHINSTANTANEOUSSPEED 800.0f
#define SWORDKNIGHTSLASHTIME 0.35f
#define SWORDKNIGHTSLASHFRAMETIME 0.05f

#define SWORDKNIGHTUNDERHANDTIME SWORDKNIGHTRAISESWORDTIME
#define SWORDKNIGHTUNDERHANDINSTANTANEOUSSPEED 600.0f
#define SWORDKNIGHTUNDERSLASHTIME SWORDKNIGHTSLASHTIME
#define SWORDKNIGHTUNDERSLASHFRAMETIME SWORDKNIGHTSLASHFRAMETIME


#define SWORDKNIGHTDEACELECTIONSPEED SWORDKNIGHTSLASHINSTANTANEOUSSPEED / SWORDKNIGHTSLASHTIME


enum class SwordKnightState
{
	PendulumStride,
	RaiseSword,
	Slash,
	Underhand,
	ReversingSlash,
	BeInhaled,
	Hitted,
	Max,
};

// 설명 : 검을 들고 항상 커비를 노리고 있는 파워몹 검사입니다.
class SwordKnight : public PowerEnemies
{
public:
	// constrcuter destructer
	SwordKnight();
	~SwordKnight();

	// delete Function
	SwordKnight(const SwordKnight& _Other) = delete;
	SwordKnight(SwordKnight&& _Other) noexcept = delete;
	SwordKnight& operator=(const SwordKnight& _Other) = delete;
	SwordKnight& operator=(SwordKnight&& _Other) noexcept = delete;

	void init(const std::string& _FileName, SwordKnightState _State, const float4& _Pos);

protected:


private:
	// GameEngineObject 상속
	void Start() override;
	void Update(float _Delta) override;
	void Render(float _Delta) override;


	// Enemy 상속
	void ChangeRespawnState() override;



	// this
	// 상태패턴 함수
	SwordKnightState State = SwordKnightState::Max;
	SwordKnightState RespawnState = SwordKnightState::Max;

	void StateUpdate(float _Delta);
	void ChangeState(SwordKnightState _State);



	void PendulumStrideStart();
	void PendulumStrideUpdate(float _Delta);

	float AbilityCoolDown = 0.0f;


	void RaiseSwordStart();
	void RaiseSwordUpdate(float _Delta);

	void SlashStart();
	void SlashUpdate(float _Delta);

	void UnderhandStart();
	void UnderhandUpdate(float _Delta);

	void ReversingSlashStart();
	void ReversingSlashUpdate(float _Delta);



	// 충돌
	void EnemyCollisionCheck();




	// 디버깅
	void ThisDebugTriggerRender(HDC _dc);
	void ThisDebugRender(HDC _dc, int& _RenderNumber, const int _TextXPos, const int _TextYPos);
};

