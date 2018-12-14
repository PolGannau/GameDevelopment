#include "p2Defs.h"
#include "p2Point.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Bar.h"

j1Bar::j1Bar(UIType type, iPoint position, p2SString name, j1Ui * parent)
{
	ui_type = type;
	this->parent = parent;
	if (ui_type == UIType::VERTICAL_BAR)
		spriteRect = { 843,330,17,10 };
	if(parent!=nullptr)
		elementRect = { parent->relativePosition.x + position.x,parent->relativePosition.y + position.y };

	else
		relativePosition = { App->gui->viewPoint.x + position.x, App->gui->viewPoint.y };
}

j1Bar::~j1Bar() {};

bool j1Bar::PreUpdate()
{
	return true;
}

void j1Bar::DrawElement()
{
}
