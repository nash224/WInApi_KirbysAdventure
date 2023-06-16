#include "LargeStarFireEffect.h"
#include "ContentsEnum.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineSprite.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/ResourcesManager.h>

#include "GlobalContents.h"
#include "ObejctDisapearingEffect.h"

LargeStarFireEffect::LargeStarFireEffect()
{
}

LargeStarFireEffect::~LargeStarFireEffect()
{
}


void LargeStarFireEffect::Start()
{
	MainRenderer = CreateRenderer(RenderOrder::AbillityEffect);
	if (nullptr == MainRenderer)
	{
		MsgBoxAssert("�������� ���� ������ ����..");
		return;
	}

	GlobalContents::SpriteFileLoad("AbillityStartEffect_1x8_32x31.bmp", "Resources\\Effect\\KirbyBaseEffect", 8, 1);

	MainRenderer->CreateAnimation("LargeStarFire", "AbillityStartEffect_1x8_32x31.bmp", 0, 7, FramesInter);
	MainRenderer->ChangeAnimation("LargeStarFire");

	MainRenderer->SetRenderScaleToTexture();

	Scale = ResourcesManager::GetInst().FindSprite("AbillityStartEffect_1x8_32x31.bmp")->GetSprite(0).RenderScale;
}

void LargeStarFireEffect::init(const float4& _Pos, const float4& _MaterScale, const float4& _Dir)
{
	EffectDir = _Dir;
	SetPos(_Pos + EffectDir * (_MaterScale.Half().X + Scale.Half().X) + float4{ 0.0f, -_MaterScale.Half().Y });
}


void LargeStarFireEffect::Update(float _Delta)
{
	float4 WinScale = GameEngineWindow::MainWindow.GetScale();
	if (GetCameraPos().X > GetPos().X && GetPos().X > GetCameraPos().X + WinScale.X)
	{
		Death();
		if (nullptr != MainRenderer)
		{
			MainRenderer = nullptr;
		}
		if (nullptr != EffectCollision)
		{
			EffectCollision = nullptr;
		}
	}


	AddPos(EffectDir * LARGESTARFIREEFFECTSPEED * _Delta);
}
