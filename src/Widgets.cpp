#include "Widgets.h"
#include <Fl/names.h>
#include <math.h>

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
	if (child(0)->h() < h()) {
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

	// if (scrollbar_is_hovered) {
	// 	fl_cursor(FL_CURSOR_HAND);

	// } else {
	// 	fl_cursor(FL_CURSOR_DEFAULT);
	// }

	static bool Dragging = false;

	switch (evt) {
	case FL_PUSH:
		if (scrollbar_is_hovered) {
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

			if (scroll_y_ratio > 1)scroll_y_ratio = 1;

			if (scroll_y_ratio < 0)scroll_y_ratio = 0;

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
			return Fl_Scroll::handle(evt);

		scroll_to(0, yposition() + Fl::event_dy() * 5);

		if (yposition() < 0)scroll_to(0, 0);

		if (yposition() > child(0)->h() - h()) scroll_to(0, child(0)->h() - h());

		scroll_y_ratio = (float)yposition() / (float)(child(0)->h() - h());
		redraw();
		return Fl_Scroll::handle(evt);
		break;

	case FL_ENTER:
	case FL_LEAVE:
	case FL_MOVE:
		if (child(0)->h() < h())
			return Fl_Scroll::handle(evt);

		if (scrollbar_is_hovered && scroll_w == scroll_w_min) {
			selection_color(Fl::event_button1() ? (Fl_Color)10 : fl_lighter(FL_GRAY0));
			scroll_w = scroll_w_full;
			redraw();
			return 1;

		} else {
			if (!scrollbar_is_hovered && scroll_w != scroll_w_min) {
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

	switch (evt) {
	case FL_SHOW:
		if (!_color_assigned) {
			_color_assigned = true;
			_color = color();
		}

		break;

	case FL_ENTER:
		hovered = true;

		if (!_color_assigned) {
			_color_assigned = true;
			_color = color();
		}

		if (active())
			color(fl_lighter(down_color()));
		else
			return 0;

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

void QButton::draw() {
	fl_color(active() ? color() : FL_GRAY);
	fl_rectf(x(), y(), w(), h());

	fl_color(labelcolor());
	fl_font(labelfont(), labelsize());
	fl_draw(label(), x(), y(), w(), h(), align() | FL_ALIGN_INSIDE, image(), true);
}

// =================================================================
// ========================= SCROLL
QInput::QInput(int x, int y, int w, int h, const char* l): Fl_Value_Input(x, y, w, h, l)
{
}

QInput::QInput(): QInput(0, 0, 100, 20, (const char*)"")
{
}

void QInput::draw() {
	Fl_Value_Input::draw();
}

int QInput::handle(int evt) {
	switch (evt) {
	case FL_PUSH:
		Fl::focus(this);
		break;
	}

	return Fl_Value_Input::handle(evt);
}