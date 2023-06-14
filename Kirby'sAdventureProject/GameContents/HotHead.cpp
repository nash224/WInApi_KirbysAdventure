#include "HotHead.h"
#include "ContentsEnum.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/ResourcesManager.h>

#include "GlobalContents.h"
#include "Kirby.h"
#include "FrameBreathEffect.h"
#include "FireBallEffect.h"
#include <vector>

HotHead::HotHead()
{
}

HotHead::~HotHead()
{
}


void HotHead::Start()
{
	MainRenderer = CreateRenderer(RenderOrder::Play);

	GlobalContents::SpriteFileLoad("Left_PowerEnemy.bmp", "Resources\\Unit\\Grunt", 6, 5);
	GlobalContents::SpriteFileLoad("Right_PowerEnemy.bmp", "Resources\\Unit\\Grunt", 6, 5);

	MainRenderer->CreateAnimation("Left_Walk", "Left_PowerEnemy.bmp", 6, 7, HOTHEADWALKINGCHANGEANIMATIONTIME, true);
	MainRenderer->CreateAnimation("Right_Walk", "Right_PowerEnemy.bmp", 6, 7, HOTHEADWALKINGCHANGEANIMATIONTIME, true);

	MainRenderer->CreateAnimation("Left_FireBallCharging", "Left_PowerEnemy.bmp", 8, 9, HOTHEADWOBBLETIME, true);
	MainRenderer->CreateAnimation("Right_FireBallCharging", "Right_PowerEnemy.bmp", 8, 9, HOTHEADWOBBLETIME, true);

	MainRenderer->CreateAnimation("Left_FireBall", "Left_PowerEnemy.bmp", 10, 11, HOTHEADWOBBLETIME, true);
	MainRenderer->CreateAnimation("Right_FireBall", "Right_PowerEnemy.bmp", 10, 11, HOTHEADWOBBLETIME, true);

	MainRenderer->CreateAnimation("Left_FlameBreathCharging", "Left_PowerEnemy.bmp", 8, 9, HOTHEADWOBBLETIME, true);
	MainRenderer->CreateAnimation("Right_FlameBreathCharging", "Right_PowerEnemy.bmp", 8, 9, HOTHEADWOBBLETIME, true);

	MainRenderer->CreateAnimation("Left_FlameBreath", "Left_PowerEnemy.bmp", 10, 11, HOTHEADWOBBLETIME, true);
	MainRenderer->CreateAnimation("Right_FlameBreath", "Right_PowerEnemy.bmp", 10, 11, HOTHEADWOBBLETIME, true);


	MainRenderer->SetRenderScaleToTexture();
	MainRenderer->SetScaleRatio(3.0f);

	Scale = float4{ 24.0f, 39.0f };
	SetCheckPoint(Scale);

	Dir = ActorDir::Left;


	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	BodyCollision->SetCollisionPos(float4{ 0.0f , -SMALLTYPECOLLISIONSCALE.hY() });
	BodyCollision->SetCollisionScale(SMALLTYPECOLLISIONSCALE);
	BodyCollision->SetCollisionType(CollisionType::Rect);
}

void HotHead::init(const std::string& _FileName, HotHeadState _State, const float4& _Pos)
{
	Attribute = AttributeType::Fire;
	Ability = AbilityStar::Fire;

	SetGroundTexture(_FileName);
	RespawnLocation = _Pos;
	RespawnState = _State;
	SetPos(RespawnLocation);
	ChangeState(_State);
	StringRespawnState = CurState;
}



/* 天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天 */

void HotHead::Update(float _Delta)
{
	GroundCheck();

	if (true == IsInhaleCollision())
	{
		ChangeState(HotHeadState::Inhaled);
		return;
	}


	StateUpdate(_Delta);

	CheckOverScreen();
}

void HotHead::StateUpdate(float _Delta)
{
	switch (State)
	{
	case HotHeadState::Walk:					return WalkUpdate(_Delta);
	case HotHeadState::FireBallCharging:		return FireBallChargingUpdate(_Delta);
	case HotHeadState::FireBall:				return FireBallUpdate(_Delta);
	case HotHeadState::FlameBreathCharging:		return FlameBreathChargingUpdate(_Delta);
	case HotHeadState::FlameBreath:				return FlameBreathUpdate(_Delta);
	case HotHeadState::Inhaled:					return InhaledUpdate(_Delta);
	default:
		break;
	}
}

void HotHead::ChangeState(HotHeadState _State)
{
	if (_State != State || _State == RespawnState)
	{
		switch (_State)
		{
		case HotHeadState::Walk:					WalkStart();					break;
		case HotHeadState::FireBallCharging:		FireBallChargingStart();		break;
		case HotHeadState::FireBall:				FireBallStart();				break;
		case HotHeadState::FlameBreathCharging:		FlameBreathChargingStart();		break;
		case HotHeadState::FlameBreath:				FlameBreathStart();				break;
		case HotHeadState::Inhaled:					InhaledStart();					break;
		default:
			break;
		}
	}

	State = _State;
}

void HotHead::ChangeRespawnState()
{
	ChangeState(RespawnState);
}

/* 天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天天 */


void HotHead::WalkStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	GravityReset();
	GetKirbyDirection();
	ChangeAnimationState("Walk");
}

void HotHead::WalkUpdate(float _Delta)
{
	StateTime += _Delta;

	if (StateTime > HOTHEADWALKINGCHANGESTATETIME)
	{
		StateTime = 0.0f;

		if(HOTHEADFIREBALLRANGEDETECTION > abs(Kirby::GetMainKirby()->GetPos().X - GetPos().X))
		{
			ChangeState(HotHeadState::FlameBreathCharging);
			return;
		}
		else if (HOTHEADRANGEDETECTION > abs(Kirby::GetMainKirby()->GetPos().X - GetPos().X))
		{
			ChangeState(HotHeadState::FireBallCharging);
			return;
		}
	}

	if (true == CheckLeftWall() || true == LeftGroundIsCliff())
	{
		Dir = ActorDir::Right;
		MainRenderer->ChangeAnimation("Right_Walk");
	}
	else if (true == CheckRightWall() || true == RightGroundIsCliff())
	{
		Dir = ActorDir::Left;
		MainRenderer->ChangeAnimation("Left_Walk");
	}

	if (ActorDir::Left == Dir)
	{
		CurrentSpeed = -HOTHEADSPEED;
	}
	else if (ActorDir::Right == Dir)
	{
		CurrentSpeed = HOTHEADSPEED;
	}


	BlockedByGround();
	BlockedByWall();

	if (false == GetGroundState())
	{
		Gravity(_Delta);
	}
	VerticalUpdate(_Delta);
	
	HorizontalUpdate(_Delta);
}


void HotHead::FireBallChargingStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	GetKirbyDirection();
	ChangeAnimationState("FireBall");
}

void HotHead::FireBallChargingUpdate(float _Delta)
{
	StateTime += _Delta;

	if (StateTime > HOTHEADFIREBALLCHARGINGTIME)
	{
		IsChangeState = true;
	}

	if (true == IsChangeState)
	{
		ChangeState(HotHeadState::FireBall);
		return;
	}
}


void HotHead::FireBallStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	WobbleCount = 0;

	ActorDirUnitVector = GetDirUnitVector();

	FireBallEffect* FireBallEffectPtr = GetLevel()->CreateActor<FireBallEffect>();
	FireBallEffectPtr->init(GetPos(), Scale, ActorDirUnitVector);
	FireBallEffectPtr->SetActorCollision(CollisionOrder::MonsterAbility, CollisionType::Rect);

	ChangeAnimationState("FireBall");
}

void HotHead::FireBallUpdate(float _Delta)
{
	StateTime += _Delta;

	if (StateTime > HOTHEADWOBBLETIME)
	{
		StateTime = 0.0f;
		++WobbleCount;

		if (ActorDir::Left == Dir)
		{
			if (1 == WobbleCount % 3)
			{
				AddPos(float4::LEFT * 6.0f);
			}
			else if (2 == WobbleCount % 3)
			{
				AddPos(float4::RIGHT * 4.0f);
			}
			else if (0 == WobbleCount % 3)
			{
				AddPos(float4::RIGHT * 2.0f);
			}
		}
		else if (ActorDir::Right == Dir)
		{
			if (1 == WobbleCount % 3)
			{
				AddPos(float4::RIGHT * 6.0f);
			}
			else if (2 == WobbleCount % 3)
			{
				AddPos(float4::LEFT * 4.0f);
			}
			else if (0 == WobbleCount % 3)
			{
				AddPos(float4::LEFT * 2.0f);
			}
		}
	}

	if (9 == WobbleCount)
	{
		IsChangeState = true;
	}

	if (true == IsChangeState)
	{
		ActorDirUnitVector = float4::ZERO;
		ChangeState(HotHeadState::Walk);
		return;
	}
}

void HotHead::FlameBreathChargingStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	GetKirbyDirection();
	ChangeAnimationState("FlameBreathCharging");
}

void HotHead::FlameBreathChargingUpdate(float _Delta)
{
	StateTime += _Delta;

	if (StateTime > HOTHEADFLAMEBREATHCHARGINGTIME)
	{
		IsChangeState = true;
	}

	if (true == IsChangeState)
	{
		ChangeState(HotHeadState::FlameBreath);
		return;
	}
}


void HotHead::FlameBreathStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	WobbleCount = 0;
	ActorDirUnitVector = GetDirUnitVector();
	ChangeAnimationState("FlameBreath");
}

void HotHead::FlameBreathUpdate(float _Delta)
{
	StateTime += _Delta;

	if (StateTime > HOTHEADWOBBLETIME)
	{
		StateTime = 0.0f;
		++WobbleCount;

		FrameBreathEffect* FrameBreathEffectPtr = GetLevel()->CreateActor<FrameBreathEffect>();
		FrameBreathEffectPtr->init(GetPos(), Scale, ActorDirUnitVector);
		FrameBreathEffectPtr->SetActorCollision(CollisionOrder::MonsterAbility, CollisionType::Rect);


		if (ActorDir::Left == Dir)
		{
			if (1 == WobbleCount % 3)
			{
				AddPos(float4::LEFT * 6.0f);
			}
			else if (2 == WobbleCount % 3)
			{
				AddPos(float4::RIGHT * 4.0f);
			}
			else if (0 == WobbleCount % 3)
			{
				AddPos(float4::RIGHT * 2.0f);
			}
		}
		else if (ActorDir::Right == Dir)
		{
			if (1 == WobbleCount % 3)
			{
				AddPos(float4::RIGHT * 6.0f);
			}
			else if (2 == WobbleCount % 3)
			{
				AddPos(float4::LEFT * 4.0f);
			}
			else if (0 == WobbleCount % 3)
			{
				AddPos(float4::LEFT * 2.0f);
			}
		}
	}

	if (45 == WobbleCount)
	{
		IsChangeState = true;
	}

	if (true == IsChangeState)
	{
		ActorDirUnitVector = float4::ZERO;
		ChangeState(HotHeadState::Walk);
		return;
	}
}


void HotHead::InhaledStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	BodyCollision->Off();
	ActorDirUnitVector = GetKirbyOpponentDistance();
	InhaleTargetPos = GetKirbyOpponentDistance();
	InhaleTargetPosYDistance = InhaleTargetPos.Y -15.0f;
	InhaleTargetPosXDistance = InhaleTargetPos.X;
	
	CurentVerticalSpeed = InhaleTargetPosYDistance;
	CurrentSpeed = 0.0f;
}

void HotHead::InhaledUpdate(float _Delta)
{
	StateTime += _Delta;

	float4 KirbyPos = Kirby::GetMainKirby()->GetPos();


	if (ActorDirUnitVector.X < 0.0f)
	{
		float InhaleXSpeed = InhaleTargetPosXDistance / INHALETIME * _Delta;
		CurrentSpeed += InhaleXSpeed;

		if (GetPos().X < KirbyPos.X)
		{
			BodyCollision->On();
			Off();
			return;
		}
	}
	else if (ActorDirUnitVector.X > 0.0f)
	{
		float InhaleXSpeed = InhaleTargetPosXDistance / INHALETIME * _Delta;
		CurrentSpeed += InhaleXSpeed;

		if (GetPos().X > KirbyPos.X)
		{
			BodyCollision->On();
			Off();
			return;
		}
	}


	VerticalUpdateBasedlevitation(_Delta);

	HorizontalUpdate(_Delta);
}