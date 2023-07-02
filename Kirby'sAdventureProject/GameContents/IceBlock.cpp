#include "IceBlock.h"
#include "ContentsEnum.h"
#include "GlobalContents.h"


#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEngineCore/GameEngineRenderer.h>


#include "ActorUtils.h"
#include "ObejctDisapearingEffect.h"



IceBlock::IceBlock()
{
}

IceBlock::~IceBlock()
{
}


void IceBlock::Start()
{
	MainRenderer = CreateRenderer(RenderOrder::AbillityEffect);
	if (nullptr == MainRenderer)
	{
		MsgBoxAssert("랜더러가 널일 이유가 없어..");
		return;
	}

	GameEngineWindowTexture* Texture = GlobalContents::TextureFileLoad("IceBlock.bmp", "Resources\\Effect\\KirbyBaseEffect");
	if (nullptr == Texture)
	{
		MsgBoxAssert("텍스처를 불러오지 못했습니다.");
		return;
	}

	MainRenderer->SetTexture("IceBlock.bmp");


	Scale = Texture->GetScale();

	SetCheckPoint(Scale);






	BlockBodyCollision = CreateCollision(CollisionOrder::IceBlockMoveBody);
	if (nullptr == BlockBodyCollision)
	{
		MsgBoxAssert("충돌체를 생성하지 못했습니다.");
		return;
	}

	BlockBodyCollision->SetCollisionScale(Scale);
	BlockBodyCollision->SetCollisionType(CollisionType::Rect);
	BlockBodyCollision->On();

}

void IceBlock::init(const float4& _Pos, const float4& _MasterPos)
{
	SetPos(_Pos + float4{ 0.0f , -_MasterPos.Half().Y });
}





void IceBlock::Update(float _Delta)
{
	StateUpdate(_Delta);
}


void IceBlock::StateUpdate(float _Delta)
{
	switch (State)
	{
	case IceBlockState::Idle:
		IdleUpdate(_Delta);
		break;
	case IceBlockState::Wing:
		WingUpdate(_Delta);
		break;
	default:
		break;
	}
}


void IceBlock::ChangeState(IceBlockState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case IceBlockState::Idle:
			IdleStart();
			break;
		case IceBlockState::Wing:
			WingStart();
			break;
		default:
			break;
		}

		State = _State;
	}
}



void IceBlock::IdleStart()
{

}

void IceBlock::IdleUpdate(float _Delta)
{
	BlockToPlayerCollisionCheck();
}



void IceBlock::WingStart()
{
	
}

void IceBlock::WingUpdate(float _Delta)
{
	// 충돌 체크
	AbilityToActorCollisionCheck(CollisionOrder::MonsterBody, true);

	if (false == IsAbilityCollisionCheck)
	{
		int Damage = GameEngineRandom::MainRandom.RandomInt(3, 5);
		AbilityToBossCollisionCheck(CollisionOrder::BossBody, Damage, true);
	}


	// 위치 동기화
	AddPos(EffectDir * SMALLSTARFIREEFFECTSPEED * _Delta);


	// 중앙에 비트맵이 닿았는 지 검사
	if (true == CheckCenterPoint())
	{
		Call_DisapearEffect(false);

		Death();
		EffectPointerRelease();

		return;
	}


	// 카메라 밖으로 넘어가면 
	float4 WinScale = GameEngineWindow::MainWindow.GetScale();
	if (GetCameraPos().X > GetPos().X && GetPos().X > GetCameraPos().X + WinScale.X)
	{
		// 죽고 정리
		Death();
		EffectPointerRelease();

		return;
	}
}






void IceBlock::BlockToPlayerCollisionCheck()
{
	if (nullptr == BlockBodyCollision)
	{
		MsgBoxAssert("충돌체를 불러오지 못했습니다.");
		return;
	}

	
	std::vector<GameEngineCollision*> PlayerCol;
	if (true == BlockBodyCollision->Collision(CollisionOrder::PlayerBody, PlayerCol, CollisionType::Rect, CollisionType::Rect))
	{
		for (size_t i = 0; i < PlayerCol.size(); i++)
		{
			GameEngineCollision* Collision = PlayerCol[i];
			if (nullptr == Collision)
			{
				MsgBoxAssert("충돌체를 불러오지 못했습니다.");
				return;
			}

			GameEngineActor* PlayerPtr = Collision->GetActor();
			if (nullptr == PlayerPtr)
			{
				MsgBoxAssert("액터를 불러오지 못했습니다.");
				return;
			}

			float4 KirbyPos = PlayerPtr->GetPos();

			if (GetPos().X > KirbyPos.X)
			{
				EffectDir = float4::RIGHT;
			}
			else if (GetPos().X <= KirbyPos.X)
			{
				EffectDir = float4::LEFT;
			}

			BlockBodyCollision->Off();
			ChangeState(IceBlockState::Wing);
			return;
		}
	}

}



void IceBlock::SkillDeathEffect()
{
	Call_DisapearEffect();
}




void IceBlock::LevelEnd()
{
	Death();
	EffectPointerRelease();
}