#ifndef __j1LABEL_H__
#define __j1LABEL_H__

#include "j1UI.h"
#include "p2Point.h"
#include "j1Fonts.h"

class j1Label : public j1Ui
{
public:

	j1Label(UIType type, iPoint position, _TTF_Font* font, p2SString text, SDL_Color textColor, j1Ui* parent);
	~j1Label();

	void DrawElement();
	void SetTextTo(p2SString newtext);

	_TTF_Font*			font;
	p2SString			currentText;
	SDL_Color			textColor;

};
#endif // !__j1LABEL_H__