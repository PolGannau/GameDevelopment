#include "p2Defs.h"
#include "p2Point.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1GraphicElement.h"

j1GraphinElement::j1GraphicElement(UIType type, iPoint position, p2SString name, j1Ui * parent)
{
	ui_type = type;
	this->parent = parent;
	spriteRect = { 0,512,483,512 };
	elementRect = { parent->relativePosition.x + position.x - 2,parent->relativePosition.y + position.y - 2,spriteRect.w + 4,spriteRect.h + 4 };
	if (parent != nullptr)
		relativePosition = { parent->relativePosition.x + position.x, parent->relativePosition.y + position.y };
	else
		relativePosition = { App->gui->viewPoint.x + position.x,App->gui->viewPoint.y };
}

j1GraphinElement::~j1GraphicElement() {};

void j1GraphinElement::DrawElement()
{
	iPoint position;

	if (parent != nullptr)
		position = { parent->relativePosition.x + position.x, parent->relativePosition.y + position.y };
	else
		position = { App->gui->viewPoint.x + position.x,App->gui->viewPoint.y };

	App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &spriteRect, 0.0f);
}
