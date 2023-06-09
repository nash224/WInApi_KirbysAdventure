#include "PoppyBrosJr.h"
#include "ContentsEnum.h"
#include "GlobalContents.h"


#include <GameEnginePlatform/GameEngineSound.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>


#include "VegetableValleyPlayLevel.h"
#include "Kirby.h"





// 슈도코드
// GameEngienRenderer* AppleRender = CreateRenderer()
// AppleRender->SetRenderPos(0,0,0)

// 슈도코드
// GameEngienRenderer* MonsterRender = CreateRenderer()
// MonsterRender->SetRenderPos(0,-90,0)

// AppleRunStart() 
//{
//   AppleRender->Change("AppleRun");
//   MonsterRender->Change("AppleRun");
//}

// AppleDeathStart() 
//{
//   AppleRender->Death();
//   AppleRender = nullptr;
//   MonsterRender->Change("AppleRun");
//   MonsterRender->SetRenderPos({0, 0, 0});
//}

// AppleDeathUpdate() 
//{
//   AppleRender->Death();
//   AppleRender = nullptr;
//   MonsterRender->Change("AppleRun");
//   MonsterRender->SetRenderPos();
//}




PoppyBrosJr::PoppyBrosJr()
{
}

PoppyBrosJr::~PoppyBrosJr()
{
}




void PoppyBrosJr::Start()
{
	MainRenderer = CreateRenderer(RenderOrder::Play);
	if (nullptr == MainRenderer)
	{
		MsgBoxAssert("렌더러 생성에 실패했습니다.");
		return;
	}

	GlobalContents::SpriteFileLoad("Left_NormalEnemy.bmp", "Resources\\Unit\\Grunt", 4, 5);
	GlobalContents::SpriteFileLoad("Right_NormalEnemy.bmp", "Resources\\Unit\\Grunt", 4, 5);


	MainRenderer->CreateAnimation("Left_AppleRun", "Left_NormalEnemy.bmp", 16, 17, 0.2f, true);
	MainRenderer->CreateAnimation("Right_AppleRun", "Right_NormalEnemy.bmp", 16, 17, 0.2f, true);

	MainRenderer->CreateAnimation("Left_Escape", "Left_NormalEnemy.bmp", 17, 17, 0.5f, false);
	MainRenderer->CreateAnimation("Right_Escape", "Right_NormalEnemy.bmp", 17, 17, 0.5f, false);

	MainRenderer->CreateAnimation("Left_Idle", "Left_NormalEnemy.bmp", 16, 16, 0.5f, false);
	MainRenderer->CreateAnimation("Right_Idle", "Right_NormalEnemy.bmp", 16, 16, 0.5f, false);

	MainRenderer->CreateAnimation("Left_Jump", "Left_NormalEnemy.bmp", 17, 17, 0.5f, false);
	MainRenderer->CreateAnimation("Right_Jump", "Right_NormalEnemy.bmp", 17, 17, 0.5f, false);

	MainRenderer->CreateAnimation("Left_Fall", "Left_NormalEnemy.bmp", 17, 17, 0.5f, false);
	MainRenderer->CreateAnimation("Right_Fall", "Right_NormalEnemy.bmp", 17, 17, 0.5f, false);


	MainRenderer->SetRenderScaleToTexture();
	MainRenderer->SetScaleRatio(3.0f);

	Scale = float4{ 24.0f, 39.0f };
	SetCheckPoint(Scale);

	Dir = ActorDir::Left;
	SetName("PoppyBros & Jr");

	ChangeState(NormalState::Idle);


	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	if (nullptr == BodyCollision)
	{
		MsgBoxAssert("충돌체를 생성하지 못했습니다.");
		return;
	}

	BodyCollision->SetCollisionPos(float4{ 0.0f , -SMALLTYPECOLLISIONSCALE.hY() });
	BodyCollision->SetCollisionScale(SMALLTYPECOLLISIONSCALE);
	BodyCollision->SetCollisionType(CollisionType::Rect);


	// 사운드 재생
	GlobalContents::SoundFileLoad("Jr_Sound.wav", "Resources\\SoundResources\\EffectVoice");
}


/* ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ */


void PoppyBrosJr::Update(float _Delta)
{
	GroundCheck();

	StateUpdate(_Delta);
}




void PoppyBrosJr::AppleRunStart()
{
	SetCheckPoint(float4{ 48.0f , 96.0f });

	StateTime = 0.0f;
	IsChangeState = false;

	ChangeAnimationState("AppleRun");
}



void PoppyBrosJr::AppleRunUpdate(float _Delta)
{
	StateTime += _Delta;

	if (StateTime >= StateChangeTime)
	{
		IsChangeState = true;
	}

	if (true == IsChangeState)
	{
		ChangeState(NormalState::Jump);
		return;
	}


	BlockedByGround();
	BlockedByWall();

	if (false == GetGroundState())
	{
		Gravity(_Delta);
		GravityLimit(_Delta);
		VerticalUpdate(_Delta);
	}

	EnemyCollisionCheck();
}




void PoppyBrosJr::EscapeStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	ChangeAnimationState("Idle");
}

void PoppyBrosJr::EscapeUpdate(float _Delta)
{
	StateTime += _Delta;

	if (StateTime >= StateChangeTime)
	{
		IsChangeState = true;
	}

	if (true == IsChangeState)
	{
		ChangeState(NormalState::Jump);
		return;
	}


	BlockedByGround();
	BlockedByWall();

	if (false == GetGroundState())
	{
		Gravity(_Delta);
		GravityLimit(_Delta);
		VerticalUpdate(_Delta);
	}

	EnemyCollisionCheck();
}



void PoppyBrosJr::IdleStart()
{
	StateTime = 0.0f;
	IsChangeState = false;
	ChangeAnimationState("Idle");
}

void PoppyBrosJr::IdleUpdate(float _Delta)
{
	StateTime += _Delta;

	if (StateTime >= StateChangeTime)
	{
		IsChangeState = true;
	}

	if (true == IsChangeState)
	{
		ChangeState(NormalState::Jump);
		return;
	}


	BlockedByGround();
	BlockedByWall();

	if (false == GetGroundState())
	{
		Gravity(_Delta);
		GravityLimit(_Delta);
		VerticalUpdate(_Delta);
	}

	EnemyCollisionCheck();
}


void PoppyBrosJr::JumpStart()
{
	StateTime = 0.0f;
	AbleJump = true;
	CurrentJumpDistance = 0.0f;

	GravityReset();

	// 사운드 재생
	GameEngineSound::SoundPlay("Jr_Sound.wav");

	ChangeAnimationState("Jump");
}

void PoppyBrosJr::JumpUpdate(float _Delta)
{
	if (0.0f < GetGravityVector().Y)
	{
		ChangeState(NormalState::Fall);
		return;
	}


	float JumpPower = JumpDistance / JumpDuration;
	CurrentJumpDistance += JumpPower * _Delta;

	if (CurrentJumpDistance > JumpDistance)
	{
		AbleJump = false;
	}

	if (CurrentJumpDistance < JumpDistance && true == AbleJump)
	{
		SetGravityVector(float4::UP * JumpPower);
	}

	if (true == CheckLeftWall())
	{
		Dir = ActorDir::Right;
		CurrentSpeed = -CurrentSpeed;

		if (nullptr == MainRenderer)
		{
			MsgBoxAssert("충돌체를 불러오지 못했습니다.");
			return;
		}

		MainRenderer->ChangeAnimation("Right_Jump");
	}
	else if (true == CheckRightWall())
	{
		Dir = ActorDir::Left;
		CurrentSpeed = -CurrentSpeed;

		if (nullptr == MainRenderer)
		{
			MsgBoxAssert("충돌체를 불러오지 못했습니다.");
			return;
		}

		MainRenderer->ChangeAnimation("Left_Jump");
	}

	if (ActorDir::Left == Dir)
	{
		AddPos(float4::LEFT * Jump_XSpeed * _Delta);
	}

	if (ActorDir::Right == Dir)
	{
		AddPos(float4::RIGHT * Jump_XSpeed * _Delta);
	}


	BlockedByGround();
	BlockedByWall();

	if (false == AbleJump)
	{
		Gravity(_Delta);
	}
	GravityLimit(_Delta);
	VerticalUpdate(_Delta);


	EnemyCollisionCheck();
}


void PoppyBrosJr::FallStart()
{
	StateTime = 0.0f;
	AbleJump = true;
	CurrentJumpDistance = 0.0f;

	GravityReset();

	ChangeAnimationState("Fall");
}

void PoppyBrosJr::FallUpdate(float _Delta)
{
	if (true == GetGroundState())
	{
		ChangeState(NormalState::Idle);
		return;
	}


	if (true == CheckLeftWall())
	{
		Dir = ActorDir::Right;
		CurrentSpeed = -CurrentSpeed;

		if (nullptr == MainRenderer)
		{
			MsgBoxAssert("충돌체를 불러오지 못했습니다.");
			return;
		}

		MainRenderer->ChangeAnimation("Right_Fall");
	}
	else if (true == CheckRightWall())
	{
		Dir = ActorDir::Left;
		CurrentSpeed = -CurrentSpeed;

		if (nullptr == MainRenderer)
		{
			MsgBoxAssert("충돌체를 불러오지 못했습니다.");
			return;
		}

		MainRenderer->ChangeAnimation("Left_Fall");
	}

	if (ActorDir::Left == Dir)
	{
		AddPos(float4::LEFT * Jump_XSpeed * _Delta);
	}

	if (ActorDir::Right == Dir)
	{
		AddPos(float4::RIGHT * Jump_XSpeed * _Delta);
	}


	BlockedByGround();
	BlockedByWall();

	Gravity(_Delta);
	GravityLimit(_Delta);
	VerticalUpdate(_Delta);


	EnemyCollisionCheck();
}



/* ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ */


void PoppyBrosJr::Render(float _Delta)
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

}


void PoppyBrosJr::ThisDebugRender(HDC _dc, int& _RenderNumber, const int _TextXPos, const int _TextYPos)
{
	if (nullptr == _dc)
	{
		MsgBoxAssert("핸들을 불러오지 못했습니다.");
		return;
	}


	{
		std::string Text = "";
		Text += "사과 없엉";
		TextOutA(_dc, _TextXPos, 2 + _TextYPos - _RenderNumber * DebugRenderText_YInter, Text.c_str(), static_cast<int>(Text.size()));

		++_RenderNumber;
	}
}

