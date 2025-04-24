#pragma once

#include "gameObject.h"

class Map : public GameObject
{
private:
	class Scene* m_Scene;

	enum MAP_OBJ_ID
	{
		ID_NONE,
		ID_BOX_UNDER,
		ID_BOX_MIDDLE,
		ID_BOX_ABOVE,
		ID_BUILDING,
	};

	XMFLOAT3 m_Distance = { 7.0f, 4.0f,10.0f };


public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void CreateBox(int x, int y, int z, MAP_OBJ_ID id);
	void CreateBuilding(int x, int y, int z, MAP_OBJ_ID id);

};