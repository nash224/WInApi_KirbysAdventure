#include "BossUI.h"
#include "ContentsEnum.h"
#include "GlobalContents.h"


#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEnginePlatform/GameEngineSound.h>
#include <GameEngineCore/GameEngineRenderer.h>


#include "Kirby.h"
#include "Boss.h"



BossUI::BossUI()
{
	UI = this;
}

BossUI::~BossUI()
{
}



void BossUI::Start()
{
	HubRendererSet("BossUI.bmp", "Resources\\UI");
	EndingRendererSet();
	LivesAniRendererSet();
	PortraitRendererSet(PORTRAITLOCATION);
	LivesNumberRendererSet(BOSS_LIVESFIRSTNUMBERLOCATION);
	StaminaCountRendererSet("UI_LifeBar_3x1_24x42.bmp", "Resources\\UI", BOSS_STAMINAFIRSTNUMBERLOCATION);
	BossStaminaRendererSet();
}



// UI 이미지 세팅
void BossUI::EndingRendererSet()
{
	// 엔딩 패널 렌더러
	EndingPanelRenderer = CreateUIRenderer(RenderOrder::PlayUI);
	if (nullptr == EndingPanelRenderer)
	{
		MsgBoxAssert("렌더러가 Null 입니다..");
		return;
	}

	GameEngineWindowTexture* EndingPanel_Texture = GlobalContents::TextureFileLoad("ThanksForWatching.bmp", "Resources\\UI");
	if (nullptr == EndingPanel_Texture)
	{
		MsgBoxAssert("UI 텍스처가 널일리가 없어");
		return;
	}

	float4 EndingPanelScale = EndingPanel_Texture->GetScale();

	EndingPanelRenderer->SetTexture("ThanksForWatching.bmp");
	EndingPanelRenderer->SetRenderPos(BOSS_ENDINGPANEL_LOCATION + EndingPanelScale.Half());
	EndingPanelRenderer->Off();
}





// 목숨 캐릭터 세팅
void BossUI::LivesAniRendererSet()
{
	// UI LivesAnimation
	LivesAniRenderer = CreateUIRenderer(RenderOrder::PlayUI);
	if (nullptr == LivesAniRenderer)
	{
		MsgBoxAssert("렌더러가 Null 입니다..");
		return;
	}


	GameEngineSprite* Sprite = GlobalContents::SpriteFileLoad("UI_LivesAni_3x1_39x36.bmp", "Resources\\UI", 3, 1);
	if (nullptr == Sprite)
	{
		MsgBoxAssert("UI 텍스처가 널일리가 없어");
		return;
	}

	float4 LivesAniScale = Sprite->GetSprite(0).RenderScale;

	LivesAniRenderer->CreateAnimationToFrame("UI_LivesAnimation", "UI_LivesAni_3x1_39x36.bmp", { 0, 1, 2, 1 }, 0.5f, true);
	LivesAniRenderer->ChangeAnimation("UI_LivesAnimation");
	LivesAniRenderer->SetRenderPos(BOSS_LIVESANILOCATION + LivesAniScale.Half());


}



void BossUI::BossStaminaRendererSet()
{
	// 로드
	GameEngineWindowTexture* Texture = GlobalContents::TextureFileLoad("Boss_UI_LifeBar_1x1_9x24.bmp", "Resources\\UI");
	if (nullptr == Texture)
	{
		MsgBoxAssert("텍스처를 불러오지 못했습니다.");
		return;
	}

	// 이미지 한칸 사이즈
	Boss_StaminaScale = Texture->GetScale();

	Boss_StaminaRenderer.reserve(Boss_MaxStaminaCount);

	// 초기설정
	for (int i = 0; i < Boss_MaxStaminaCount; i++)
	{
		GameEngineRenderer* BossStaminaRenderer = CreateUIRenderer(RenderOrder::PlayUI);
		if (nullptr == BossStaminaRenderer)
		{
			MsgBoxAssert("렌더러 생성에 실패했습니다.");
			return;
		}

		BossStaminaRenderer->SetTexture("Boss_UI_LifeBar_1x1_9x24.bmp");
		BossStaminaRenderer->SetCopyPos(float4::ZERO);
		BossStaminaRenderer->SetCopyScale(Boss_StaminaScale);
		BossStaminaRenderer->SetRenderPos(BOSS_STAMINA_FIRSTNUMBERLOCATION
			+ float4{ Boss_Stamina_Image_Inter * static_cast<int>(i) , 0.0f } + Boss_StaminaScale.Half());
		BossStaminaRenderer->SetRenderScale(Boss_StaminaScale);
		BossStaminaRenderer->Off();


		Boss_StaminaRenderer.push_back(BossStaminaRenderer);
	}


	// 사운드
	GlobalContents::SoundFileLoad("Boss_StaminaFullSound.wav", "Resources\\SoundResources\\EffectVoice");
}


/* ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ */




void BossUI::Update(float _Delta)
{
	// 피 다 채웠으면 실행 x
	if (false == Boss_Stamina_Full_Done)
	{
		BossAppearance(_Delta);
	}


	PortraitState(_Delta);
	ChangePortrait_StarStick();
	ChangePortrait_ByeBye();

	OuchState(_Delta);

	StaminaState();

	if (true == Boss_Stamina_Full_Done)
	{
		BossStaminaState(_Delta);
	}
}





// 보스 출현 로직
void BossUI::BossAppearance(float _Delta)
{
	if (nullptr == BossPtr)
	{
		MsgBoxAssert("보스를 불러오지 못했습니다.");
		return;
	}

	// 보스가 플레이어를 찾았으면, 피를채움
	if (true == BossPtr->IsBossFindKirby)
	{
		BossPtr->IsBossFindKirby = false;
		IsBossStaminaFull = true;
	}

	if (true == IsBossStaminaFull)
	{
		Boss_Stamina_Full_Time += _Delta;
	}


	// 일정 간격마다
	if (Boss_Stamina_Full_Time > Boss_Stamina_Full_Inter)
	{
		Boss_Stamina_Full_Time = 0.0f;

		// 2번 피를 채움
		for (int i = 0; i < 2; i++)
		{
			GameEngineRenderer* Boss_Stamina_RendererPtr = Boss_StaminaRenderer[UI_BossStamina];
			if (nullptr == Boss_Stamina_RendererPtr)
			{
				MsgBoxAssert("참조하지 못했습니다.");
				return;
			}

			Boss_Stamina_RendererPtr->On();


			++UI_BossStamina;
		}


		// 사운드 재생
		GameEngineSound::SoundPlay("Boss_StaminaFullSound.wav");


		if (Boss_MaxStaminaCount == UI_BossStamina)
		{
			Boss_Stamina_Full_Done = true;
		}
	}
}


// UI 데미지 업데이트
void BossUI::OuchState(float _Delta)
{
	if (true == Ouch_State)
	{
		Ouch_Time += _Delta;
	}

	if (Ouch_Time > Ouch_Duration)
	{
		Ouch_Time = 0.0f;
		Ouch_State = false;
		PortraitRenderer->ChangeAnimation("Portrait_Normal");
	}


	// 커비의 체력과 UI의 체력이 다르면+
	if (m_KirbySteminaCount != KirbyPtr->m_KirbyHp && -1 != KirbyPtr->m_KirbyHp)
	{
		// 커비의 체력이 감소하면
		if (m_KirbySteminaCount > KirbyPtr->m_KirbyHp)
		{
			// 피가 0일때 모든 Hp Off
			if (0 == KirbyPtr->m_KirbyHp)
			{
				for (int i = 0; i < StaminaRenderer_vec.size(); i++)
				{
					GameEngineRenderer* StaminaRenderer = StaminaRenderer_vec[i];
					if (nullptr == StaminaRenderer)
					{
						MsgBoxAssert("렌더러를 불러오지 못했습니다.");
						return;
					}

					StaminaRenderer->Off();
				}
			}
			// 피가 0보다 클 때
			else if (KirbyPtr->m_KirbyHp > 0)
			{
				GameEngineRenderer* StaminaRenderer_Off = StaminaRenderer_vec[KirbyPtr->m_KirbyHp];
				if (nullptr == StaminaRenderer_Off)
				{
					MsgBoxAssert("렌더러를 불러오지 못했습니다.");
					return;
				}

				StaminaRenderer_Off->Off();


				for (int i = 0; i < KirbyPtr->m_KirbyHp; i++)
				{
					GameEngineRenderer* StaminaRenderer = StaminaRenderer_vec[i];
					if (nullptr == StaminaRenderer)
					{
						MsgBoxAssert("렌더러를 불러오지 못했습니다.");
						return;
					}

					float Stamina_Inter = 0.1f * static_cast<float>(KirbyPtr->m_KirbyHp);

					StaminaRenderer->FindAnimation("StaminaRemain")->Inters = { Stamina_Inter , Stamina_Inter };
				}

				if (1 == KirbyPtr->m_KirbyHp)
				{
					GameEngineSound::SoundPlay("Kirby_LowerHP.wav");
				}

				PortraitRenderer->ChangeAnimation("Portrait_OUCH");
			}

		}

		m_KirbySteminaCount = KirbyPtr->m_KirbyHp;
		Ouch_State = true;
	}
}



// 보스 스태미나 업데이트
void BossUI::BossStaminaState(float _Delta)
{
	if (nullptr == BossPtr)
	{
		MsgBoxAssert("보스를 불러오지 못했습니다.");
		return;
	}

	int Current_BossHp = BossPtr->m_BossHp;


	if (UI_BossStamina != Current_BossHp)
	{
		for (int i = Current_BossHp; i < UI_BossStamina; i++)
		{
			if (i < 0)
			{
				continue;
			}

			GameEngineRenderer* CurStaminarenderer = Boss_StaminaRenderer[i];
			if (nullptr == CurStaminarenderer)
			{
				MsgBoxAssert("벡터의 렌더러를 불러오지 못했습니다.");
				return;
			}

			CurStaminarenderer->Off();
		}

		UI_BossStamina = Current_BossHp;
	}
}



// 별 막대사탕 초상화
void BossUI::ChangePortrait_StarStick()
{
	if (nullptr == KirbyPtr)
	{
		MsgBoxAssert("커비를 불러오지 못했습니다.");
		return;
	}

	if (true == KirbyPtr->IsReachedStarStick)
	{
		if (false == IsChangeStarStick)
		{
			if (nullptr == PortraitRenderer)
			{
				MsgBoxAssert("초상화 렌더러를 불러오지 못했습니다.");
				return;
			}

			PortraitRenderer->ChangeAnimation("Portrait_Clear");

			IsChangeStarStick = true;
		}
	}
}



// 엔딩 초상화
void BossUI::ChangePortrait_ByeBye()
{
	if (true == IsCall_ByeByePortrait)
	{
		if (nullptr == PortraitRenderer)
		{
			MsgBoxAssert("렌더러를 불러오지 못했습니다.");
				return;
		}

		if (nullptr == EndingPanelRenderer)
		{
			MsgBoxAssert("렌더러를 불러오지 못했습니다.");
			return;
		}

		for (int i = 0; i < StaminaRenderer_vec.size(); i++)
		{
			GameEngineRenderer* StaminaRenderer = StaminaRenderer_vec[i];
			if (nullptr == StaminaRenderer)
			{
				MsgBoxAssert("렌더러를 불러오지 못했습니다.");
				return;
			}

			StaminaRenderer->Off();
		}



		PortraitRenderer->ChangeAnimation("Portrait_ByeBye");
		EndingPanelRenderer->On();

		IsCall_ByeByePortrait = false;
	}
}




/* ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ */



void BossUI::LevelStart()
{
	UI = this;


	KirbyPtr = Kirby::GetMainKirby();
	if (nullptr == KirbyPtr)
	{
		MsgBoxAssert("Kirby가 Null 입니다.");
		return;
	}

	BossPtr = Boss::GetMainBossPtr();
	if (nullptr == BossPtr)
	{
		MsgBoxAssert("보스를 불러오지 못했습니다.");
		return;
	}



	LevelStartStamina();
	LevelStartPortrait();
	LevelStartLivesCount();
}


// 스태미나 유지
void BossUI::LevelStartStamina()
{
	if (nullptr == KirbyPtr)
	{
		MsgBoxAssert("Kirby가 Null 입니다.");
		return;
	}


	// 커비 체력
	for (int i = KirbyPtr->m_KirbyHp; i < StaminaRenderer_vec.size(); i++)
	{
		GameEngineRenderer* StaminaRenderer = StaminaRenderer_vec[i];
		if (nullptr == StaminaRenderer)
		{
			MsgBoxAssert("렌더러를 불러오지 못했습니다.");
			return;
		}

		StaminaRenderer->Off();
	}
}



/* ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ */



void BossUI::LevelEnd()
{
	// 보스 스태미나 설정 초기화
	for (int i = 0; i < Boss_StaminaRenderer.size(); i++)
	{
		GameEngineRenderer* StaminaRenderer = Boss_StaminaRenderer[i];
		if (nullptr == StaminaRenderer)
		{
			MsgBoxAssert("펑");
			return;
		}

		StaminaRenderer->Off();
	}

	Boss_Stamina_Full_Done = false;
	IsBossStaminaFull = false;
	UI_BossStamina = 0;
}