#include "SwordKnight.h"
#include "ContentsEnum.h"
#include "GlobalContents.h"


#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineSound.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>


#include "VegetableValleyPlayLevel.h"
#include "Kirby.h"



SwordKnight::SwordKnight()
{
}

SwordKnight::~SwordKnight()
{
}


void SwordKnight::Start()
{
	MainRenderer = CreateRenderer(RenderOrder::Play);
	if (nullptr == MainRenderer)
	{
		MsgBoxAssert("렌더러를 생성하지 못했습니다.");
		return;
	}

	GlobalContents::SpriteFileLoad("Left_SwordKnight.bmp", "Resources\\Unit\\Grunt", 7, 3);
	GlobalContents::SpriteFileLoad("Right_SwordKnight.bmp", "Resources\\Unit\\Grunt", 7, 3);
	 
	MainRenderer->CreateAnimation("Left_PendulumStride", "Left_SwordKnight.bmp", 14, 19, 0.15f, true);
	MainRenderer->CreateAnimation("Right_PendulumStride", "Right_SwordKnight.bmp", 14, 19, 0.15f, true);

	MainRenderer->CreateAnimation("Left_RaiseSword", "Left_SwordKnight.bmp", 7, 7, SWORDKNIGHTRAISESWORDTIME, false);
	MainRenderer->CreateAnimation("Right_RaiseSword", "Right_SwordKnight.bmp", 7, 7, SWORDKNIGHTRAISESWORDTIME, false);

	MainRenderer->CreateAnimation("Left_Slash", "Left_SwordKnight.bmp", 7, 13, SWORDKNIGHTSLASHFRAMETIME, false);
	MainRenderer->CreateAnimation("Right_Slash", "Right_SwordKnight.bmp", 7, 13, SWORDKNIGHTSLASHFRAMETIME, false);

	MainRenderer->CreateAnimation("Left_Underhand", "Left_SwordKnight.bmp", 0, 0, SWORDKNIGHTUNDERHANDTIME, false);
	MainRenderer->CreateAnimation("Right_Underhand", "Right_SwordKnight.bmp", 0, 0, SWORDKNIGHTUNDERHANDTIME, false);

	MainRenderer->CreateAnimation("Left_ReversingSlash", "Left_SwordKnight.bmp", 0, 6, SWORDKNIGHTSLASHFRAMETIME, false);
	MainRenderer->CreateAnimation("Right_ReversingSlash", "Right_SwordKnight.bmp", 0, 6, SWORDKNIGHTSLASHFRAMETIME, false);

	MainRenderer->SetRenderScaleToTexture();
	MainRenderer->SetScaleRatio(3.0f);


	Scale = float4{ 24.0f, 39.0f };
	SetCheckPoint(Scale);

	Dir = ActorDir::Left;
	SetName("Sword Knight");



	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	if (nullptr == BodyCollision)
	{
		MsgBoxAssert("바디 콜리전이 Null일리가 없어..");
		return;
	}

	BodyCollision->SetCollisionPos(float4{ 0.0f , -SMALLTYPECOLLISIONSCALE.hY() });
	BodyCollision->SetCollisionScale(SMALLTYPECOLLISIONSCALE);
	BodyCollision->SetCollisionType(CollisionType::Rect);
	BodyCollision->On();



	AbilityCollision = CreateCollision(CollisionOrder::MonsterAbility);
	if (nullptr == AbilityCollision)
	{
		MsgBoxAssert("바디 콜리전이 Null일리가 없어..");
		return;
	}

	AbilityCollision->SetCollisionScale(float4{ 141.0f, 99.0f });
	AbilityCollision->SetCollisionType(CollisionType::Rect);
	AbilityCollision->Off();


	// 사운드 로드
	GlobalContents::SoundFileLoad("SwordKnight_AttackSound.wav", "Resources\\SoundResources\\EffectVoice");
}



void SwordKnight::init(const std::string& _FileName, SwordKnightState _State, const float4& _Pos)
{
	Attribute = AttributeType::None;
	Ability = AbilityStar::Sword;

	SetGroundTexture(_FileName);
	RespawnLocation = _Pos;
	RespawnState = _State;
	SetPos(RespawnLocation);
	ChangeState(_State);
	StringRespawnState = CurState;
}



/* ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ */

void SwordKnight::Update(float _Delta)
{
	GroundCheck();

	StateUpdate(_Delta);
}

void SwordKnight::StateUpdate(float _Delta)
{
	switch (State)
	{
	case SwordKnightState::PendulumStride:				return PendulumStrideUpdate(_Delta);
	case SwordKnightState::RaiseSword:					return RaiseSwordUpdate(_Delta);
	case SwordKnightState::Slash:						return SlashUpdate(_Delta);
	case SwordKnightState::Underhand:					return UnderhandUpdate(_Delta);
	case SwordKnightState::ReversingSlash:				return ReversingSlashUpdate(_Delta);
	case SwordKnightState::BeInhaled:					return BeInhaledUpdate(_Delta);
	case SwordKnightState::Hitted:						return HittedUpdate(_Delta);
	default:
		break;
	}
}

void SwordKnight::ChangeState(SwordKnightState _State)
{
	if (_State != State || _State == RespawnState)
	{
		switch (_State)
		{
		case SwordKnightState::PendulumStride:				PendulumStrideStart();				break;
		case SwordKnightState::RaiseSword:					RaiseSwordStart();					break;
		case SwordKnightState::Slash:						SlashStart();						break;
		case SwordKnightState::Underhand:					UnderhandStart();					break;
		case SwordKnightState::ReversingSlash:				ReversingSlashStart();				break;
		case SwordKnightState::BeInhaled:					BeInhaledStart();					break;
		case SwordKnightState::Hitted:						HittedStart();						break;
		default:
			break;
		}
	}

	State = _State;
}

void SwordKnight::ChangeRespawnState()
{
	ChangeState(RespawnState);
}

/* ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ */


void SwordKnight::PendulumStrideStart()
{
	AbilityCoolDown = 0.0f;
	IsChangeState = false;

	GravityReset();
	GetKirbyDirection();

	if (ActorDir::Left == Dir)
	{
		CurrentSpeed = SWORDKNIGHTSPEED;
	}
	else if (ActorDir::Right == Dir)
	{
		CurrentSpeed = -SWORDKNIGHTSPEED;
	}


	ChangeAnimationState("PendulumStride");
}

void SwordKnight::PendulumStrideUpdate(float _Delta)
{
	AbilityCoolDown += _Delta;

	// 사정거리 안이면
	if (AbilityCoolDown > SWORDKNIGHTSLASHCOOLDOWN &&
		SWORDKNIGHTRANGEDETECTION > abs(Kirby::GetMainKirby()->GetPos().X - GetPos().X))
	{
		AbilityCoolDown = 0.0f;
		IsChangeState = true;
	}


	// 랜덤으로 스킬 발동
	if (true == IsChangeState)
	{
		int ActionNumber = GameEngineRandom::MainRandom.RandomInt(2, 5) / 3 + 1;

		switch (ActionNumber)
		{
		case 1:
			ChangeState(SwordKnightState::Underhand);
			break;
		case 2:
			ChangeState(SwordKnightState::RaiseSword);
			break;
		default:
			break;
		}
		return;
	}




	EnemyCollisionCheck();

	if (true == CheckLeftWall() || true == LeftGroundIsCliff() || 
		RespawnLocation.X - SWORDKNIGHTRANGEDETECTION > GetPos().X)
	{
		CurrentSpeed = SWORDKNIGHTSPEED;
	}
	else if (true == CheckRightWall() || true == RightGroundIsCliff() ||
		RespawnLocation.X + SWORDKNIGHTRANGEDETECTION < GetPos().X)
	{
		CurrentSpeed = -SWORDKNIGHTSPEED;
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


void SwordKnight::RaiseSwordStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	GetKirbyDirection();

	float CollisionPosX = 0.0f;

	if (ActorDir::Left == Dir)
	{
		CollisionPosX = -10.5f;
	}
	else if (ActorDir::Right == Dir)
	{
		CollisionPosX = 10.5f;
	}



	if (nullptr == AbilityCollision)
	{
		MsgBoxAssert("충돌체가 Null 입니다.");
		return;
	}


	AbilityCollision->SetCollisionPos(float4{ CollisionPosX , -49.5f });
	

	ChangeAnimationState("RaiseSword");
}

void SwordKnight::RaiseSwordUpdate(float _Delta)
{
	StateTime += _Delta;


	if (StateTime > SWORDKNIGHTRAISESWORDTIME)
	{
		IsChangeState = true;
	}

	if (true == IsChangeState)
	{
		ChangeState(SwordKnightState::Slash);
		return;
	}

	EnemyCollisionCheck();

	BlockedByGround();
}


void SwordKnight::SlashStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	if (ActorDir::Left == Dir)
	{
		CurrentSpeed = -SWORDKNIGHTSLASHINSTANTANEOUSSPEED;
	}
	else if (ActorDir::Right == Dir)
	{
		CurrentSpeed = SWORDKNIGHTSLASHINSTANTANEOUSSPEED;
	}
	
	if (nullptr == AbilityCollision)
	{
		MsgBoxAssert("충돌체가 Null 입니다.");
		return;
	}


	// 사운드 재생
	GameEngineSound::SoundPlay("SwordKnight_AttackSound.wav");



	AbilityCollision->On();
	ChangeAnimationState("Slash");
}

void SwordKnight::SlashUpdate(float _Delta)
{
	if (true == MainRenderer->IsAnimationEnd())
	{
		IsChangeState = true;
	}

	if (true == IsChangeState)
	{
		if (nullptr == AbilityCollision)
		{
			MsgBoxAssert("충돌체가 Null 입니다.");
			return;
		}

		AbilityCollision->Off();


		int SlashLink = GameEngineRandom::MainRandom.RandomInt(0, 2) / 2;
		switch (SlashLink)
		{
		case 0:
			ChangeState(SwordKnightState::PendulumStride);
			break;
		case 1:
			ChangeState(SwordKnightState::ReversingSlash);
			break;
		default:
			break;
		}

		return;
	}


	if (true == CheckLeftWall() || true == LeftGroundIsCliff() || 
		true == CheckRightWall() || true == RightGroundIsCliff())
	{
		CurrentSpeed = -CurrentSpeed;
	}



	BlockedByGround();
	BlockedByWall();

	DecelerationUpdate(_Delta, SWORDKNIGHTDEACELECTIONSPEED);
	HorizontalUpdate(_Delta);


	EnemyCollisionCheck();
	EnemyAbilityAttack();
}


void SwordKnight::UnderhandStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	GetKirbyDirection();

	float CollisionPosX = 0.0f;

	if (ActorDir::Left == Dir)
	{
		CollisionPosX = -10.5f;
	}
	else if (ActorDir::Right == Dir)
	{
		CollisionPosX = 10.5f;
	}


	if (nullptr == AbilityCollision)
	{
		MsgBoxAssert("충돌체가 Null 입니다.");
		return;
	}

	AbilityCollision->SetCollisionPos(float4{ CollisionPosX , -49.5f });


	ChangeAnimationState("Underhand");
}

void SwordKnight::UnderhandUpdate(float _Delta)
{
	StateTime += _Delta;

	if (StateTime > SWORDKNIGHTUNDERHANDTIME)
	{
		IsChangeState = true;
	}

	if (true == IsChangeState)
	{
		ChangeState(SwordKnightState::ReversingSlash);
		return;
	}

	EnemyCollisionCheck();

	BlockedByGround();
}


void SwordKnight::ReversingSlashStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	if (ActorDir::Left == Dir)
	{
		CurrentSpeed = -SWORDKNIGHTUNDERHANDINSTANTANEOUSSPEED;
	}
	else if (ActorDir::Right == Dir)
	{
		CurrentSpeed = SWORDKNIGHTUNDERHANDINSTANTANEOUSSPEED;
	}


	if (nullptr == AbilityCollision)
	{
		MsgBoxAssert("충돌체가 Null 입니다.");
		return;
	}

	AbilityCollision->On();


	// 사운드 재생
	GameEngineSound::SoundPlay("SwordKnight_AttackSound.wav");


	ChangeAnimationState("ReversingSlash");
}

void SwordKnight::ReversingSlashUpdate(float _Delta)
{

	if (true == MainRenderer->IsAnimationEnd())
	{
		IsChangeState = true;
	}

	if (true == IsChangeState)
	{
		AbilityCollision->Off();
		ChangeState(SwordKnightState::PendulumStride);
		return;
	}


	if (true == CheckLeftWall() || true == LeftGroundIsCliff() ||
		true == CheckRightWall() || true == RightGroundIsCliff())
	{
		CurrentSpeed = -CurrentSpeed;
	}



	BlockedByGround();
	BlockedByWall();


	DecelerationUpdate(_Delta, SWORDKNIGHTDEACELECTIONSPEED);
	HorizontalUpdate(_Delta);



	EnemyCollisionCheck();
	EnemyAbilityAttack();
}



void SwordKnight::EnemyCollisionCheck()
{
	if (true == IsInhaledStateOn)
	{
		ChangeState(SwordKnightState::BeInhaled);
		return;
	}

	if (true == IsHitted)
	{
		ChangeState(SwordKnightState::Hitted);
		return;
	}
}




/* ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ */


void SwordKnight::Render(float _Delta)
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

	ThisDebugTriggerRender(dc);
}


// 디버깅 모드 텍스트
void SwordKnight::ThisDebugRender(HDC _dc, int& _RenderNumber, const int _TextXPos, const int _TextYPos)
{
	if (nullptr == _dc)
	{
		MsgBoxAssert("핸들을 불러오지 못했습니다.");
		return;
	}

	if (0.0f != AbilityCoolDown)
	{
		std::string Text = "";
		Text += "스킬 쿨타임 : ";
		Text += std::to_string(SWORDKNIGHTSLASHCOOLDOWN - AbilityCoolDown);
		TextOutA(_dc, _TextXPos, 2 + _TextYPos - _RenderNumber * DebugRenderText_YInter, Text.c_str(), static_cast<int>(Text.size()));

		++_RenderNumber;
	}
}


// 디버깅 모드 도형
void SwordKnight::ThisDebugTriggerRender(HDC _dc)
{
	float4 WinScale = GameEngineWindow::MainWindow.GetScale();

	float4 ActorScenePos = ActorCameraPos();
	float4 DistanceToKriby = GetKirbyOpponentDistance();


	float4 ActorPos = GetPos();
	
	Kirby* KirbyPtr = Kirby::GetMainKirby();
	if (nullptr == KirbyPtr)
	{
		MsgBoxAssert("커비를 불러오지 못했습니다.");
		return;
	}

	float4 KirbyPos = KirbyPtr->GetPos();
	float4 OpponentDistance = KirbyPos - ActorPos;


	if (OpponentDistance.X < 0.0f)
	{
		MoveToEx(_dc, ActorScenePos.iX() - static_cast<int>(SWORDKNIGHTRANGEDETECTION), 0, NULL);
		LineTo(_dc, ActorScenePos.iX() - static_cast<int>(SWORDKNIGHTRANGEDETECTION), WinScale.iY());
	}
	else if (OpponentDistance.X >= 0.0f)
	{
		MoveToEx(_dc, ActorScenePos.iX() + static_cast<int>(SWORDKNIGHTRANGEDETECTION), 0, NULL);
		LineTo(_dc, ActorScenePos.iX() + static_cast<int>(SWORDKNIGHTRANGEDETECTION), WinScale.iY());
	}
}