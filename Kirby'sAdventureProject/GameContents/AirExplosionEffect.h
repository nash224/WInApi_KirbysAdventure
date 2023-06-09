#pragma once
#include "SkillEffect.h"

#define AIREXPLOSIONEFFECTTIME 0.8f
#define AIREXPLOSIONEFFECTFRAMECHANGETIME 0.1f

#define AIREXPLOSIONEFFECTADISTANCE 12.0f
#define AIREXPLOSIONEFFECTBDISTANCE AIREXPLOSIONEFFECTADISTANCE + 24.0f
#define AIREXPLOSIONEFFECTDISTANCE AIREXPLOSIONEFFECTADISTANCE + 48.0f


// 설명 : Scarfy가 달고 있는 Bomb 효과입니다.
class AirExplosionEffect : public SkillEffect
{
public:
	// constrcuter destructer
	AirExplosionEffect();
	~AirExplosionEffect();

	// delete Function
	AirExplosionEffect(const AirExplosionEffect& _Other) = delete;
	AirExplosionEffect(AirExplosionEffect&& _Other) noexcept = delete;
	AirExplosionEffect& operator=(const AirExplosionEffect& _Other) = delete;
	AirExplosionEffect& operator=(AirExplosionEffect&& _Other) noexcept = delete;

	void init(const float4& _Pos, const float4& _MaterScale);

protected:

private:
	// GameEngineObject override
	void Start() override;
	void Update(float _Delta) override;


	// GameEngineLevel override
	void LevelEnd() override;



	// this
	int ImagePosNumber = 0;
	int EndCycle = 0;
	float ImageFrameChangeTime = 0.0f;


};

