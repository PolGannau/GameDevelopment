#ifndef __J1GRAPHICELEMENT_H__
#define __J1GRAPHICELEMENT_H__

#include "j1UI.h"
#include "p2Point.h"

class SDL_Rect;

class j1GraphinElement :public j1Ui
{
public:

	j1GraphicElement(UIType type, iPoint position, p2SString name, j1Ui* parent);
	~j1GraphicElement();

	void DrawElement();
};

#endif // !__J1GRAPHICELEMENT_H__
