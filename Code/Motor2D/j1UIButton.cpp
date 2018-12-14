#include "p2Defs.h"
#include "p2Point.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1UIButton.h"


j1Button::j1Button(UIType type, iPoint position, p2SString name, j1Ui * parent) : j1Ui(type, position, name, parent)
{
	ui_type = type;
	this->parent = parent;
	if (parent->ui_type == UIType::HORIZONTAL_BAR)
		spriteRect = { 642,169,229,69 };
	elementRect = { parent->relativePosition.x + position.x - 2,parent->relativePosition.y + position.y - 2,spriteRect.w + 4,spriteRect.h + 4 };
	if (parent != nullptr)
		relativePosition = { parent->relativePosition.x + position.x, parent->relativePosition.y + position.y };

	else
		relativePosition = { App->gui->viewPoint.x + position.x,App->gui->viewPoint.y + position.y };
}

j1Button::~j1Button() {};

bool j1Button::PreUpdate()
{
	if (state == CLICK_STATE::NORMAL)
	{
		spriteRect = { 642,169,229,69 };
	}

	else if (state == CLICK_STATE::HOVER)
	{
		spriteRect = { 0,113,229,69 };
	}

	else if (state == CLICK_STATE::CLICKED)
	{
		spriteRect = { 411,169,229,69 };
	}

	return true;
}

void j1Button::DrawElement()
{
	iPoint position;

	if (parent != nullptr)
		position = { parent->relativePosition.x + position.x, parent->relativePosition.y + position.y };

	else
		position = { App->gui->viewPoint.x + position.x,App->gui->viewPoint.y };

	App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &spriteRect, 0.0F);
}
