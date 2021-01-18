#include "Widgets.h"
#include <Fl/Fl_Double_Window.H>
#include <Fl/names.h>
#include <math.h>
#include <Fl/fl_draw.H>
#include <Fl/Fl_Box.H>
#include <Fl/Enumerations.H>

#ifdef _WINDOWS_
#include <windows.h>
#include <windef.h>
#include <winuser.h>
#else
#include <X11/Xlib.h>
#endif

#include <Fl/platform.H>
// ========================= SCROLL
QScroll::QScroll(int x, int y, int w, int h, const char* l): Fl_Scroll(x, y, w, h, l)
{
	type(0);
}

QScroll::QScroll(): QScroll(0, 0, 100, 20, (const char*)"")
{
}

const int scroll_w_full = 12;
const int scroll_w_min = 4;
const int scroll_v_margin = 2;

static int scroll_w = scroll_w_min;

void QScroll::draw()
{
	if (child(0)->h() < h())
	{
		this->scroll_to(0, 0);
		Fl_Scroll::draw();
		return;
	}

	fl_push_clip(x(), y(), x() + w(), y() + h());

	int scroll_y = y() + scroll_v_margin + scroll_h / 2;
	scroll_y += scroll_y_ratio * (h() - scroll_v_margin * 2 - scroll_h);
	scroll_y -= scroll_h / 2;
	scroll_h = (h() - (scroll_v_margin * 2)) * h() / child(0)->h();

	Fl_Scroll::draw();

	//Background
	fl_color(FL_GRAY);
	fl_rectf(x() + w() - scroll_w, y(), scroll_w, h());

	//Handle
	fl_draw_box(FL_RFLAT_BOX,
                x() + w() - scroll_w + 2,
                scroll_y,
                scroll_w - 4,
                scroll_h,
                selection_color());

	fl_pop_clip();
}

int QScroll::handle(int evt)
{
	bool scrollbar_is_hovered = Fl::event_inside(x() + w() - scroll_w_full * 1.5, y(), scroll_w_full * 1.5, h());

	static bool Dragging = false;

	switch (evt)
	{
	case FL_PUSH:
		if (scrollbar_is_hovered)
		{
			Dragging = true;
		}

		Fl_Scroll::handle(evt);
		return 1;
		break;

	case FL_DRAG:
		if (Dragging)
		{
			scroll_y_ratio = ((Fl::event_y() - y()) - (float)(scroll_v_margin + scroll_h / 2))
                             /
                             (float)(h() - 2 * scroll_v_margin - scroll_h);

			if (scroll_y_ratio > 1)
			{
				scroll_y_ratio = 1;
			}

			if (scroll_y_ratio < 0)
			{
				scroll_y_ratio = 0;
			}

			this->scroll_to(0, scroll_y_ratio * (child(0)->h() - h()));
		}

		Fl_Scroll::handle(evt);
		return 1;
		break;

	case FL_RELEASE:
		Dragging = false;
		return Fl_Scroll::handle(evt);
		break;

	case FL_MOUSEWHEEL:
		if (child(0)->h() < h())
		{
			return Fl_Scroll::handle(evt);
		}

		// if (Fl::belowmouse() != this)return 0;

		scroll_to(0, yposition() + Fl::event_dy() * 5);

		if (yposition() < 0)
		{
			scroll_to(0, 0);
		}

		if (yposition() > child(0)->h() - h())
		{
			scroll_to(0, child(0)->h() - h());
		}

		scroll_y_ratio = (float)yposition() / (float)(child(0)->h() - h());
		redraw();
		Fl_Scroll::handle(evt);
		return 1;
		break;

	case FL_ENTER:
	case FL_LEAVE:
	case FL_MOVE:
		if (child(0)->h() < h())
		{
			return Fl_Scroll::handle(evt);
		}

		if (scrollbar_is_hovered && scroll_w == scroll_w_min)
		{
			selection_color(Fl::event_button1() ? (Fl_Color)10 : fl_lighter(FL_GRAY0));
			scroll_w = scroll_w_full;
			redraw();
			return 1;

		}

		else
		{
			if (!scrollbar_is_hovered && scroll_w != scroll_w_min)
			{
				selection_color(fl_darker(FL_GRAY));
				scroll_w = scroll_w_min;
				redraw();
				return 1;
			}
		}

		return Fl_Scroll::handle(evt);
		break;
	}

    return Fl_Scroll::handle(evt);
}

void QScroll::resize(int X, int Y, int W, int H)
{
    for (int child_id = 0; child_id < children(); ++child_id)
    {
        child(child_id)->size(W, child(child_id)->h());
    }

    Fl_Scroll::resize(X, Y, W, H);
}

// ========================= BUTTON
QButton::QButton(int x, int y, int w, int h, const char* l): Fl_Button(x, y, w, h, l)
{
}

QButton::QButton(): QButton(0, 0, 100, 20, (const char*)"")
{
}

int QButton::handle(int evt)
{
	bool button_is_hovered = Fl::event_inside(this);

	switch (evt)
	{
	case FL_SHOW:
		if (!_color_assigned)
		{
			_color_assigned = true;
			_color = color();
		}

		break;

	case FL_ENTER:
		hovered = true;

		if (!_color_assigned)
		{
			_color_assigned = true;
			_color = color();
		}

		if (active())
		{
			color(fl_lighter(down_color()));
		}

		else
		{
			return 0;
		}

		fl_cursor(FL_CURSOR_HAND);
		redraw();
		Fl::awake();
		return 1;

	case FL_LEAVE:
		hovered = false;
		color(_color);
		redraw();
		Fl::awake();
		fl_cursor(FL_CURSOR_DEFAULT);
		return Fl_Button::handle(evt);
	}

	return Fl_Button::handle(evt);
}

void QButton::draw()
{
	fl_color(active() ? color() : FL_GRAY);
	fl_rectf(x(), y(), w(), h());

	draw_label();
	// fl_color(active ? labelcolor() : fl_lighter(FL_GRAY));
	// fl_font(labelfont(), labelsize());
	// fl_draw(label(), x(), y(), w(), h(), align() | FL_ALIGN_INSIDE, image(), true);
}

// =================================================================
// ========================= SCROLL
QInput::QInput(int x, int y, int w, int h, const char* l): Fl_Value_Input(x, y, w, h, l)
{
}

QInput::QInput(): QInput(0, 0, 100, 20, (const char*)"")
{
}

void QInput::draw()
{
	Fl_Value_Input::draw();
}

int QInput::handle(int evt)
{
	switch (evt)
	{
	case FL_PUSH:
		Fl::focus(this);
		break;
	}

	return Fl_Value_Input::handle(evt);
}

// =================================================================
// ========================= COMBO
QChoice::QChoice(): QChoice(0, 0, 100, 20)
{
}

QChoice::QChoice(int x, int y, int w, int h, const char* l): Fl_Choice(x, y, w, h, l)
{
}

void QChoice::draw()
{
    fl_push_clip(x(), y(), w(), h());

    //Background
    fl_draw_box(FL_GLEAM_THIN_DOWN_BOX, x(), y(), w(), h(), color());

    //Label
    fl_color(this->labelcolor());
    fl_font(this->labelfont(), this->labelsize());
    fl_draw(this->text(), x(), y(), w() - 20, h(), FL_ALIGN_CENTER, image(), 1);

    //HoverIndictor Rect
    fl_color(fl_color_average(color(), FL_BLACK, 0.95));
    fl_rectf(x() + w() - 20, y() + 1, 20 - 1, h() - 2);

    //Down-Arrow
//    if (HANDLE_WIDTH == 20)
    {
        int TCenterX = x() + w() - 10;
        int TCenterY = y() + (h() / 2) + 2;

        fl_color(Fl::belowmouse() == this ? FL_BLACK : fl_darker(color()));
        fl_begin_polygon();
        fl_vertex(TCenterX - 5, TCenterY - 3);
        fl_vertex(TCenterX + 5, TCenterY - 3);
        fl_vertex(TCenterX, TCenterY + 2);
        fl_end_polygon();
    }

    fl_pop_clip();
}

int QChoice::handle(int evt)
{
    switch (evt)
    {
    case FL_ENTER:
//        this->HANDLE_WIDTH = 20;
        fl_cursor(FL_CURSOR_HAND);
        redraw();
        Fl::awake();

        return 1;

    case FL_LEAVE:
//        this->HANDLE_WIDTH = 5;
        fl_cursor(FL_CURSOR_DEFAULT);
        redraw();
        Fl::awake();

        return 1;
    }

    return Fl_Choice::handle(evt);
}


FilePicker::FilePicker(const char* fname, const char* ext, const char* title): Fl_Native_File_Chooser(Type::BROWSE_FILE)
{
    this->options(Fl_Native_File_Chooser::USE_FILTER_EXT);
    this->filter(ext);
    this->directory(".");
    this->preset_file(fname);
    this->title(title == 0 ? "Seleccione el archivo" : title);
}

FileSaver::FileSaver(const char* fname, const  char* ext, const  char* title): Fl_Native_File_Chooser(Type::BROWSE_SAVE_FILE)
{
    this->options(Fl_Native_File_Chooser::SAVEAS_CONFIRM |
                  Fl_Native_File_Chooser::USE_FILTER_EXT |
                  Fl_Native_File_Chooser::NEW_FOLDER
                 );
    this->filter(ext);
    this->preset_file(fname);
    this->directory(".");
    this->title(title == 0 ? "Seleccione el archivo" : title);
}

QCheck::QCheck(int x, int y, int w, int h, const char* l): Fl_Check_Button(x, y, w, h, l) {}

void QCheck::draw()
{
    //BG
    {
        Fl_Color bg_color = value() ? 10 : color();

        if (Fl::belowmouse() == this)
        {
            bg_color = fl_color_average(bg_color, FL_BLACK, 0.9);
        }

//        fl_draw_box(FL_GLEAM_THIN_DOWN_BOX, x(), y(), w(), h(), bg_color);
//        fl_draw_box(FL_BORDER_BOX, x() + w() - h() - 4, y() + 2, 20, h() - 4, bg_color);
        fl_draw_box(FL_GLEAM_THIN_DOWN_BOX, x(), y(), w() - 21, h(), bg_color);
        fl_draw_box(FL_GLEAM_THIN_DOWN_BOX, x() + w() - 20, y(), 20, h(), bg_color);
    }

    //LABEL
    fl_color(labelcolor());
    fl_font(labelfont(), labelsize());
    fl_draw(label(), x() + 5, y(), w() - 20 - 10, h(), align(), NULL, 1);

    //CHECKMARK
    if (value())
    {
#undef V
#define V(X, Y) fl_vertex(x()+w()-10+X,y()+h()/2+Y);
//        fl_rectf(x() + w() - 18, y() + 1, 16, h() - 2, 10);
        fl_color(FL_BLACK);
        fl_begin_line();
        V(-5, 0);
        V(0, 4);
        V(5, -6);
        fl_end_line();
    }
    else
    {
        fl_color(fl_lighter(labelcolor()));
        fl_font(labelfont(), labelsize() - 2);
        fl_draw("no", x() + w() - 20, y(), 20, h(), FL_ALIGN_CENTER, NULL, true);
    }
}

QCloseButton::QCloseButton(int x, int y, int w, int h, const char* l): Fl_Button(x, y, w, h, l)
{
}

void QCloseButton::draw()
{
    Fl_Button::draw();
#undef V
#define V(X,Y) fl_vertex(x()+w()/2+X,y()+h()/2+Y)
    const int R = h() / ((Fl::belowmouse() == this) ? 4 : 6);
    fl_color(FL_WHITE);

    fl_begin_line();
    V(-R, -R);
    V(R, R);
    fl_end_line();
    fl_begin_line();
    V(-R, R);
    V(R, -R);
    fl_end_line();
}

int DraggingWindow::handle(int event)
{
    static int xoff, yoff;

    switch (event)
    {
    case FL_PUSH:
        xoff = this->x() - Fl::event_x_root();
        yoff = this->y() - Fl::event_y_root();
        Fl_Double_Window::handle(event);
        return 1; //Must always return 1

    case FL_DRAG:
        this->position(xoff + Fl::event_x_root(), yoff + Fl::event_y_root());
        this->redraw();
        break;

    case FL_RELEASE:
        this->show();
        break;
    }

    return Fl_Double_Window::handle(event);
}

void SetTopMost(Fl_Window* w)
{
#ifdef _WINDOWS_
    SetWindowPos(fl_xid(w), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#else

#endif
}
