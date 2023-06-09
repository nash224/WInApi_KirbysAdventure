#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 : Stage 클리어한 증표입니다. 커비의 프라이드가 높아집니다.
class Flag : public GameEngineActor
{
	friend class VegetableValleyHub;
public:
	// constrcuter destructer
	Flag();
	~Flag();

	// delete Function
	Flag(const Flag& _Other) = delete;
	Flag(Flag&& _Other) noexcept = delete;
	Flag& operator=(const Flag& _Other) = delete;
	Flag& operator=(Flag&& _Other) noexcept = delete;


	void init(const float4& _StageOneDoorPos);

protected:

private:
	// GameEngineObject 상속
	void Start() override;


	// 렌더
	GameEngineRenderer* FlagRenderer = nullptr;
};

