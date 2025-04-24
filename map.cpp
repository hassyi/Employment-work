#include "map.h"
#include "scene.h"
#include "game.h"
#include "transform3DComponent.h"
#include "box.h"
#include "building.h"

const int MAP_CHIP_X = 15;
const int MAP_CHIP_Y = 6;
const int MAP_CHIP_Z = 10;

//0 : なし,	1 : ボックス下,	2 : ボックス中, 3 : ボックス上, 4 : ビル
const int map[MAP_CHIP_Y][MAP_CHIP_Z][MAP_CHIP_X] =
{
	//下
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,4,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,4,0,0,0},
		{0,0,0,0,0,0,0,4,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	},
	//真ん中
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,2,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,2,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,2,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	},
	//上
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	},
	//障害物下段
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	},
	//障害物中段
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	},
	//障害物上段
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	},


};


void Map::Init()
{
	for (int y = 0; y < MAP_CHIP_Y; y++)
	{
		for (int z = 0; z < MAP_CHIP_Z; z++)
		{
			for (int x = 0; x < MAP_CHIP_X; x++)
			{
				switch (map[y][z][x])
				{
				case ID_BOX_UNDER:
					CreateBox(x, y, z, ID_BOX_UNDER);
					break;
				case ID_BOX_MIDDLE:
					CreateBox(x, y, z, ID_BOX_MIDDLE);
					break;
				case ID_BOX_ABOVE:
					CreateBox(x, y, z, ID_BOX_ABOVE);
					break;
				case ID_BUILDING:
					CreateBuilding(x, y, z, ID_BUILDING);
					break;
				default:
					break;
				}
			}
		}
	}
}

void Map::Uninit()
{

}

void Map::Update()
{

}

void Map::Draw()
{

}

void Map::CreateBox(int x, int y, int z, MAP_OBJ_ID id)
{
	XMFLOAT3 pos{};
	pos.x = x * m_Distance.x;
	pos.z = z * m_Distance.z;
	if (id == ID_BOX_UNDER) {
		pos.y = y * m_Distance.y;
	}
	else if (id == ID_BOX_MIDDLE) {
		pos.y = y * m_Distance.y - 2.0f;
	}
	else if (id == ID_BOX_ABOVE) {
		pos.y = y * m_Distance.y - 2.0f;
	}
	Box* box = Scene::GetInstance()->GetScene<Game>()->AddGameObject<Box>(1);
	box->GetComponent<Transform3DComponent>()->SetPos(XMFLOAT3(pos.x, pos.y, -pos.z));
	box->GetComponent<Transform3DComponent>()->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
}

void Map::CreateBuilding(int x, int y, int z, MAP_OBJ_ID id)
{
	XMFLOAT3 pos{};
	pos.x = x * m_Distance.x;
	pos.y = y * m_Distance.y;
	pos.z = z * m_Distance.z;

	Building* building = Scene::GetInstance()->GetScene<Game>()->AddGameObject<Building>(1);
	building->GetComponent<Transform3DComponent>()->SetPos(XMFLOAT3(pos.x, pos.y, -pos.z));
}

