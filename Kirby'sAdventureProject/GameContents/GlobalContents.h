#pragma once
#include <string>

#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEngineCore/GameEngineSprite.h>
#include <GameEngineCore/GameEngineLevel.h>

// 설명 : 모든 Contents 영역에서 공통적으로 사용하는 기능을 제공합니다.
class GlobalContents
{
public:
	// constrcuter destructer
	GlobalContents();
	~GlobalContents();

	// delete Function
	GlobalContents(const GlobalContents& _Other) = delete;
	GlobalContents(GlobalContents&& _Other) noexcept = delete;
	GlobalContents& operator=(const GlobalContents& _Other) = delete;
	GlobalContents& operator=(GlobalContents&& _Other) noexcept = delete;

	static GameEngineWindowTexture* TextureFileLoad(const std::string& _FileName, const std::string& _Path);
	static GameEngineSprite* SpriteFileLoad(const std::string& _FileName, const std::string& _Path, int _XCount, int _YCount);
	static void SoundFileLoad(const std::string& _FileName, const std::string& _Path);

	static void FadeOut(GameEngineLevel* _Level);
	static void FadeIn(GameEngineLevel* _Level);
	static void WhiteFadeOut(GameEngineLevel* _Level);
	static void WhiteFadeIn(GameEngineLevel* _Level);


protected:

private:

};