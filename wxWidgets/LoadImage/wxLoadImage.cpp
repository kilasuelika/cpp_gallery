// wxLoadImage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<wx/wx.h>
#include<string>
using namespace std;

class myPanel : public wxPanel {
private:
	wxImage img;
public:
	void OnPaint(wxPaintEvent& event) {
		wxPaintDC dc(this);
		dc.DrawBitmap(img, 10, 10, false);
		//wxFont ft;
		//ft.SetNativeFontInfo(L"思源黑体 Light");
		//dc.SetFont(ft);
		dc.DrawText(wxString::FromUTF8("What are you doing什么"),100,100);
		const char* a = "你好";
		wxString b = wxString::FromUTF8(a);
		wxMessageBox(b, wxString::FromUTF8("万qua "));
	};
	myPanel(wxWindow* par, wxWindowID id, string filename) :wxPanel(par, id) {
		
		//img.AddHandler(new wxPNGHandler);
		img.LoadFile(filename, wxBITMAP_TYPE_PNG);



	};

	DECLARE_EVENT_TABLE()


};

BEGIN_EVENT_TABLE(myPanel, wxPanel)
EVT_PAINT(myPanel::OnPaint)

END_EVENT_TABLE()

class myApp : public wxApp {
private:
	bool OnInit() {
		wxFrame* fr = new wxFrame(nullptr, wxID_ANY, "Hello");
		wxImage::AddHandler(new wxPNGHandler);
		wxIcon icon("bluewhales 024.ico", wxBITMAP_TYPE_ICO);
		fr->SetIcon(icon);
		fr->Maximize();
		myPanel* pa = new myPanel(fr, wxID_ANY, "document.png");

		fr->Show();
		return true;
	}
public:

};

IMPLEMENT_APP(myApp)

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
