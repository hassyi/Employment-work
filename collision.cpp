#include "main.h"
#include "collision.h"
#include "scene.h"
#include "manager.h"


//” ‚Ì“–‚½‚è”»’è
bool Collision::CollisionBB(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 scl1, XMFLOAT3 scl2)
{
	float Axmax = pos1.x + (scl1.x);
	float Axmin = pos1.x - (scl1.x);
	float Aymax = pos1.y + (scl1.y);
	float Aymin = pos1.y - (scl1.y);
	float Azmax = pos1.z + (scl1.z);
	float Azmin = pos1.z - (scl1.z);

	float Bxmax = pos2.x + (scl2.x);
	float Bxmin = pos2.x - (scl2.x);
	float Bymax = pos2.y + (scl2.y);
	float Bymin = pos2.y - (scl2.y);
	float Bzmax = pos2.z + (scl2.z);
	float Bzmin = pos2.z - (scl2.z);

	if ((Axmax >= Bxmin) && (Axmin <= Bxmax))
	{
		if ((Aymin <= Bymax) && (Aymax >= Bymin))
		{
			if ((Azmax >= Bzmin) && (Azmin <= Bzmax))
			{
				return true;
			}
		}
	}

	return false;
}

bool Collision::CollisionBBHeight(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 scl1, XMFLOAT3 scl2)
{

	float Axmax = pos1.x + (scl1.x);
	float Axmin = pos1.x - (scl1.x);
	float Azmax = pos1.z + (scl1.z);
	float Azmin = pos1.z - (scl1.z);
	float Aymax = pos1.y + (scl1.y);
	float Aymin = pos1.y - (scl1.y);

	float Bxmax = pos2.x + (scl2.x);
	float Bxmin = pos2.x - (scl2.x);
	float Bzmax = pos2.z + (scl2.z);
	float Bzmin = pos2.z - (scl2.z);
	float Bymax = pos2.y + (scl2.y);
	float Bymin = pos2.y - (scl2.y);

	if ((Aymax >= Bymin))
	{
		return true;
	}
	//if ((Axmax >= Bxmin) && (Axmin <= Bxmax))
	//{
	//	if ((Azmax >= Bzmin) && (Azmin <= Bzmax))
	//	{
	//		return true;
	//	}
	//}


	return false;
}

//‹…‘Ì‚Ì“–‚½‚è”»’è
bool Collision::CollisionBS(XMFLOAT3 pos1, XMFLOAT3 pos2, float radius)
{
	XMFLOAT3 direction;
	direction.x = pos1.x - pos2.x;
	direction.y = pos1.y - pos2.y;
	direction.z = pos1.z - pos2.z;

	float length;
	length = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);

	if (length < radius) {
		return true;
	}

	return false;
}

//‰~’Œ‚Ì“–‚½‚è”»’è
bool Collision::CollisionCylinder(XMFLOAT3 pos1, XMFLOAT3 pos2, float radius)
{
	XMFLOAT3 direction;
	direction.x = pos2.x - pos1.x;
	direction.y = pos2.y - pos1.y;
	direction.z = pos2.z - pos1.z;

	float length;
	length = sqrtf(direction.x * direction.x + direction.z * direction.z);

	if (length < radius) {
		return true;
	}

	return false;
}

bool Collision::CollisionCylinderHeight(XMFLOAT3 pos1, XMFLOAT3 pos2, float height)
{
	XMFLOAT3 direction;
	direction.y = pos2.y - pos1.y;

	if (-direction.y > height - 0.5f) {
		return true;
	}

	return false;
}

std::tuple<bool, std::list<Box*>> Collision::CollisionBB(XMFLOAT3 pos, XMFLOAT3 scl)
{
	Scene* scene = Manager::GetScene();
	int objSize = 0;
	std::tuple <bool, std::list<Box*>> collision;

	std::list <Box*> gameobjectlist = scene->GetGameObjectList<Box>();
	std::list <Box*> objectlist;

	for (auto obj : gameobjectlist)
	{
		XMFLOAT3 position = obj->GetPos();
		XMFLOAT3 scale = obj->GetScale();

		if (pos.z - (scl.z) <= position.z + (scale.z) &&
			pos.z + (scl.z) >= position.z - (scale.z) &&
			pos.x - (scl.x) <= position.x + (scale.x) &&
			pos.x + (scl.x) >= position.x - (scale.x) &&
			pos.y - (scl.y * 2) <= position.y + (scale.y * 2) &&//‘«‚ª‚Â‚­
			pos.y + (scl.y * 2) >= position.y - (scale.y)//“ª‚Ô‚Â‚¯‚é
			)
		{
			objectlist.push_back(obj);
			objSize = objectlist.size();
		}

	}

	if (objSize != 0)
	{
		collision = std::make_tuple(true, objectlist);
		return collision;
	}
	else if (objSize == 0)
	{
		collision = std::make_tuple(false, objectlist);
		return collision;
	}
}
