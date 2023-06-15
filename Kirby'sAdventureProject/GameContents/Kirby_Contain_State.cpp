#include "Kirby.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/ResourcesManager.h>

#include "GlobalContents.h"
#include "DustEffect.h"
#include "HitObjectEffect.h"
#include "SmallStarFireEffect.h"
#include "LargeStarFireEffect.h"
#include "GetAbilityEffect.h"



void Kirby::Contain_StateResourceLoad()
{
	GlobalContents::SpriteFileLoad("Contain_Left_Kirby.bmp", "Resources\\Unit\\Kirby", 10, 2);
	GlobalContents::SpriteFileLoad("Contain_Right_Kirby.bmp", "Resources\\Unit\\Kirby", 10, 2);

	MainRenderer->CreateAnimation("Normal_Left_Contain_Idle", "Contain_Left_Kirby.bmp", 0, 0, 0.1f, false);
	MainRenderer->CreateAnimation("Normal_Right_Contain_Idle", "Contain_Right_Kirby.bmp", 0, 0, 0.1f, false);

	MainRenderer->CreateAnimation("Normal_Left_Contain_Walk", "Contain_Left_Kirby.bmp", 1, 4, 0.2f, true);
	MainRenderer->CreateAnimation("Normal_Right_Contain_Walk", "Contain_Right_Kirby.bmp", 1, 4, 0.2f, true);

	MainRenderer->CreateAnimation("Normal_Left_Contain_Run", "Contain_Left_Kirby.bmp", 1, 4, 0.1f, true);
	MainRenderer->CreateAnimation("Normal_Right_Contain_Run", "Contain_Right_Kirby.bmp", 1, 4, 0.1f, true);

	MainRenderer->CreateAnimation("Normal_Left_Contain_Turn", "Contain_Left_Kirby.bmp", 1, 1, 0.1f, false);
	MainRenderer->CreateAnimation("Normal_Right_Contain_Turn", "Contain_Right_Kirby.bmp", 1, 1, 0.1f, false);

	MainRenderer->CreateAnimation("Normal_Left_Contain_Jump", "Contain_Left_Kirby.bmp", 1, 2, 0.1f, false);
	MainRenderer->CreateAnimation("Normal_Right_Contain_Jump", "Contain_Right_Kirby.bmp", 1, 2, 0.1f, false);

	MainRenderer->CreateAnimation("Normal_Left_Contain_Fall", "Contain_Left_Kirby.bmp", 2, 2, 0.1f, false);
	MainRenderer->CreateAnimation("Normal_Right_Contain_Fall", "Contain_Right_Kirby.bmp", 2, 2, 0.1f, false);

	MainRenderer->CreateAnimation("Normal_Left_Contain_Gulp", "Contain_Left_Kirby.bmp", 9, 11, 0.2f, false);
	MainRenderer->CreateAnimation("Normal_Right_Contain_Gulp", "Contain_Right_Kirby.bmp", 9, 11, 0.2f, false);

	MainRenderer->CreateAnimation("Normal_Left_Contain_Disgorge", "Contain_Left_Kirby.bmp", 5, 8, 0.08f, false);
	MainRenderer->CreateAnimation("Normal_Right_Contain_Disgorge", "Contain_Right_Kirby.bmp", 5, 8, 0.08f, false);

	MainRenderer->FindAnimation("Normal_Left_Contain_Walk")->Inters = { 0.2f , 0.3f , 0.2f , 0.3f };
	MainRenderer->FindAnimation("Normal_Right_Contain_Walk")->Inters = { 0.2f , 0.3f , 0.2f , 0.3f };

	MainRenderer->FindAnimation("Normal_Left_Contain_Run")->Inters = { 0.12f , 0.2f , 0.12f , 0.2f };
	MainRenderer->FindAnimation("Normal_Right_Contain_Run")->Inters = { 0.12f , 0.2f , 0.12f , 0.2f };
}

void Kirby::Contain_IdleStart()
{
	StateTime = 0.0f;
	ChangeKirbyBodyState(KirbyBodyState::Fat);
	ChangeAnimationState("Contain_Idle");
}

void Kirby::Contain_IdleUpdate(float _Delta)
{
	if (false == GetGroundState())
	{
		ChangeState(KirbyState::Contain_Fall);
		return;
	}
	if (true == GameEngineInput::IsPress('S'))
	{
		ChangeState(KirbyState::Contain_Gulp);
		return;
	}
	if (true == (GameEngineInput::IsDown('Z')))
	{
		ChangeState(KirbyState::Contain_Disgorge);
		return;
	}
	if (true == (GameEngineInput::IsDown('X')))
	{
		ChangeState(KirbyState::Contain_Jump);
		return;
	}

	if (true == CheckLeftWall() && GameEngineInput::IsPress('A') && CurrentSpeed == 0.0f)
	{
		return;
	}
	if (true == CheckRightWall() && GameEngineInput::IsPress('D') && CurrentSpeed == 0.0f)
	{
		return;
	}

	if (true == (GameEngineInput::IsPress('A') || GameEngineInput::IsPress('D')) && false == (GameEngineInput::IsPress('A') && GameEngineInput::IsPress('D')))
	{
		ChangeState(KirbyState::Contain_Walk);
		return;
	}

	BlockedByGround();
	BlockedByWall();

	DecelerationUpdate(_Delta);
	HorizontalUpdate(_Delta);
}


void Kirby::Contain_WalkStart()
{
	StateTime = 0.0f;
	KirbyDirCheck();
	ChangeAnimationState("Contain_Walk");
}

void Kirby::Contain_WalkUpdate(float _Delta)
{
	if (false == GetGroundState())
	{
		GravityReset();
		ChangeState(KirbyState::Contain_Fall);
		return;
	}
	if (true == GameEngineInput::IsPress('S'))
	{
		ChangeState(KirbyState::Contain_Gulp);
		return;
	}

	if (CurrentSpeed < WALKMAXSPEED * 0.2f && true == GameEngineInput::IsDown('A') && false == (GameEngineInput::IsPress('A') && GameEngineInput::IsPress('D')))
	{
		ChangeState(KirbyState::Contain_Run);
		return;
	}

	if (CurrentSpeed > WALKMAXSPEED * 0.2f && true == GameEngineInput::IsDown('D') && false == (GameEngineInput::IsPress('A') && GameEngineInput::IsPress('D')))
	{
		ChangeState(KirbyState::Contain_Run);
		return;
	}

	if (true == GameEngineInput::IsDown('X'))
	{
		ChangeState(KirbyState::Contain_Jump);
		return;
	}
	if (true == GameEngineInput::IsDown('Z'))
	{
		ChangeState(KirbyState::Contain_Disgorge);
		return;
	}

	if (CurrentSpeed > 0.0f && true == GameEngineInput::IsPress('A') && false == (GameEngineInput::IsPress('A') && GameEngineInput::IsPress('D')))
	{
		ChangeState(KirbyState::Contain_Turn);
		return;
	}

	if (CurrentSpeed < 0.0f && true == GameEngineInput::IsPress('D') && false == (GameEngineInput::IsPress('A') && GameEngineInput::IsPress('D')))
	{
		ChangeState(KirbyState::Contain_Turn);
		return;
	}

	if (CurrentSpeed == 0.0f &&
		((GameEngineInput::IsPress('A') && GameEngineInput::IsPress('D')) ||
			(GameEngineInput::IsFree('A') && GameEngineInput::IsFree('D'))))
	{
		ChangeState(KirbyState::Contain_Idle);
		return;
	}

	if (true == CheckLeftWallBasedSpeed())
	{
		HitObjectEffect* HitObjectEffectPtr = GetLevel()->CreateActor<HitObjectEffect>();
		HitObjectEffectPtr->init(GetPos(), float4::ZERO);

		CurrentSpeed = 0.0f;
		ChangeState(KirbyState::Contain_Idle);
		return;
	}
	if (true == CheckRightWallBasedSpeed())
	{
		HitObjectEffect* HitObjectEffectPtr = GetLevel()->CreateActor<HitObjectEffect>();
		HitObjectEffectPtr->init(GetPos(), float4::ZERO);

		CurrentSpeed = 0.0f;
		ChangeState(KirbyState::Contain_Idle);
		return;
	}


	BlockedByGround();
	MoveHorizontal(WALKSPEED, _Delta);


	BlockedByWall();

	DecelerationUpdate(_Delta);
	HorizontalUpdate(_Delta);
}


void Kirby::Contain_RunStart()
{
	StateTime = 0.0f;

	DustEffect* DustEffectPtr = GetLevel()->CreateActor<DustEffect>();
	DustEffectPtr->init(GetPos(), GetKirbyScale(), -GetDirUnitVector());
	ChangeAnimationState("Contain_Run");
}

void Kirby::Contain_RunUpdate(float _Delta)
{
	if (false == GetGroundState())
	{
		GravityReset();
		ChangeState(KirbyState::Contain_Fall);
		return;
	}
	if (true == GameEngineInput::IsPress('S'))
	{
		ChangeState(KirbyState::Contain_Gulp);
		return;
	}
	if (true == GameEngineInput::IsDown('X'))
	{
		ChangeState(KirbyState::Contain_Jump);
		return;
	}
	if (true == GameEngineInput::IsDown('Z'))
	{
		ChangeState(KirbyState::Contain_Disgorge);
		return;
	}

	if (CurrentSpeed > 0.0f && true == GameEngineInput::IsPress('A') && false == (GameEngineInput::IsPress('A') && GameEngineInput::IsPress('D')))
	{
		ChangeState(KirbyState::Contain_Turn);
		return;
	}

	if (CurrentSpeed < 0.0f && true == GameEngineInput::IsPress('D') && false == (GameEngineInput::IsPress('A') && GameEngineInput::IsPress('D')))
	{
		ChangeState(KirbyState::Contain_Turn);
		return;
	}

	if (CurrentSpeed == 0.0f &&
		((GameEngineInput::IsPress('A') && GameEngineInput::IsPress('D')) ||
			(GameEngineInput::IsFree('A') && GameEngineInput::IsFree('D'))))
	{
		ChangeState(KirbyState::Contain_Idle);
		return;
	}

	if (true == CheckLeftWallBasedSpeed())
	{
		HitObjectEffect* HitObjectEffectPtr = GetLevel()->CreateActor<HitObjectEffect>();
		HitObjectEffectPtr->init(GetPos(), float4::ZERO);
		CurrentSpeed = 0.0f;

		ChangeState(KirbyState::Contain_Idle);
		return;
	}

	if (true == CheckRightWallBasedSpeed())
	{
		HitObjectEffect* HitObjectEffectPtr = GetLevel()->CreateActor<HitObjectEffect>();
		HitObjectEffectPtr->init(GetPos(), float4::ZERO);
		CurrentSpeed = 0.0f;

		ChangeState(KirbyState::Contain_Idle);
		return;
	}

	BlockedByGround();
	MoveHorizontal(RUNSPEED, _Delta);

	BlockedByWall();

	DecelerationUpdate(_Delta);
	HorizontalUpdate(_Delta);
}


void Kirby::Contain_TurnStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	if (CurrentSpeed > 0.0f)
	{
		Dir = ActorDir::Right;
	}
	if (CurrentSpeed < 0.0f)
	{
		Dir = ActorDir::Left;
	}

	DustEffect* DustEffectPtr = GetLevel()->CreateActor<DustEffect>();
	DustEffectPtr->init(GetPos(), GetKirbyScale(), GetDirUnitVector());

	ChangeAnimationState("Contain_Turn");
}

void Kirby::Contain_TurnUpdate(float _Delta)
{
	if (CurrentSpeed == 0.0f)
	{
		IsChangeState = true;
	}

	if (true == GetGroundState() && true == IsChangeState)
	{
		ChangeState(KirbyState::Contain_Idle);
		return;
	}

	if (false == GetGroundState())
	{
		GravityReset();
		ChangeState(KirbyState::Contain_Fall);
		return;
	}

	if (true == CheckLeftWallBasedSpeed())
	{
		// 별 효과
		CurrentSpeed = 0.0f;
	}
	if (true == CheckLeftWallBasedSpeed())
	{
		// 별 효과
		CurrentSpeed = 0.0f;
	}


	BlockedByGround();
	BlockedByWall();

	ActorUtils::DecelerationUpdate(_Delta, BRAKESPEED);
	HorizontalUpdate(_Delta);
}


void Kirby::Contain_JumpStart()
{
	StateTime = 0.0f;
	AbleJump = true;
	IsChangeState = false;
	CurrentJumpDistance = 0.0f;
	KirbyDirCheck();
	GravityReset();
	ChangeAnimationState("Contain_Jump");
}

void Kirby::Contain_JumpUpdate(float _Delta)
{

	StateTime += _Delta;

	if (true == GameEngineInput::IsFree('X'))
	{
		IsChangeState = true;
	}


	if (true == GameEngineInput::IsDown('Z'))
	{
		ChangeState(KirbyState::Contain_Disgorge);
		return;
	}

	if (IsChangeState = true && 0.0f < GetGravityVector().Y)
	{
		ChangeState(KirbyState::Contain_Fall);
		return;
	}

	if (true == CeilingCheck())
	{
		// 별 효과
		GravityReset();
	}

	if (true == GameEngineInput::IsUp('X'))
	{
		AbleJump = false;
	}

	float JumpPower = JUMPMAXDISTANCE / JUMPTIME;
	CurrentJumpDistance += JumpPower * _Delta;

	if (true == GameEngineInput::IsUp('X') || CurrentJumpDistance > JUMPMAXDISTANCE)
	{
		AbleJump = false;
	}

	if (true == GameEngineInput::IsPress('X') && CurrentJumpDistance < JUMPMAXDISTANCE && true == AbleJump)
	{
		SetGravityVector(float4::UP * JumpPower);
	}



	BlockedByCeiling();
	BlockedByGround();
	MoveHorizontal(WALKSPEED, _Delta);
	BlockedByWall();


	ChangeAnimationState("Contain_Jump");

	DecelerationUpdate(_Delta);
	HorizontalUpdate(_Delta);

	Gravity(_Delta);
	GravityLimit(_Delta);
	VerticalUpdate(_Delta);
}


void Kirby::Contain_FallStart()
{
	StateTime = 0.0f;
	ChangeAnimationState("Contain_Fall");
}

void Kirby::Contain_FallUpdate(float _Delta)
{
	if (true == GameEngineInput::IsDown('Z'))
	{
		ChangeState(KirbyState::Contain_Disgorge);
		return;
	}

	if (true == GetGroundState() && CurrentSpeed != 0.0f)
	{

		HitObjectEffect* HitObjectEffectPtr = GetLevel()->CreateActor<HitObjectEffect>();
		HitObjectEffectPtr->init(GetPos(), float4::ZERO);

		ChangeState(KirbyState::Contain_Walk);
		return;
	}

	if (true == GetGroundState() && CurrentSpeed == 0.0f)
	{

		HitObjectEffect* HitObjectEffectPtr = GetLevel()->CreateActor<HitObjectEffect>();
		HitObjectEffectPtr->init(GetPos(), float4::ZERO);

		ChangeState(KirbyState::Contain_Idle);
		return;
	}

	if (true == CheckLeftWallBasedSpeed())
	{
		// 별효과
		CurrentSpeed = 0.0f;
	}
	if (true == CheckRightWallBasedSpeed())
	{
		// 별효과
		CurrentSpeed = 0.0f;
	}


	BlockedByGround();
	MoveHorizontal(WALKSPEED, _Delta);
	BlockedByWall();
	ChangeAnimationState("Contain_Fall");

	DecelerationUpdate(_Delta);
	HorizontalUpdate(_Delta);

	Gravity(_Delta);
	GravityLimit(_Delta);
	VerticalUpdate(_Delta);
}


void Kirby::Contain_GulpStart()
{
	StateTime = 0.0f;
	ChangeKirbyBodyState(KirbyBodyState::Little);

	if (Star.SwallowedPowerEnemyNumber > 0)
	{
		GetAbilityEffectPtr = GetLevel()->CreateActor<GetAbilityEffect>(UpdateOrder::PlayerAbility);
		GetAbilityEffectPtr->init(GetPos(), GetKirbyScale());
	}

	ChangeAnimationState("Contain_Gulp");
}

void Kirby::Contain_GulpUpdate(float _Delta)
{
	StateTime += _Delta;
	if (true == MainRenderer->IsAnimationEnd())
	{
		IsChangeState = true;
	}

	if (true == IsChangeState && AbilityStar::Max == CurrentAbilityStar && true == GameEngineInput::IsPress('S'))
	{
		ChangeState(KirbyState::LowerPosture);
		return;
	}

	if (true == IsChangeState && AbilityStar::Max == CurrentAbilityStar && false == GameEngineInput::IsPress('S'))
	{
		if (CurrentSpeed == 0.0f)
		{
			ChangeState(KirbyState::Idle);
			return;
		}
		if (CurrentSpeed != 0.0f)
		{
			ChangeState(KirbyState::Walk);
			return;
		}
	}

	if (true == IsChangeState && AbilityStar::Max != CurrentAbilityStar)
	{
		ChangeState(KirbyState::GetAbility);
		return;
	}

	if (true == CheckLeftWallBasedSpeed())
	{
		HitObjectEffect* HitObjectEffectPtr = GetLevel()->CreateActor<HitObjectEffect>();
		HitObjectEffectPtr->init(GetPos(), float4::ZERO);

		CurrentSpeed = 0.0f;
	}
	if (true == CheckRightWallBasedSpeed())
	{
		HitObjectEffect* HitObjectEffectPtr = GetLevel()->CreateActor<HitObjectEffect>();
		HitObjectEffectPtr->init(GetPos(), float4::ZERO);

		CurrentSpeed = 0.0f;
	}


	BlockedByGround();
	BlockedByWall();

	DecelerationUpdate(_Delta);
	HorizontalUpdate(_Delta);
}


void Kirby::Contain_DisgorgeStart()
{
	StateTime = 0.0f;
	ChangeKirbyBodyState(KirbyBodyState::Little);
	StarAttack();
	ChangeAnimationState("Contain_Disgorge");
}

void Kirby::Contain_DisgorgeUpdate(float _Delta)
{
	IsChangeState = MainRenderer->IsAnimationEnd();

	if (true == IsChangeState && false == GetGroundState())
	{
		ChangeState(KirbyState::Fall);
		return;
	}

	if (true == IsChangeState && true == GetGroundState())
	{
		ChangeState(KirbyState::Idle);
		return;
	}




	BlockedByGround();
	MoveHorizontal(WALKSPEED, _Delta);
	BlockedByWall();

	DecelerationUpdate(_Delta);
	HorizontalUpdate(_Delta);

	if (false == GetGroundState())
	{
		Gravity(_Delta);
		GravityLimit(_Delta);
		VerticalUpdate(_Delta);
	}
}


///////////////////////////////////

void Kirby::StarAttack()
{
	float4 UnitDir = GetDirUnitVector();

	if (Star.SwallowedEnemyNumber >= 2)
	{
		// 큰별
		LargeStarFireEffect* LargeStarEffect = GetLevel()->CreateActor<LargeStarFireEffect>(UpdateOrder::PlayerAbility);
		LargeStarEffect->init(GetPos(), GetKirbyScale(), GetDirUnitVector());
		LargeStarEffect->SetActorCollision(CollisionOrder::PlayerAbility, CollisionType::Rect);
	}
	else if (1 == Star.SwallowedEnemyNumber)
	{
		// 작은별
		SmallStarFireEffect* StarStarEffect = GetLevel()->CreateActor<SmallStarFireEffect>(UpdateOrder::PlayerAbility);
		StarStarEffect->init(CurrentLevelBitMapFileName, GetPos(), GetKirbyScale(), GetDirUnitVector());
		StarStarEffect->SetActorCollision(CollisionOrder::PlayerAbility, CollisionType::Rect);
	}
} 



void Kirby::GetAbilityStart()
{
	StateTime = 0.0f;
	IsChangeState = false;

	if (1 == Star.SwallowedPowerEnemyNumber)
	{
		Mode = CurrentAbilityStar;
	}
	else if (Star.SwallowedPowerEnemyNumber > 1)
	{
		// 랜덤 모드로 대체될 예정
		Mode = CurrentAbilityStar;
	}
	else
	{
		MsgBoxAssert("삼킨 파워몬스터가 없는데 능력을 얻을려고 했습니다.");
		return;
	}

	CurrentAbilityStar = AbilityStar::Max;
	Star.SwallowedEnemyNumber = 0;
	Star.SwallowedPowerEnemyNumber = 0;

	ChangeAnimationState("GetAbility"); 
}

void Kirby::GetAbilityUpdate(float _Delta)
{
	StateTime += _Delta;

	if (StateTime > GetABilityStateEndTime)
	{
		IsChangeState = true;
	}

	if (true == GetGroundState() && true == IsChangeState && 0 == CurrentSpeed)
	{
		ChangeState(KirbyState::Idle);
		return;
	}

	if (true == GetGroundState() && true == IsChangeState && 0 != CurrentSpeed)
	{
		ChangeState(KirbyState::Walk);
		return;
	}

	if (false == GetGroundState() && true == IsChangeState)
	{
		ChangeState(KirbyState::Fall);
		return;
	}



	BlockedByGround();
	BlockedByCeiling();
	BlockedByWall();

	if (false == GetGroundState())
	{
		Gravity(_Delta);
		GravityLimit(_Delta);
		VerticalUpdate(_Delta);
	}

	Kirby::DecelerationUpdate(_Delta);
	HorizontalUpdate(_Delta);
}