#include "GlobalContents.h"

#include <GameEngineCore/ResourcesManager.h>

GlobalContents::GlobalContents() 
{
}

GlobalContents::~GlobalContents() 
{
}


GameEngineWindowTexture* GlobalContents::TextureFileLoad(const std::string& _FileName, const std::string& _Path)
{
	static GameEngineWindowTexture* ReturnValue;

	if (true == ResourcesManager::GetInst().IsLoadTexture(_FileName))
	{
		GameEngineWindowTexture* Texture = ResourcesManager::GetInst().FindTexture(_FileName);
		if (nullptr == Texture)
		{
			// ����Ƚ�� 0
			MsgBoxAssert("�̰� �� ������?");
			return ReturnValue;
		}

		return Texture;
	}

	GameEnginePath FilePath;
	FilePath.SetCurrentPath();

	std::string ParentPath = GameEnginePath::GetParentString(_Path);
	FilePath.MoveParentToExistsChild(ParentPath);
	FilePath.MoveChild(_Path);
	GameEngineWindowTexture* Texture = ResourcesManager::GetInst().TextureLoad(FilePath.PlusFilePath(_FileName));

	if (nullptr == Texture)
	{
		// ����Ƚ�� 0
		MsgBoxAssert("�̰� �� ������?");
		return ReturnValue;
	}

	return Texture;
}

GameEngineSprite* GlobalContents::SpriteFileLoad(const std::string& _FileName, const std::string& _Path, int _XCount, int _YCount)
{
	static GameEngineSprite* ReturnValue;

	if (true == ResourcesManager::GetInst().IsLoadTexture(_FileName))
	{
		GameEngineSprite* Sprite = ResourcesManager::GetInst().FindSprite(_FileName);
		if (nullptr == Sprite)
		{
			// ����Ƚ�� 0
			MsgBoxAssert("�̰� �� ������?");
			return ReturnValue;
		}

		return Sprite;
	}

	GameEnginePath FilePath;
	std::string ParentPath = GameEnginePath::GetParentString(_Path);
	FilePath.MoveParentToExistsChild(ParentPath);
	FilePath.MoveChild(_Path);

	GameEngineSprite* Sprite = ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath(_FileName), _XCount, _YCount);

	if (nullptr == Sprite)
	{
		// ����Ƚ�� 0
		MsgBoxAssert("�̰� �� ������?");
		return ReturnValue;
	}

	return Sprite;
}
