#pragma once
#include "UIManager.h"

// 이미지 좌상단 기준
// 목숨 애니메이션 위치
#define Play_LIVESANILOCATION float4{ 573.0f , 66.0f}


// 목숨 숫자 위치
#define PLAY_LIVESFIRSTNUMBERLOCATION float4{ 636.0f , 78.0f}

// 스태미나 위치
#define PLAY_STAMINAFIRSTNUMBERLOCATION float4{ 228.0f , 45.0f}

// 점수 위치
#define PLAY_SCOREFIRSTNUMBERLOCATION float4{ 228.0f , 102.0f}





// 설명 :
class PlayUI : public UIManager
{
public:
	// 점수
	static int PlayUI_Score;

public:
	// constrcuter destructer
	PlayUI();
	~PlayUI();

	// delete Function
	PlayUI(const PlayUI& _Other) = delete;
	PlayUI(PlayUI&& _Other) noexcept = delete;
	PlayUI& operator=(const PlayUI& _Other) = delete;
	PlayUI& operator=(PlayUI&& _Other) noexcept = delete;


protected:


	// GameEngineObject 상속
private:
	void Start() override;
	void Update(float _Delta) override;

	void LevelStart() override;



	// 렌더
	GameEngineRenderer* LivesAniRenderer = nullptr;
	std::vector<GameEngineRenderer*> ScoreRenderer_vec;



	// Init 
	void LivesAniRendererSet();
	void ScoreRendererSet();

	const size_t ScoreRenderer = 7;
		


	// Update
	// Hp 감소 패턴
	bool Ouch_State = false;
	float Ouch_Time = 0.0f;
	const float Ouch_Duration = 1.0f;


	void OuchState(float _Delta);


	// Hp 증가 패턴
	float Increase_Hp_Time = 0.0f;
	const float Increase_Hp_Cycle = 0.12f;
	bool IsIncresing_Hp = false;



	// 점수 패턴
	int RenderScore = 0;


	void ScoreState();



	// LevelStart
	void LevelStartStamina();

};

