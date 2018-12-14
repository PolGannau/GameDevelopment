#include "p2Defs.h"
#include "p2Point.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Label.h"

j1Label::j1Label(UIType type, iPoint position, _TTF_Font * font, p2SString text = "", SDL_Color textColor, j1Ui * parent) : j1Ui(type, position, parent)
{
	this->font = font;
	this->textColor = textColor;
	this->parent = parent;
	if (parent != nullptr)
	{
		relativePosition = { parent->relativePosition.x + position.x, parent->relativePosition.y + position.y };
		this->currentText = parent->name;
	}
	else
	{
		relativePosition = { App->gui->viewPoint.x + position.x,App->gui->viewPoint.y };
		this->currentText = text;
	}

	App->font->CalcSize(text.GetString(), elementRect.w, elementRect.h, font);
	elementRect = { position.x - 2,position.y - 2,elementRect.w + 4,elementRect.h + 4 };
}

j1Label::~j1Label()
{
}

void j1Label::DrawElement()
{
	SDL_Texture* text = App->font->Print(currentText.GetString(), textColor, font);
	App->render->Blit(text, relativePosition.x, relativePosition.y, nullptr, 0.0F);

	//Destroy the texture
	SDL_DestroyTexture(text);
}

void j1Label::SetTextTo(p2SString newtext)
{
	this->currentText = newtext;
}
