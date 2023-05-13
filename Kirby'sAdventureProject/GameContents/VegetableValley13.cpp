#include "VegetableValley13.h"
#include "Player.h"
#include "BackGround.h"

#include <GameEngineBase/GameEngineMath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineCamera.h>


VegetableValley13::VegetableValley13()
{
}

VegetableValley13::~VegetableValley13()
{
}

void VegetableValley13::Start()
{
	LevelBackGround = GameEngineLevel::CreateActor<BackGround>();
	LevelBackGround->init("VegetableValley1.bmp", float4{ 2 , 342 }, float4{ 1016 , 168 });

	LevelPlayer = GameEngineLevel::CreateActor<Player>();
}

void VegetableValley13::Update(float _Delta)
{
	if (true == GameEngineInput::IsDown('P'))
	{
		GameEngineCore::ChangeLevel("PauseLevel");
	}

	/*if (LevelPlayer->GetPos().iX() >= LevelBackGround->GetScale().iX())
	{
		GameEngineCore::ChangeLevel("PauseLevel");
	}*/
}

void VegetableValley13::Render()
{

}

void VegetableValley13::Release()
{

}

void VegetableValley13::LevelStart(GameEngineLevel* _PrevLevel) { }

void VegetableValley13::LevelEnd(GameEngineLevel* _NextLevel) { }