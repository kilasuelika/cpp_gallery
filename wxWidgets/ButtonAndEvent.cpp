// ConsoleApplication16.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "wx/wx.h"
#include <vector>

using namespace std;
enum
{
	BUTTON_Hello = wxID_HIGHEST + 1
};

class myFrame : public wxFrame {
public:
	wxButton* HelloWorld;
	wxButton* HelloWorld1;
	wxPanel* panel;
	wxPanel* panel1;
	wxBoxSizer* sizer;

	int cursor_x = 10, cursor_y=10;

	void OnPaint(wxPaintEvent& event) {

		SetStatusText(wxT("ÕýÔÚÊäÈë"), 0);
		wxClientDC dc = wxClientDC(this);
		while (true) {
			dc.SetPen(wxPen(wxColor(255, 255, 255), 3));
			dc.DrawLine(wxPoint(cursor_x, cursor_y), wxPoint(cursor_x, cursor_y+20));
			Update();
			wxMilliSleep(500);

			dc.SetPen(wxPen(wxColor(0, 0, 0), 3));
			dc.DrawLine(wxPoint(cursor_x, cursor_y), wxPoint(cursor_x, cursor_y + 20));
			Update();
			wxMilliSleep(500);

		};
	};
	void OnKeyDown(wxKeyEvent& event) {
		wxClientDC dc = wxClientDC(this);
		SetStatusText(event.GetUnicodeKey(), 0);
	};

	myFrame(wxWindow* parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize) :
		wxFrame(parent,id,title, pos, size) {
		SetBackgroundColour(wxColor(255, 255, 255));
		//panel =new wxPanel(this, wxID_ANY);
		//panel->SetBackgroundColour(wxColour(0, 0, 0));
		Connect(wxEVT_PAINT, wxPaintEventHandler(myFrame::OnPaint));
		//Connect(wxEVT_CHAR, wxKeyEventHandler(myFrame::OnKeyDown));
		CreateStatusBar(3);
		SetStatusText(wxT("Ready"), 0);
		Center();
	};

};


class MyApp : public wxApp
{
	bool OnInit() {
		frame = new myFrame((wxFrame*)NULL, -1, wxT("Hello wxDC"),
			wxPoint(50, 50), wxSize(800, 600));
		frame->Show();
		return true;
	};

	wxFrame* frame;

public:

};

IMPLEMENT_APP(MyApp)
