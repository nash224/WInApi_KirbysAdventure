#include "Kirby.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/ResourceManager.h>




void Kirby::IdleStart()
{
	ChangeAnimationState("Idle");
}

void Kirby::RunStart()
{
	DirCheck();
	ChangeAnimationState("Run");
}

void Kirby::JumpStart()
{
	ChangeAnimationState("Jump");
}

void Kirby::AerialMotionStart()
{
	ChangeAnimationState("AerialMotion");
}

void Kirby::FallStart()
{
	ChangeAnimationState("Fall");
}

// =============================================//

void Kirby::IdleUpdate(float _Delta)
{

	if(false == GetGroundState())
	{
		Gravity(_Delta);
	}
	else
	{
		unsigned int CheckColor = GetGroundColor(RGB(255, 255, 255), float4::UP * 3);

		while (CheckColor != RGB(255, 255, 255))
		{
			CheckColor = GetGroundColor(RGB(255, 255, 255), float4::UP * 3);
			AddPos(float4::UP*3);
		}

		GravityReset();
	}

	if (true == (GameEngineInput::IsDown('X')))
	{
		ChangeState(KirbyState::Jump);
		return;
	}

	if (true == (GameEngineInput::IsPress('A') || GameEngineInput::IsPress('D')) &&
		false == (GameEngineInput::IsPress('A') && GameEngineInput::IsPress('D')))
	{
		ChangeState(KirbyState::Run);
	}
}


// RunUpdate중에 A와 D가 프레임사이로 연속적으로 입력됐을 경우
// 좌우 애니메이션이 전환되지 않는 버그가 있음
void Kirby::RunUpdate(float _Delta)
{
	if (false == GetGroundState())
	{
		Gravity(_Delta);
	}
	else
	{
		unsigned int CheckColor = GetGroundColor(RGB(255, 255, 255), float4::UP * 3);

		while (CheckColor != RGB(255, 255, 255))
		{
			CheckColor = GetGroundColor(RGB(255, 255, 255), float4::UP * 3);
			AddPos(float4::UP * 3);
		}

		GravityReset();
	}


	float Speed = 600.0f;
	float4 CheckPos = float4::ZERO;
	float4 MovePos = float4::ZERO;

	if (GameEngineInput::IsPress('A') && GameEngineInput::IsPress('D'))
	{
		MovePos = { 0.0f, MovePos.Y };
	}
	else if (true == GameEngineInput::IsPress('A'))
	{
		CheckPos = { -24.0f , -30.0f };
		MovePos = { -Speed * _Delta, 0.0f };
	}
	else if (true == GameEngineInput::IsPress('D'))
	{
		CheckPos = { 24.0f , -30.0f };
		MovePos = { Speed * _Delta, 0.0f };
	}

	if (MovePos.X == 0.0f)
	{
		ChangeState(KirbyState::Idle);
	}

	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), CheckPos);
		if (Color == RGB(255,255,255))
		{
			AddPos(MovePos);
		}
	}

}

void Kirby::JumpUpdate(float _Delta)
{

	float4 MovePos = float4::ZERO;
	float JumpPower = 600.0f;


	if (GameEngineInput::IsPress('X') && 3.0f >= GameEngineInput::GetPressTime('X'))
	{
		AbleJump = true;
		MovePos = { 0.0f, - JumpPower * _Delta };
	}


	AddPos(MovePos);

	if (false == GetGroundState() && false == AbleJump)
	{
		Gravity(_Delta);
	}

	if (true == GetGroundState() && false == AbleJump)
	{
		ChangeState(KirbyState::Idle);
	}


}


void Kirby::AerialMotionUpdate(float _Delta)
{

}

void Kirby::FallUpdate(float _Delta)
{

}

// ============================================








//Projectile* NewRazer = GetLevel()->CreateActor<Projectile>();
//NewRazer->Renderer->SetTexture("MetaKnightsSoldiersStand.bmp");
//NewRazer->SetDir(float4::RIGHT);
//NewRazer->SetPos(GetPos() + float4::XValue(60.0f));
//NewRazer->SetSpeed(300.0f);