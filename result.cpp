#include "result.h"
#include "input.h"
#include "manager.h"
#include "title.h"
#include "Resultpolygon2D.h"

void Result::Init()
{
	AddGameObject<ResultPolygon2D>(2);

}

void Result::Update()
{
	if (Input::GetKeyTrigger(VK_RETURN)) {
		Manager::SetScene<Title>();
	}
}
