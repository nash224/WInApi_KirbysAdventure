#include "GameEffect.h"
#include "ContentsEnum.h"

#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineSprite.h>


GameEffect::GameEffect()
{
}

GameEffect::~GameEffect()
{
}


void GameEffect::LoadBackGroundEffect(const std::string& _FileName, const std::string& _Path, int _XCount, int _YCount)
{
	ResourcesManager::GetInst().SpriteFileLoad(_FileName, _Path, _XCount, _YCount);
}

void GameEffect::Update(float _Delta)
{

}