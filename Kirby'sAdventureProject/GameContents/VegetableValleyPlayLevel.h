#pragma once
#include <GameEngineCore/GameEngineLevel.h>


#include <GameEnginePlatform/GameEngineSound.h>

#include "Enemy.h"
#include <vector>

// 페이드 아웃 지속시간
#define FADEOUT_ENDTIME 0.2f
// 커비 Enter 시간
#define KIRBY_ENTERSTATETIME 0.2f


// 설명 :
class VegetableValleyPlayLevel : public GameEngineLevel
{
public:
	static bool PrevLevelTriggerOn;
	static bool NextLevelTriggerOn;
	static bool IsStageEnd;
	static bool IsPlayerEnter;

	static bool ChangeClearDoor;

	static float BGMSoundVolume;
	


	// 디버그
	static bool Level_DebugRenderIsOn;

	// 사운드
	static GameEngineSoundPlayer BGM_Player;
	static bool IsBGM_On;


protected:

	static float UpdateTime;
	float FPSText = 0.0f;

public:
	// constrcuter destructer
	VegetableValleyPlayLevel();
	~VegetableValleyPlayLevel();

	// delete Function
	VegetableValleyPlayLevel(const VegetableValleyPlayLevel& _Other) = delete;
	VegetableValleyPlayLevel(VegetableValleyPlayLevel&& _Other) noexcept = delete;
	VegetableValleyPlayLevel& operator=(const VegetableValleyPlayLevel& _Other) = delete;
	VegetableValleyPlayLevel& operator=(VegetableValleyPlayLevel&& _Other) noexcept = delete;

	size_t GetLevelEnemyCount() const
	{
		return LevelEnemy.size();
	}

	float4 GetLevelBackgroundScale() const
	{
		return BackGroundScale;
	}

	std::string GetLevelBitMapFileName() const
	{
		return BitMapFileName;
	}

	class UIManager* GetUIManager()
	{
		return LevelUIManager;
	}

	class BackGround* GetLevelBackGroundPtr()
	{
		return LevelBackGround;
	}

	class Boss* GetLevelBossPtr()
	{
		return LevelBoss;
	}



protected:
	class UIManager* LevelUIManager = nullptr;
	class BackGround* LevelBackGround = nullptr;
	class GameEffect* LevelEffect = nullptr;
	class Kirby* LevelPlayer = nullptr;
	class Boss* LevelBoss = nullptr;



	

	std::vector<Enemy*> LevelEnemy;



	std::string BitMapFileName = "";

	float4 BackGroundScale = float4::ZERO;



	void SetLevelBackgroundScale(const float4& _Scale)
	{
		BackGroundScale = _Scale;
	}



	virtual void EnemySummon() {}
	void SetPlayerPosAndCameraPos(const float4& _PlayerPos, const float4& _CameraPos);
	void CheckRespawnEnemy();

	void CreateAndSetupBackgroundEffectRenderer(
		const std::string& _AnimationName,
		const std::string& _FileName,
		int _StartFrame, int _EndFrame,
		float4 _Pos, float _Ratio,
		float _Inter = 0.1f, bool _Loop = true);


	// 카메라
	float4 CameraFrontCheckPos = float4::ZERO;
	float4 CameraBackCheckPos = float4::ZERO;



	void CameraFocus(float _Delta);


	// 카메라 지진
public:
	static int Camera_ShakeCount;

	static void RequestShakeCountToLevel(size_t _Value = 1)
	{
		for (size_t i = 0; i < _Value; i++)
		{
			++Camera_ShakeCount;
		}
	}

	static void RequestShakeMagnitudeToLevel(float _Value = 3.0f)
	{
		Camera_ShakeMagnitude = _Value;
	}

private:
	static float Camera_ShakeMagnitude;
	int Camera_ShakeNumber = 0;

	const float Camera_ShakeCycle = 0.04f;
	float Camera_ShakeTime = 0.0f;


public:
	// Kirby Repsawn
	float4 Kirby_RespawnPos = float4::ZERO;

	std::string LevelBgmFileName = "";

	void RePlayBGM();

};

