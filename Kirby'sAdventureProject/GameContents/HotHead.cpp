#include "HotHead.h"
#include "ContentsEnum.h"
#include "GlobalContents.h"


#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>


#include "VegetableValleyPlayLevel.h"
#include "FrameBreathEffect.h"
#include "FireBallEffect.h"
#include "Kirby.h"


HotHead::HotHead()
{
}

HotHead::~HotHead()
{
}


void HotHead::Start()
{
	MainRenderer = CreateRenderer(RenderOrder::Play);
	if (nullptr == MainRenderer)
	{
		MsgBoxAssert("렌더러를 생성하지 못했습니다.");
		return;
	}

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
	SetName("Hot Head");


	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	if (nullptr == BodyCollision)
	{
		MsgBoxAssert("바디 콜리전이 Null일리가 없어..");
		return;
	}
	BodyCollision->SetCollisionPos(float4{ 0.0f , -SmallTypeScale.hY()});
	BodyCollision->SetCollisionScale(SmallTypeScale);
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



/* ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ */


void HotHead::Update(float _Delta)
{
	GroundCheck();

	StateUpdate(_Delta);
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
	case HotHeadState::BeInhaled:				return BeInhaledUpdate(_Delta);
	case HotHeadState::Hitted:					return HittedUpdate(_Delta);
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
		case HotHeadState::BeInhaled:				BeInhaledStart();				break;
		case HotHeadState::Hitted:					HittedStart();					break;
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

	EnemyCollisionCheck();

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
		GravityLimit(_Delta);
		VerticalUpdate(_Delta);
	}
	
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

	EnemyCollisionCheck();
}


void HotHead::FireBallStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	WobbleCount = 0;

	float4 DistanceToKriby = GetKirbyOpponentDistance();
	float EffectDeg = DistanceToKriby.AngleDeg();

	// 각도 제한 
	if (DistanceToKriby.X < 0.0f)
	{
		if (EffectDeg < SemicircleAngle - FireBall_HighAngle)
		{
			EffectDeg = SemicircleAngle - FireBall_HighAngle;
		}
		else if (EffectDeg >= SemicircleAngle + FireBall_HighAngle)
		{
			EffectDeg = SemicircleAngle + FireBall_HighAngle;
		}
	}
	else if (DistanceToKriby.X >= 0.0f)
	{
		if (EffectDeg > FireBall_HighAngle && EffectDeg < 90.0f)
		{
			EffectDeg = FireBall_HighAngle;
		}
		else if (EffectDeg > 270.0f && EffectDeg <= 360.0f - FireBall_HighAngle)
		{
			EffectDeg = 360.0f - FireBall_HighAngle;
		}
	}




	GameEngineLevel* CurLevePtr = GetLevel();
	if (nullptr == CurLevePtr)
	{
		MsgBoxAssert("레벨을 불러오지 못했습니다.");
		return;
	}



	// 발사!
	FireBallEffect* FireBallEffectPtr = CurLevePtr->CreateActor<FireBallEffect>(UpdateOrder::Ability);
	if (nullptr == FireBallEffectPtr)
	{
		MsgBoxAssert("액터가 Null 입니다..");
		return;
	}


	FireBallEffectPtr->init(GetPos(), Scale, float4::GetUnitVectorFromDeg(EffectDeg));
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

		// 부들부들 떨음
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
		ChangeState(HotHeadState::Walk);
		return;
	}

	EnemyCollisionCheck();

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

	EnemyCollisionCheck();
}


void HotHead::FlameBreathStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	WobbleCount = 0;
	ChangeAnimationState("FlameBreath");
}

void HotHead::FlameBreathUpdate(float _Delta)
{
	StateTime += _Delta;

	// 부들부들 떨음
	if (StateTime > HOTHEADWOBBLETIME)
	{
		StateTime = 0.0f;
		++WobbleCount;


		GameEngineLevel* CurLevePtr = GetLevel();
		if (nullptr == CurLevePtr)
		{
			MsgBoxAssert("레벨을 불러오지 못했습니다.");
			return;
		}

		FrameBreathEffect* FrameBreathEffectPtr = CurLevePtr->CreateActor<FrameBreathEffect>(UpdateOrder::Ability);
		if (nullptr == FrameBreathEffectPtr)
		{
			MsgBoxAssert("액터를 생성하지 못했습니다.");
			return;
		}

		FrameBreathEffectPtr->init(GetPos(), Scale, GetDirUnitVector());
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
		ChangeState(HotHeadState::Walk);
		return;
	}

	EnemyCollisionCheck();
}





void HotHead::EnemyCollisionCheck()
{
	if (true == IsInhaledStateOn)
	{
		ChangeState(HotHeadState::BeInhaled);
		return;
	}

	if (true == IsHitted)
	{
		ChangeState(HotHeadState::Hitted);
		return;
	}
}





void HotHead::Render(float _Delta)
{
	if (false == VegetableValleyPlayLevel::Level_DebugRenderValue)
	{
		return;
	}


	GameEngineWindowTexture* BackBufferPtr = GameEngineWindow::MainWindow.GetBackBuffer();
	if (nullptr == BackBufferPtr)
	{
		MsgBoxAssert("백버퍼를 불러오지 못했습니다.");
		return;
	}

	HDC dc = BackBufferPtr->GetImageDC();
	if (nullptr == dc)
	{
		MsgBoxAssert("핸들을 불러오지 못했습니다.");
		return;
	}

	int TextRenderNum = 0;


	float4 ActorScenePos = ActorCameraPos();

	int TextXPos = ActorScenePos.iX() - Scale.Half().iX();
	int TextYPos = ActorScenePos.iY() - (Scale * 2.0f).iY();


	EnemyDebugRender(dc, TextRenderNum, TextXPos, TextYPos);
	ThisDebugRender(dc, TextRenderNum, TextXPos, TextYPos);

	ThisDebugTriggerRender(dc);
}



void HotHead::ThisDebugRender(HDC _dc, int& _RenderNumber, const int _TextXPos, const int _TextYPos)
{
	if (nullptr == _dc)
	{
		MsgBoxAssert("핸들을 불러오지 못했습니다.");
		return;
	}

	float4 DistanceToKriby = GetKirbyOpponentDistance();
	float EffectDeg = DistanceToKriby.AngleDeg();



	if (DistanceToKriby.X < 0.0f)
	{
		if (EffectDeg < SemicircleAngle - FireBall_HighAngle)
		{
			EffectDeg = SemicircleAngle - FireBall_HighAngle;
		}
		else if (EffectDeg >= SemicircleAngle + FireBall_HighAngle)
		{
			EffectDeg = SemicircleAngle + FireBall_HighAngle;
		}
	}
	else if (DistanceToKriby.X >= 0.0f)
	{
		if (EffectDeg > FireBall_HighAngle && EffectDeg < 90.0f)
		{
			EffectDeg = FireBall_HighAngle;
		}
		else if (EffectDeg > 270.0f && EffectDeg <= 360.0f - FireBall_HighAngle)
		{
			EffectDeg = 360.0f - FireBall_HighAngle;
		}
	}


	{
		std::string Text = "";
		Text += "FireBall 각도";
		Text += std::to_string(static_cast<int>(EffectDeg));
		TextOutA(_dc, _TextXPos, 2 + _TextYPos - _RenderNumber * DebugRenderText_YInter, Text.c_str(), static_cast<int>(Text.size()));

		++_RenderNumber;
	}
}


void HotHead::ThisDebugTriggerRender(HDC _dc)
{
	float4 WinScale = GameEngineWindow::MainWindow.GetScale();

	float4 KirbyCameraPos = KirbyActorCameraPos();
	float4 ActorScenePos = ActorCameraPos();
	float4 DistanceToKriby = GetKirbyOpponentDistance();

	Kirby* KirbyPtr = Kirby::GetMainKirby();
	if (nullptr == KirbyPtr)
	{
		MsgBoxAssert("커비를 불러오지 못했습니다.");
		return;
	}


	float4 KirbyPos = KirbyPtr->GetPos();

	if (HOTHEADFIREBALLRANGEDETECTION < abs(KirbyPos.X - GetPos().X) && HOTHEADRANGEDETECTION > abs(KirbyPos.X - GetPos().X))
	{
		float4 FireBallAngle = float4::GetUnitVectorFromDeg(FireBall_HighAngle);
		FireBallAngle *= WinScale.Half().X;

		float EffectDeg = DistanceToKriby.AngleDeg();

		if (DistanceToKriby.X < 0.0f)
		{
			if (EffectDeg < SemicircleAngle - FireBall_HighAngle)
			{
				MoveToEx(_dc, ActorScenePos.iX(), ActorScenePos.iY() - Scale.Half().iY(), NULL);
				LineTo(_dc, ActorScenePos.iX() - WinScale.Half().iX(), ActorScenePos.iY() + FireBallAngle.iY() - Scale.Half().iY());
			}
			else if (EffectDeg >= SemicircleAngle + FireBall_HighAngle)
			{
				MoveToEx(_dc, ActorScenePos.iX(), ActorScenePos.iY() - Scale.Half().iY(), NULL);
				LineTo(_dc, ActorScenePos.iX() - WinScale.Half().iX(), ActorScenePos.iY() - FireBallAngle.iY() - Scale.Half().iY());
			}
			else
			{
				MoveToEx(_dc, ActorScenePos.iX(), ActorScenePos.iY() - Scale.Half().iY(), NULL);
				LineTo(_dc, KirbyCameraPos.iX(), KirbyCameraPos.iY() - Scale.Half().iY());
			}
		}
		else if (DistanceToKriby.X >= 0.0f)
		{
			if (EffectDeg > FireBall_HighAngle && EffectDeg < 90.0f)
			{
				MoveToEx(_dc, ActorScenePos.iX(), ActorScenePos.iY() - Scale.Half().iY(), NULL);
				LineTo(_dc, ActorScenePos.iX() + WinScale.Half().iX(), ActorScenePos.iY() + FireBallAngle.iY() - Scale.Half().iY());
			}
			else if (EffectDeg > 270.0f && EffectDeg <= 360.0f - FireBall_HighAngle)
			{
				MoveToEx(_dc, ActorScenePos.iX(), ActorScenePos.iY() - Scale.Half().iY(), NULL);
				LineTo(_dc, ActorScenePos.iX() + WinScale.Half().iX(), ActorScenePos.iY() - FireBallAngle.iY() - Scale.Half().iY());
			}
			else
			{
				MoveToEx(_dc, ActorScenePos.iX(), ActorScenePos.iY() - Scale.Half().iY(), NULL);
				LineTo(_dc, KirbyCameraPos.iX(), KirbyCameraPos.iY() - Scale.Half().iY());
			}
		}

	}


	if (DistanceToKriby.X < 0.0f)
	{
		MoveToEx(_dc, ActorScenePos.iX() - static_cast<int>(HOTHEADFIREBALLRANGEDETECTION), 0, NULL);
		LineTo(_dc, ActorScenePos.iX() - static_cast<int>(HOTHEADFIREBALLRANGEDETECTION), WinScale.iY());

		MoveToEx(_dc, ActorScenePos.iX() - static_cast<int>(HOTHEADRANGEDETECTION), 0, NULL);
		LineTo(_dc, ActorScenePos.iX() - static_cast<int>(HOTHEADRANGEDETECTION), WinScale.iY());
	}

	if (DistanceToKriby.X >= 0.0f)
	{
		MoveToEx(_dc, ActorScenePos.iX() + static_cast<int>(HOTHEADFIREBALLRANGEDETECTION), 0, NULL);
		LineTo(_dc, ActorScenePos.iX() + static_cast<int>(HOTHEADFIREBALLRANGEDETECTION), WinScale.iY());

		MoveToEx(_dc, ActorScenePos.iX() + static_cast<int>(HOTHEADRANGEDETECTION), 0, NULL);
		LineTo(_dc, ActorScenePos.iX() + static_cast<int>(HOTHEADRANGEDETECTION), WinScale.iY());
	}


}