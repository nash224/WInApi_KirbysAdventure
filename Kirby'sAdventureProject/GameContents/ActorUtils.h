#pragma once
#include "ContentsActor.h"

#define CHECKGROUNDGAP 12.0f
#define CHECKCEILINGDGAP 12.0f
#define CHECKWALLWIDTHHGAP 9.0f
#define CHECKWALLHEIGHTHGAP 12.0f
#define CHECKGAP 3.0f

#define SMALLTYPECOLLISIONSCALE float4{ 48.0f , 48.0f }

#define CHECKSTAIRHEIGHT 48.0f

#define GRAVITYMAXVECTOR 450.0f


enum class AbilityStar
{
	Normal,
	Spark,
	Laser,
	Beam,
	Fire,
	Freeze,
	Thorn,
	Sword,
	Ice,
	UFO,
	Max,
};

// 설명 : 커비와 몬스터의 기능을 제공하는 부모 클래스입니다. 중력, 감지, 충돌, 이동을 제공합니다.
class ActorUtils : public ContentsActor
{
public:
	// constrcuter destructer
	ActorUtils();
	~ActorUtils();

	// delete Function
	ActorUtils(const ActorUtils& _Other) = delete;
	ActorUtils(ActorUtils&& _Other) noexcept = delete;
	ActorUtils& operator=(const ActorUtils& _Other) = delete;
	ActorUtils& operator=(ActorUtils&& _Other) noexcept = delete;


	// 커비 세계관 능력
	AbilityStar Ability = AbilityStar::Max;


	// 충돌 변수
	bool IsCollisioned = false;
	bool IsInhaledStateOn = false;
	bool IsHitted = false;



	void GravityOn()
	{
		IsGravity = true;
	}

	void GravityOff()
	{
		IsGravity = false;
	}

	void SetDir(ActorDir _Dir)
	{
		Dir = _Dir;
	}

	void SetGroundTexture(const std::string& _GroundTextureName);



protected:
	bool IsChangeState = false;
	float StateTime = 0.0f;


	GameEngineCollision* BodyCollision = nullptr;
	GameEngineRenderer* MainRenderer = nullptr;


	// 좌우 방향 값 반환 함수
	float4 GetDirUnitVector() const;




	// 비트맵 감지
	float4 GroundLeftCheckPoint = float4::ZERO;
	float4 GroundRightCheckPoint = float4::ZERO;
	float4 WallBotLeftCheckPoint = float4::ZERO;
	float4 WallTopLeftCheckPoint = float4::ZERO;
	float4 WallBotRightCheckPoint = float4::ZERO;
	float4 WallTopRightCheckPoint = float4::ZERO;
	float4 CeilLeftCheckPoint = float4::ZERO;
	float4 CeilRightCheckPoint = float4::ZERO;

	virtual void SetCheckPoint(const float4& _ScaleSize);


	void GroundCheck();
	bool CeilingCheck();
	bool CheckLeftWall();
	bool CheckRightWall();
	bool CheckLeftWallBasedSpeed(unsigned int _DefaultColor = RGB(255, 255 , 255));
	bool CheckRightWallBasedSpeed(unsigned int _DefaultColor = RGB(255, 255, 255));
	bool CheckCeilingBasedSpeed(unsigned int _DefaultColor = RGB(255, 255, 255));
	bool CheckMapLeftWallBasedSpeed(unsigned int _DefaultColor = RGB(255, 255, 255));
	bool CheckMapRightWallBasedSpeed(unsigned int _DefaultColor = RGB(255, 255, 255));
	bool CheckMapCeilingBasedSpeed(unsigned int _DefaultColor = RGB(255, 255, 255));
	void BlockedByWall();
	void BlockedByGround();
	void BlockedByCeiling();
	void BlockedByAll();
	bool IsSolidGround();
	bool IsPassableGround();

	virtual void ActorCollisionDetectionPointRender();

	bool GetGroundState() const
	{
		return isGround;
	}


private:
	bool isGround = false;


	// 중력
	bool IsGravity = true;
	const float GravityPower = 800.0f;
	const float GravityMaxVector = 450.0f;
	float AirResistance = 1.0f;
	float4 GravityVector = float4::ZERO;

protected:
	void Gravity(float _Delta);
	void ReverseGravity(float _Delta);
	void GravityLimit(float _Delta);
	virtual void VerticalUpdate(float _Delta);
	void GravityReset()
	{
		GravityVector = float4::ZERO;
	}

	float4 GetGravityVector() const
	{
		return GravityVector;
	}

	void SetGravityVector(const float4& _GravityVector)
	{
		GravityVector = _GravityVector;
	}

	void AddGravityVector(const float _GravityPower, float _Delta)
	{
		GravityVector += float4::UP * _GravityPower * _Delta;
	}

	void SetAirResistance(float _AirResistance)
	{
		AirResistance = _AirResistance;
	}

	float GetAirResistance() const
	{
		return AirResistance;
	}


	// 점프 
	bool AbleJump = true;
	float CurrentJumpDistance = 0.0f;


	// Y축 이동 함수
	float CurentVerticalSpeed = 0.0f;

	void VerticalDecelerationUpdate(float _Speed, float _Delta);
	void VerticalSpeedLimitBasedlevitation(float _Speed);
	void VerticalUpdateBasedlevitation(float _Delta);





};

