#include "AirExplosionAEffect.h"
#include "ContentsEnum.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineSprite.h>

#include "GlobalContents.h"

AirExplosionAEffect::AirExplosionAEffect()
{
}

AirExplosionAEffect::~AirExplosionAEffect()
{
}


void AirExplosionAEffect::Start()
{
	MainRenderer = CreateRenderer(RenderOrder::AbillityEffect);

	GlobalContents::SpriteFileLoad("BombEffectA_1x3_16x16.bmp", "Resources\\Effect\\SkillEffect", 3, 1);

	MainRenderer->CreateAnimation("Afterimage2", "BombEffectA_1x3_16x16.bmp", 0, 0, 0.44f, false);
	MainRenderer->ChangeAnimation("Afterimage2");

	//Scale = Sprite->GetSprite(0).RenderScale;
}

void AirExplosionAEffect::init(const float4& _ExplosionPos)
{
	SetPos(_ExplosionPos);
}


void AirExplosionAEffect::Update(float _Delta)
{
	if (GetLiveTime() > AirExplosionAEffectFRAMECHANGETIME)
	{
		Death();
	}
}