#ifndef __j1BAR_H__
#define __j1BAR_H__

#include "j1UI.h"
#include "p2Point.h"

class SDL_Rect;

class j1Bar :public j1Ui
{
public:

	j1Bar(UIType type, iPoint position, p2SString name, j1Ui* parent);
	~j1Bar();
	
	bool PreUpdate();
	void DrawElement();

};

#endif // !__j1BAR_H__
