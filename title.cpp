#include "title.h"
#include "input.h"
#include "manager.h"
#include "game.h"
#include "Titlepolygon2D.h"

void Title::Init()
{
	AddGameObject<TitlePolygon2D>(2);

}

void Title::Update()
{
	if (Input::GetKeyTrigger(VK_RETURN)) {
		Manager::SetScene<Game>();
	}
}
