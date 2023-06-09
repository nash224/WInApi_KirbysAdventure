#include "LaserBall.h"
#include "ContentsEnum.h"
#include "GlobalContents.h"


#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>


#include "VegetableValleyPlayLevel.h"
#include "LaserEffect.h"
#include "Kirby.h"



LaserBall::LaserBall()
{
}

LaserBall::~LaserBall()
{
}


void LaserBall::Start()
{
	// 렌더러
	MainRenderer = CreateRenderer(RenderOrder::Play);
	if (nullptr == MainRenderer)
	{
		MsgBoxAssert("렌더러를 생성하지 못했습니다.");
		return;
	}

	GlobalContents::SpriteFileLoad("Left_PowerEnemy.bmp", "Resources\\Unit\\Grunt", 6, 5);
	GlobalContents::SpriteFileLoad("Right_PowerEnemy.bmp", "Resources\\Unit\\Grunt", 6, 5);

	MainRenderer->CreateAnimation("Left_Fly", "Left_PowerEnemy.bmp", 0, 0, 0.3f, false);
	MainRenderer->CreateAnimation("Right_Fly", "Right_PowerEnemy.bmp", 0, 0, 0.3f, false);

	MainRenderer->CreateAnimation("Left_Charging", "Left_PowerEnemy.bmp", 5, 3, LASERBALLSHOOTCHANGEFRAMETIME, true);
	MainRenderer->CreateAnimation("Right_Charging", "Right_PowerEnemy.bmp", 5, 3, LASERBALLSHOOTCHANGEFRAMETIME, true);

	MainRenderer->CreateAnimation("Left_Shoot", "Left_PowerEnemy.bmp", 5, 2, LASERBALLSHOOTCHANGEFRAMETIME, true);
	MainRenderer->CreateAnimation("Right_Shoot", "Right_PowerEnemy.bmp", 5, 2, LASERBALLSHOOTCHANGEFRAMETIME, true);

	MainRenderer->CreateAnimation("Right_RunAway", "Left_PowerEnemy.bmp", 0, 0, 0.3f, false);
	MainRenderer->CreateAnimation("Left_RunAway", "Right_PowerEnemy.bmp", 0, 0, 0.3f, false);

	MainRenderer->SetRenderScaleToTexture();
	MainRenderer->SetScaleRatio(3.0f);


	// 세팅
	Scale = float4{ 24.0f, 39.0f };
	SetCheckPoint(Scale);

	Dir = ActorDir::Left;
	SetName("Laser Ball");


	// 충돌
	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	if (nullptr == BodyCollision)
	{
		MsgBoxAssert("바디 콜리전이 Null일리가 없어..");
		return;
	}
	BodyCollision->SetCollisionPos(float4{ 0.0f , -SMALLTYPECOLLISIONSCALE.hY() });
	BodyCollision->SetCollisionScale(SMALLTYPECOLLISIONSCALE);
	BodyCollision->SetCollisionType(CollisionType::Rect);


	// 디버그용 변수
	Gravitational_Influence = false;
}


void LaserBall::init(const std::string& _FileName, LaserBallState _State, const float4& _Pos)
{
	Attribute = AttributeType::Fire;
	Ability = AbilityStar::Laser;

	SetGroundTexture(_FileName);
	RespawnLocation = _Pos;
	RespawnState = _State;
	SetPos(RespawnLocation);
	ChangeState(_State);
	StringRespawnState = CurState;
}



/* ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ */

void LaserBall::Update(float _Delta)
{
	StateUpdate(_Delta);
}

void LaserBall::StateUpdate(float _Delta)
{
	switch (State)
	{
	case LaserBallState::Fly:					return FlyUpdate(_Delta);
	case LaserBallState::Charging:				return ChargingUpdate(_Delta);
	case LaserBallState::Shoot:					return ShootUpdate(_Delta);
	case LaserBallState::RunAway:				return RunAwayUpdate(_Delta);
	case LaserBallState::BeInhaled:				return BeInhaledUpdate(_Delta);
	case LaserBallState::Hitted:				return HittedUpdate(_Delta);
	default:
		break;
	}
}

void LaserBall::ChangeState(LaserBallState _State)
{
	if (_State != State || _State == RespawnState)
	{
		switch (_State)
		{
		case LaserBallState::Fly:						FlyStart();					break;
		case LaserBallState::Charging:					ChargingStart();			break;
		case LaserBallState::Shoot:						ShootStart();				break;
		case LaserBallState::RunAway:					RunAwayStart();				break;
		case LaserBallState::BeInhaled:					BeInhaledStart();			break;
		case LaserBallState::Hitted:					HittedStart();				break;
		default:
			break;
		}
	}

	State = _State;
}

void LaserBall::ChangeRespawnState()
{
	ChangeState(RespawnState);
}

/* ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ */


void LaserBall::FlyStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	CurrentSpeed = 0.0f;
	CurentVerticalSpeed = 0.0f;
	GravityReset();
	GetKirbyDirection();
	ChangeAnimationState("Fly");
}

void LaserBall::FlyUpdate(float _Delta)
{
	StateTime += _Delta;

	float4 LaserBallPos = GetPos();
	float4 KirbyPos = Kirby::GetMainKirby()->GetPos();
	float4 OpponentDistance = KirbyPos - LaserBallPos;

	// 쏠수 있는 범위에 있으면 충전 상태
	if (LASERBALLSHOOTVERTICALDETECTRANGE > abs(OpponentDistance.Y) &&
		LASERBALLSHOOTDETECTMINRANGE < abs(OpponentDistance.X) && 
		abs(OpponentDistance.X) < LASERBALLSHOOTDETECTMAXRANGE)
	{
		IsChangeState = true;
	}

	if (true == IsChangeState)
	{
		ChangeState(LaserBallState::Charging);
		return;
	}

	EnemyCollisionCheck();

	// 플레이어가 나와의 X축 거리가 300.0f 보다 작으면 아래로 이동
	if (LASERBALLRUNAWAYDETECTRANGE > abs(OpponentDistance.X))
	{
		if (LaserBallPos.Y < KirbyPos.Y) 
		{
			if (OpponentDistance.Y < LASERBALLVERTICALDETECTRANGE)
			{
				CurentVerticalSpeed -= LASERBALLACELECTIONSPEED * _Delta;
			}
			else if (OpponentDistance.Y > LASERBALLVERTICALDETECTRANGE)
			{
				CurentVerticalSpeed += LASERBALLACELECTIONSPEED * _Delta;
			}
		}
		else if (LaserBallPos.Y > KirbyPos.Y)
		{
			if (OpponentDistance.Y > -LASERBALLVERTICALDETECTRANGE)
			{
				CurentVerticalSpeed += LASERBALLACELECTIONSPEED * _Delta;
			}
			else if (OpponentDistance.Y < -LASERBALLVERTICALDETECTRANGE)
			{
				CurentVerticalSpeed -= LASERBALLACELECTIONSPEED * _Delta;
			}
		}
	}
	else if (LASERBALLRUNAWAYDETECTRANGE < abs(OpponentDistance.X))
	{
		if (LaserBallPos.Y < KirbyPos.Y)
		{
			CurentVerticalSpeed += LASERBALLACELECTIONSPEED * _Delta;
		}
		else if (LaserBallPos.Y > KirbyPos.Y)
		{
			CurentVerticalSpeed -= LASERBALLACELECTIONSPEED * _Delta;
		}
	}

	// 일정 거리보다 가까우면 도망가고 멀면 쫗아옴
	if (OpponentDistance.X < LASERBALLRUNAWAYDETECTRANGE && KirbyPos.X > LaserBallPos.X)
	{
		if (ActorDir::Right == Dir)
		{
			Dir = ActorDir::Left;
			MainRenderer->ChangeAnimation("Left_Fly");
		}
		CurrentSpeed -= LASERBALLACELECTIONSPEED * _Delta;
	}
	else if (OpponentDistance.X > -LASERBALLRUNAWAYDETECTRANGE && KirbyPos.X < LaserBallPos.X)
	{
		if (ActorDir::Left == Dir)
		{
			Dir = ActorDir::Right;
			MainRenderer->ChangeAnimation("Right_Fly");
		}
		CurrentSpeed += LASERBALLACELECTIONSPEED * _Delta;
	}

	if (OpponentDistance.X > LASERBALLRUNAWAYDETECTRANGE)
	{
		if (ActorDir::Left == Dir)
		{
			Dir = ActorDir::Right;
			MainRenderer->ChangeAnimation("Right_Fly");
		}
		CurrentSpeed += LASERBALLACELECTIONSPEED * _Delta;
	}
	else if (OpponentDistance.X < -LASERBALLRUNAWAYDETECTRANGE)
	{
		if (ActorDir::Right == Dir)
		{
			Dir = ActorDir::Left;
			MainRenderer->ChangeAnimation("Left_Fly");
		}
		CurrentSpeed -= LASERBALLACELECTIONSPEED * _Delta;
	}

	VerticalSpeedLimitBasedlevitation(LAZERBALLLIMITSPEED);
	VerticalUpdateBasedlevitation(_Delta);

	HorizontalSpeedLimit(LAZERBALLLIMITSPEED);
	HorizontalUpdate(_Delta);
}


void LaserBall::ChargingStart()
{
	StateTime = 0.0f;
	IsChangeState = false;

	ChargingCount = GameEngineRandom::MainRandom.RandomInt(4, 12);
	ShootCount = (ChargingCount - 4) / 2 + 1;

	GetKirbyDirection();
	ChangeAnimationState("Charging");
}

void LaserBall::ChargingUpdate(float _Delta)
{
	if (true == MainRenderer->IsAnimationEnd())
	{
		--ChargingCount;
	}

	if (0 == ChargingCount)
	{
		ChangeState(LaserBallState::Shoot);
		return;
	}

	EnemyCollisionCheck();

}


void LaserBall::ShootStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	CurentVerticalSpeed = 0.0f;
	ChangeAnimationState("Shoot");
}

void LaserBall::ShootUpdate(float _Delta)
{
	if (true == MainRenderer->IsAnimationEnd())
	{
		GameEngineLevel* CurLevePtr = GetLevel();
		if (nullptr == CurLevePtr)
		{
			MsgBoxAssert("레벨을 불러오지 못했습니다.");
			return;
		}

		LaserEffect* LaserEffect1 = CurLevePtr->CreateActor<LaserEffect>(UpdateOrder::Ability);
		if (nullptr == LaserEffect1)
		{
			MsgBoxAssert("액터를 생성하지 못했습니다.");
			return;
		}

		LaserEffect1->init(GetPos(), Scale, GetAbilityDir());
		LaserEffect1->SetActorCollision(CollisionOrder::MonsterAbility, CollisionType::Rect);
		--ShootCount;
		
		if (true == IsInhaledStateOn)
		{
			LaserEffect1->Death();
		}
	}

	if (0 == ShootCount)
	{
		ChangeState(LaserBallState::RunAway);
		return;
	}

	EnemyCollisionCheck();
}

void LaserBall::RunAwayStart()
{
	StateTime = 0.0f;
	IsChangeState = false;

	GetKirbyDirection();
	ChangeAnimationState("RunAway");
}

void LaserBall::RunAwayUpdate(float _Delta)
{

	if (ActorDir::Left == Dir)
	{
		CurrentSpeed = LASERBALLRUNAWAYSPEED;
	}
	else if (ActorDir::Right == Dir)
	{
		CurrentSpeed = -LASERBALLRUNAWAYSPEED;
	}

	ReverseGravity(_Delta);
	VerticalUpdate(_Delta);

	HorizontalUpdate(_Delta);

	EnemyCollisionCheck();
}




void LaserBall::EnemyCollisionCheck()
{
	if (true == IsInhaledStateOn)
	{
		ChangeState(LaserBallState::BeInhaled);
		return;
	}

	if (true == IsHitted)
	{
		ChangeState(LaserBallState::Hitted);
		return;
	}
}



/* ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ */


void LaserBall::Render(float _Delta)
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


void LaserBall::ThisDebugRender(HDC _dc, int& _RenderNumber, const int _TextXPos, const int _TextYPos)
{
	if (nullptr == _dc)
	{
		MsgBoxAssert("핸들을 불러오지 못했습니다.");
		return;
	}


	{
		std::string Text = "";
		Text += "Laser 탄환 수 : ";
		Text += std::to_string(ShootCount);
		TextOutA(_dc, _TextXPos, 2 + _TextYPos - _RenderNumber * DebugRenderText_YInter, Text.c_str(), static_cast<int>(Text.size()));

		++_RenderNumber;
	}
}


void LaserBall::ThisDebugTriggerRender(HDC _dc)
{
	float4 WinScale = GameEngineWindow::MainWindow.GetScale();

	float4 ActorScenePos = ActorCameraPos();
	float4 DistanceToKriby = GetKirbyOpponentDistance();


	float4 ActorPos = GetPos();
	float4 KirbyPos = Kirby::GetMainKirby()->GetPos();
	float4 OpponentDistance = KirbyPos - ActorPos;


	if (OpponentDistance.X < 0.0f)
	{
		MoveToEx(_dc, ActorScenePos.iX() - static_cast<int>(LASERBALLRUNAWAYDETECTRANGE), 0, NULL);
		LineTo(_dc, ActorScenePos.iX() - static_cast<int>(LASERBALLRUNAWAYDETECTRANGE), WinScale.iY());

		MoveToEx(_dc, ActorScenePos.iX() - static_cast<int>(LASERBALLSHOOTDETECTMINRANGE), 0, NULL);
		LineTo(_dc, ActorScenePos.iX() - static_cast<int>(LASERBALLSHOOTDETECTMINRANGE), WinScale.iY());

		MoveToEx(_dc, ActorScenePos.iX() - static_cast<int>(LASERBALLSHOOTDETECTMAXRANGE), 0, NULL);
		LineTo(_dc, ActorScenePos.iX() - static_cast<int>(LASERBALLSHOOTDETECTMAXRANGE), WinScale.iY());
	}
	else if (OpponentDistance.X >= 0.0f)
	{
		MoveToEx(_dc, ActorScenePos.iX() - static_cast<int>(LASERBALLRUNAWAYDETECTRANGE), 0, NULL);
		LineTo(_dc, ActorScenePos.iX() - static_cast<int>(LASERBALLRUNAWAYDETECTRANGE), WinScale.iY());

		MoveToEx(_dc, ActorScenePos.iX() - static_cast<int>(LASERBALLSHOOTDETECTMINRANGE), 0, NULL);
		LineTo(_dc, ActorScenePos.iX() - static_cast<int>(LASERBALLSHOOTDETECTMINRANGE), WinScale.iY());

		MoveToEx(_dc, ActorScenePos.iX() - static_cast<int>(LASERBALLSHOOTDETECTMAXRANGE), 0, NULL);
		LineTo(_dc, ActorScenePos.iX() - static_cast<int>(LASERBALLSHOOTDETECTMAXRANGE), WinScale.iY());
	}


	MoveToEx(_dc, 0, ActorScenePos.iY() - static_cast<int>(LASERBALLVERTICALDETECTRANGE), NULL);
	LineTo(_dc, WinScale.iX(), ActorScenePos.iY() - static_cast<int>(LASERBALLVERTICALDETECTRANGE));

	MoveToEx(_dc, 0, ActorScenePos.iY() - static_cast<int>(LASERBALLSHOOTVERTICALDETECTRANGE), NULL);
	LineTo(_dc, WinScale.iX(), ActorScenePos.iY() - static_cast<int>(LASERBALLSHOOTVERTICALDETECTRANGE));

	MoveToEx(_dc, 0, ActorScenePos.iY() + static_cast<int>(LASERBALLVERTICALDETECTRANGE), NULL);
	LineTo(_dc, WinScale.iX(), ActorScenePos.iY() + static_cast<int>(LASERBALLVERTICALDETECTRANGE));

	MoveToEx(_dc, 0, ActorScenePos.iY() + static_cast<int>(LASERBALLSHOOTVERTICALDETECTRANGE), NULL);
	LineTo(_dc, WinScale.iX(), ActorScenePos.iY() + static_cast<int>(LASERBALLSHOOTVERTICALDETECTRANGE));

}