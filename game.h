#pragma once
#include "scene.h"

class Game : public Scene
{
private:
	class Audio* m_BGM{};


public:
	void Init()override;
	void Update()override;
};