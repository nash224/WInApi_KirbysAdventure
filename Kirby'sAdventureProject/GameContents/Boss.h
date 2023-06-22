#pragma once
#include "Enemy.h"

// ���� :
class Boss : public Enemy
{
public:
	// constrcuter destructer
	Boss();
	~Boss();

	// delete Function
	Boss(const Boss& _Other) = delete;
	Boss(Boss&& _Other) noexcept = delete;
	Boss& operator=(const Boss& _Other) = delete;
	Boss& operator=(Boss&& _Other) noexcept = delete;

protected:
	std::string CurLevel_BitMap_FileName = "";

private:

};

