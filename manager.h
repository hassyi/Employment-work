#pragma once
#include "scene.h"

class Manager
{
private:
	static Manager m_manager;

public:

	static Manager GetInstance() {
		static Manager manager;
		return manager;
	}

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

};