#include "LaserEffect.h"
#include "ContentsEnum.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/ResourcesManager.h>

#include "GlobalContents.h"

LaserEffect::LaserEffect() 
{
}

LaserEffect::~LaserEffect() 
{
}


void LaserEffect::Start() 
{
	MainRenderer = CreateRenderer(RenderOrder::AbillityEffect);

	GlobalContents::TextureFileLoad("LaserEffect_1x1_32x8.bmp", "Resources\\Effect\\SkillEffect");

	MainRenderer->SetTexture("LaserEffect_1x1_32x8.bmp");
	MainRenderer->SetRenderScaleToTexture();

	GameEngineWindowTexture* Texture = ResourcesManager::GetInst().FindTexture("LaserEffect_1x1_32x8.bmp");
	Scale = Texture->GetScale();
	SetCheckPoint(Scale);
}

void LaserEffect::init(const float4& _Pos, const float4& _MaterScale , const float4& _Dir)
{
	EffectDir = _Dir;
	SetPos(_Pos + EffectDir * (_MaterScale.Half().X + Scale.Half().X) + float4{ 0.0f, -_MaterScale.Half().Y });
}


void LaserEffect::Update(float _Delta) 
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


void LaserEffect::GroundPassUpdate(float _Delta)
{
	AddPos(EffectDir * LaserEffectSPEED * _Delta);

	float4 WinScale = GameEngineWindow::MainWindow.GetScale();

	if (CameraPos().X > GetPos().X && GetPos().X > CameraPos().X + WinScale.X)
	{
		Death();
	}
}


void LaserEffect::GroundNotPassUpdate(float _Delta)
{

}
