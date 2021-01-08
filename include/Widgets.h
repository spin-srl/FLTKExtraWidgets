#pragma once

#include <Fl/Fl.H>
#include <Fl/fl_draw.H>

#include <Fl/Fl_Scroll.H>
#include <Fl/Fl_Scrollbar.H>
#include <Fl/Fl_Button.H>
#include <Fl/Fl_Value_Input.H>

#include <FL/Fl_Choice.H>

class QScroll: public Fl_Scroll
{
public:
	QScroll();
	QScroll(int x, int y, int w, int h, const char* l = 0);

	void draw();
	int handle(int evt);

	int scroll_h;
	int content_height = 1;
	float scroll_y_ratio = 0;
};

class QButton: public Fl_Button {
public:
	QButton();
	QButton(int x, int y, int w, int h, const char* l = 0);

	bool _color_assigned = false;

	Fl_Color _color = 10;
	Fl_Color bar_color = 47;

	int bar_width = 1;
	bool hovered = false;

	void draw();
	int handle(int evt);
};

class QInput: public Fl_Value_Input {
public:
	QInput();
	QInput(int x, int y, int w, int h, const char* l = 0);

	void draw();
	int handle(int evt);
};

class QChoice: public Fl_Choice{
    int HANDLE_WIDTH=5;
public:
        QChoice();
        QChoice(int x,int y,int w,int h,const char* l=0);

        void draw();
        int handle(int evt);
};

