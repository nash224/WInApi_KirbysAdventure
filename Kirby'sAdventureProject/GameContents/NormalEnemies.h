#pragma once
#include "Enemy.h"

enum class NormalState
{
	Idle,
	Walk,
	Sit,
	Jump,
	Fall,
	Sweep,
	Max,
};


// ���� :
class NormalEnemies : public Enemy
{
public:
	// constrcuter destructer
	NormalEnemies();
	~NormalEnemies();

	// delete Function
	NormalEnemies(const NormalEnemies& _Other) = delete;
	NormalEnemies(NormalEnemies&& _Other) noexcept = delete;
	NormalEnemies& operator=(const NormalEnemies& _Other) = delete;
	NormalEnemies& operator=(NormalEnemies&& _Other) noexcept = delete;


	AttributeType Attribute = AttributeType::None;

	virtual void init(const std::string _FileName, const float4& _Pos);

protected:
	NormalState State = NormalState::Max;
	std::string CurState = "";


	void StateUpdate(float _Delta) override;
	void ChangeState(NormalState _State);
	void ChangeAnimationState(const std::string& _StateName) override;


	virtual void IdleStart() {}
	virtual void WalkStart() {}
	virtual void SitStart() {}
	virtual void JumpStart() {}
	virtual void FallStart() {}
	virtual void SweepStart() {}


	virtual void IdleUpdate(float _Delta) {}
	virtual void WalkUpdate(float _Delta) {}
	virtual void SitUpdate(float _Delta) {}
	virtual void JumpUpdate(float _Delta) {}
	virtual void FallUpdate(float _Delta) {}
	virtual void SweepUpdate(float _Delta) {}

private:

};
