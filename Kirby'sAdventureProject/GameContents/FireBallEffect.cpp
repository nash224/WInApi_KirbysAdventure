#include "FireBallEffect.h"
#include "ContentsEnum.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/ResourcesManager.h>

FireBallEffect::FireBallEffect()
{
}

FireBallEffect::~FireBallEffect()
{
}


void FireBallEffect::Start()
{
	MainRenderer = CreateRenderer(RenderOrder::AbillityEffect);

	ResourcesManager::GetInst().SpriteFileLoad("Left_FireEffect_2x1_16x16.bmp", "Resources\\Effect\\SkillEffect", 2, 1);
	ResourcesManager::GetInst().SpriteFileLoad("Right_FireEffect_2x1_16x16.bmp", "Resources\\Effect\\SkillEffect", 2, 1);

	MainRenderer->CreateAnimation("Left_FireEffect", "Left_FireEffect_2x1_16x16.bmp", 0, 1, 0.1f, true);
	MainRenderer->CreateAnimation("Right_FireEffect", "Right_FireEffect_2x1_16x16.bmp", 0, 1, 0.1f, true);

	MainRenderer->SetRenderScaleToTexture();
	MainRenderer->SetScaleRatio(3.0f);

	Scale = float4{ 48.0f , 48.0f };
	SetCheckPoint(Scale);
}

void FireBallEffect::init(const float4& _Pos, const float4& _MaterScale, const float4& _Dir)
{
	Dir = _Dir;
	SetPos(_Pos + Dir * (_MaterScale.Half().X + Scale.Half().X) + float4{ 0.0f, -_MaterScale.Half().Y });

	if (Dir.X < 0.0f)
	{
		MainRenderer->ChangeAnimation("Left_FireEffect");
	}
	else if (Dir.X >= 0.0f)
	{
		MainRenderer->ChangeAnimation("Right_FireEffect");
	}
}


void FireBallEffect::Update(float _Delta)
{
	if (true == IsPassGround)
	{
		GroundPassUpdate(_Delta);
	}
	else if (false == IsPassGround)
	{
		GroundNotPassUpdate(_Delta);
	}
}

void FireBallEffect::GroundPassUpdate(float _Delta)
{
	float4 WinScale = GameEngineWindow::MainWindow.GetScale();
	if (CameraPos().X > GetPos().X && GetPos().X > CameraPos().X + WinScale.X)
	{
		Death();
	}

	AddPos(Dir * FIREBALLEFFECTSPEED * _Delta);
}


void FireBallEffect::GroundNotPassUpdate(float _Delta)
{

}