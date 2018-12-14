#include "p2Defs.h"
#include "p2Log.h"
#include "p2DynArray.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	viewPoint = { 0,0 };
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	viewPoint = { App->render->viewport.x,App->render->viewport.y };

	p2List_item<j1Ui*>*	item = UI_elements.start;
	while (item)
	{
		if (item->data->ui_type == UIType::BUTTON)
			item->data->PreUpdate();

		item->next;
	}
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	p2List_item<j1Ui*>*	item = UI_elements.start;
	p2List_item<j1Ui*>*	draw = item->data->childs.start;
	while (item && item->data->toDraw)
	{
		item->data->DrawElement();
		item->data->toDraw = false;
		if (item->data->childs.start != nullptr)
		{
			draw = item->data->childs.start;
			while (draw && draw->data->toDraw)
			{
				draw->data->DrawElement();
				draw->data->toDraw = false;
				draw->next;
			}
		}
		item->next;
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

j1GraphicElement * j1Gui::CreateUIWindow(UIType type, iPoint position, p2SString name, j1Ui* parent)
{
	j1GraphicElement* element = nullptr;
	element = new j1GraphicElement(type, position, name, parent);
	return element;
}

j1Button * j1Gui::CreateUIButton(UIType type, iPoint position, p2SString name, j1Ui* parent)
{
	j1Button* element = nullptr;
	element = new j1Button(type, position, p2SString name, parent);
	return element;
}

void j1Gui::DestroyUiElement(j1Ui* element)
{
}

bool j1Gui::CheckMousePosition()
{


	return true;
}

// class Gui ---------------------------------------------------