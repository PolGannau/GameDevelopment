#ifndef __j1UI_H__
#define __j1UI_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "p2DynArray.h"

class SDL_Rect;

enum class UIType
{
	BUTTON,
	LABEL,
	GRAPHIC_ELEMENT,
	BAR,
	HORIZONTAL_BAR,
	VERTICAL_BAR
};

class j1Ui : public j1Module
{
public:

	j1Ui(UIType type, iPoint position, p2SString name, j1Ui* parent);
	~j1Ui();

	virtual bool Awake(pugi::xml_node);
	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();

	virtual void DrawElement();

public:

	// Important info such as relative position to the camera display
	// type of UI element, children, parent, sprite, interaction rect & type

	iPoint					relativePosition;
	UIType					ui_type;
	p2List<j1Ui*>			childs;
	j1Ui*					parent = nullptr;
	SDL_Rect				spriteRect;

	SDL_Rect				elementRect;

	// Now there is a variable which is the UI element name
	// It's important as an identifier an to give the text to labels (see how in j1Label.h file)

	p2SString				name;

	// --------------------------------------------------------------


	// Info such as the UI element can be dragged or if it's visible

	bool					draggable = true;
	bool					visible = true;
	bool					toDraw = true;

	// ------------------------------------------------------------

};

#endif // !__j1UI_H__