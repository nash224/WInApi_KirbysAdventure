#include "StarStick.h"
#include "ContentsEnum.h"


#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>


#include "GlobalContents.h"
#include "Kirby.h"
#include "MoveStarEffect.h"



StarStick::StarStick()
{
}

StarStick::~StarStick()
{
}



void StarStick::Start()
{
	// ������ ����
	MainRenderer = CreateRenderer(RenderOrder::Play);
	if (nullptr == MainRenderer)
	{
		MsgBoxAssert("�������� �������� ���߽��ϴ�.");
		return;
	}

	HaloEffectRenderer = CreateRenderer(RenderOrder::AbillityEffect);
	if (nullptr == HaloEffectRenderer)
	{
		MsgBoxAssert("�������� �������� ���߽��ϴ�.");
		return;
	}


	// ���Ϸε�
	GlobalContents::SpriteFileLoad("StarStick_4x4_72x72.bmp", "Resources\\Unit\\Object", 4, 4);

	// �ִϸ��̼� ����
	MainRenderer->CreateAnimation("StarStick", "StarStick_4x4_72x72.bmp", 0, 11, 0.08f, true);
	HaloEffectRenderer->CreateAnimation("StarStickHalEffect", "StarStick_4x4_72x72.bmp", 12, 13, 0.08f, true);

	MainRenderer->ChangeAnimation("StarStick");
	HaloEffectRenderer->ChangeAnimation("StarStickHalEffect");


	// ��Ÿ����
	Scale = float4{ 48.0f, 48.0f };
	ItemNumber = 1;

	// �ʱ� ����
	ChangeState(StarStickState::BounceOff);


	// �浹ü ����
	BodyCollision = CreateCollision(CollisionOrder::Object);
	if (nullptr == BodyCollision)
	{
		MsgBoxAssert("Collision �� Null �ϸ��� ����..");
		return;
	}


	BodyCollision->SetCollisionScale(Scale);
	BodyCollision->SetCollisionType(CollisionType::Rect);
}

void StarStick::init(const float4& _StartPos, const float4& _TargetPos)
{
	SetPos(_StartPos);

	TargetPos = _TargetPos;


	float4 StickPos = _StartPos;
	TargetMovePos = TargetPos - StickPos;

	Target_UnitVector = TargetMovePos.NormalizeReturn();
	Target_Distance = TargetMovePos.Size();
}


/* �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ� */


void StarStick::Update(float _Delta)
{
	StateUpdate(_Delta);
}


void StarStick::StateUpdate(float _Delta)
{
	switch (State)
	{
	case StarStickState::BounceOff:					return BounceOffUpdate(_Delta);
	case StarStickState::Floating:					return FloatingUpdate(_Delta);
	default:
		break;
	}
}

void StarStick::ChangeState(StarStickState _State)
{
	if (_State != State)
	{
		switch (_State)
		{
		case StarStickState::BounceOff:				BounceOffStart();				break;
		case StarStickState::Floating:				FloatingStart();				break;
		default:
			break;
		}
	}

	State = _State;
}


/* �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ� */


void StarStick::BounceOffStart()
{

}

void StarStick::BounceOffUpdate(float _Delta)
{
	Target_TotalDecelection_UnitVector += Target_UnitVector * Target_Distance * 0.5f * _Delta;
	float4 MovePos = TargetMovePos - Target_TotalDecelection_UnitVector;

	if (Target_UnitVector.X > 0.0f)
	{
		if (MovePos.X < 0.0f)
		{
			ChangeState(StarStickState::Floating);
			return;
		}
	}
	else if (Target_UnitVector.X < 0.0f)
	{
		if (MovePos.X > 0.0f)
		{
			ChangeState(StarStickState::Floating);
			return;
		}
	}


	AddPos(MovePos * _Delta);


	// �浹
	ItemCollisionCheck();
	ItemReachedState();
}


void StarStick::FloatingStart()
{
	IsChangeReverseDir = false;
	ChangePosTime = 0.0f;
	Center_Origin_YPos = GetPos().Y;
}

void StarStick::FloatingUpdate(float _Delta)
{
	ChangePosTime += _Delta;

	// �� Ư���ð�����
	if (ChangePosTime > ChangePosCycle)
	{
		ChangePosTime = 0.0f;

		// ������ �ٲ��� ������
		if (false == IsChangeReverseDir)
		{
			// Ư�� �ȼ���ŭ ������
			float4 MovePos = float4::DOWN * OneTime_Move_HeightDistance;

			AddPos(MovePos);

			// ���� ������ ����� ����ٲ�
			if (GetPos().Y >= Center_Origin_YPos + LimitHeight_ToCenter)
			{
				IsChangeReverseDir = true;
			}
		}
		// ���ϵ���
		else if (true == IsChangeReverseDir)
		{
			float4 MovePos = float4::UP * OneTime_Move_HeightDistance;

			AddPos(MovePos);

			if (GetPos().Y <= Center_Origin_YPos - LimitHeight_ToCenter)
			{
				IsChangeReverseDir = false;
			}
		}
	}


	// �浹
	ItemCollisionCheck();
	ItemReachedState();
}




void StarStick::ItemReachedState()
{
	if (true == IsReached)
	{
		
		GameEngineLevel* CurLevelPtr = GetLevel();
		if (nullptr == CurLevelPtr)
		{
			MsgBoxAssert("������ �ҷ����� ���߽��ϴ�.");
			return;
		}


		float CreateDirDeg = 180.0f;

		for (size_t i = 0; i < 4; i++)
		{
			MoveStarEffect* MoveStarEffectPtr = CurLevelPtr->CreateActor<MoveStarEffect>(UpdateOrder::Ability);
			if (nullptr == MoveStarEffectPtr)
			{
				MsgBoxAssert("���͸� �������� ���߽��ϴ�.");
				return;
			}

			CreateDirDeg += 36.0f;

			float4 StarEffectDir = float4::GetUnitVectorFromDeg(CreateDirDeg);

			MoveStarEffectPtr->init(GetPos(), StarEffectDir);
			MoveStarEffectPtr->SetSpeedAndDuration(80.0f, 1.2f);
		}




		Death();
		ItemPointerRelease();
		if (nullptr != HaloEffectRenderer)
		{
			HaloEffectRenderer = nullptr;
		}

		return;
	}
}