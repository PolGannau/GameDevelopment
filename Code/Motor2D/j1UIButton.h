#ifndef __j1UIBUTTON_H__
#define __j1UIBUTTON_H__

#include "j1UI.h"
#include "p2Point.h"

class SDL_Rect;

enum class CLICK_STATE
{
	NORMAL,
	HOVER,
	CLICKED
};

class j1Button : public j1Ui
{
public:

	j1Button(UIType type, iPoint position, p2SString name, j1Ui* parent);
	~j1Button();

	bool PreUpdate();

	void DrawElement();


	CLICK_STATE		state = CLICK_STATE::NORMAL;

};
#endif // !__j1UIBUTTON_H__