#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class ContentUtils : public GameEngineActor
{
public:
	// constrcuter destructer
	ContentUtils();
	~ContentUtils();

	// delete Function
	ContentUtils(const ContentUtils& _Other) = delete;
	ContentUtils(ContentUtils&& _Other) noexcept = delete;
	ContentUtils& operator=(const ContentUtils& _Other) = delete;
	ContentUtils& operator=(ContentUtils&& _Other) noexcept = delete;

	void CameraFocus();

	void Gravity();
	void GravityOff()
	{
		IsGravity = false;
	}

	void SetGroundTexture(const std::string& _GroundTextureName);

protected:

private:
	class GameEngineWindowTexture* GroundTexture = nullptr;

	bool IsGravity = true;
	float GravityPower = 100.f;
	float4 GravityVector = float4::ZERO;


};
