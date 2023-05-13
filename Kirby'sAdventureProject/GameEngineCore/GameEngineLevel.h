#pragma once
#include "GameEngineObject.h"
#include "GameEngineActor.h"

#include <list>
#include <map>

// ���� : ȭ�� �Ǵ� ���� ������
//        ex) Ÿ��Ʋ ���, �÷��� ���, ���� �忬
class GameEngineCamera;
class GameEngineLevel : public GameEngineObject
{
	friend class GameEngineCore;
	friend class GameEngineActor;

public:
	// constrcuter destructer
	GameEngineLevel();
	~GameEngineLevel();

	// delete Function
	GameEngineLevel(const GameEngineLevel& _Other) = delete;
	GameEngineLevel(GameEngineLevel&& _Other) noexcept = delete;
	GameEngineLevel& operator=(const GameEngineLevel& _Other) = delete;
	GameEngineLevel& operator=(GameEngineLevel&& _Other) noexcept = delete;


	// AllActors�� Ư�� ���� �׷��� �ҷ��ͼ� ���Ӱ� ���� ���� 
	template<typename ActorType>
	ActorType* CreateActor(int _Order = 0)
	{
		// �Ϲ����� ����ȯ�� �Ͼ�� (lvalue������ ����ȯ)
		std::list<GameEngineActor*>& GroupList = AllActors[_Order];
		GameEngineActor* NewActor = new ActorType();
		ActorInit(NewActor, _Order);
		GroupList.push_back(NewActor);

		return dynamic_cast<ActorType*>(NewActor);
	}

	GameEngineCamera* GetMainCamera()
	{
		return MainCamera;
	}

protected:
	virtual void LevelStart(GameEngineLevel* _PrevLevel) {}
	virtual void LevelEnd(GameEngineLevel* _NextLevel) {}

private:
	GameEngineCamera* MainCamera;
	GameEngineCamera* UICamera;
	// ��, ������Ʈ, �÷��̾�, ����

	std::map<int, std::list<GameEngineActor*>> AllActors;

	void ActorInit(GameEngineActor* _Actor, int _Order);
	
	void ActorUpdate(float _Delta);
	void ActorRender();
	void ActorRelease();
};