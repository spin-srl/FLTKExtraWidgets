#pragma once

#include <Fl/Fl.H>
#include <Fl/fl_draw.H>

#include <Fl/Fl_Scroll.H>
#include <Fl/Fl_Scrollbar.H>
#include <Fl/Fl_Button.H>
#include <Fl/Fl_Value_Input.H>
#include <Fl/Fl_Native_File_Chooser.H>
#include <Fl/Fl_Check_Button.H>

#include <FL/Fl_Choice.H>

#define FLTK_CONSTRUCTOR(CLASSNAME) CLASSNAME(int x,int y,int w,int h,const char* l=0)

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
        void resize(int X, int Y, int W, int H);
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
public:
        QChoice();
        QChoice(int x,int y,int w,int h,const char* l=0);

        void draw();
        int handle(int evt);
};

class FilePicker: public Fl_Native_File_Chooser
{
public:
        FilePicker(const char* fname, const char* ext = 0, const char* title = 0);
};

class FileSaver: public Fl_Native_File_Chooser
{
public:
        FileSaver(const char* fname, const char* ext = 0, const  char* title = 0);
};

class QCheck: public Fl_Check_Button{
public:
    QCheck(int x,int y,int w,int h,const char *l=0);

    void draw();
};

class QCloseButton: public Fl_Button{
public:
    QCloseButton(int x,int y,int w,int h,const char*l=0);

    void draw();
};

class DraggingWindow: public Fl_Double_Window{
public:
    FLTK_CONSTRUCTOR(DraggingWindow);

    int handle(int event);
};

void SetTopMost(Fl_Window* w);
