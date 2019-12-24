// wxLoadImage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<wx/wx.h>
#include<string>
#include <wx/fontenum.h>
#include <wx/caret.h>
#include<filesystem>
#include<fstream>
using namespace std;
using namespace std::filesystem;

class myPanel : public wxPanel {
private:
	wxImage img;
public:
	void OnPaint(wxPaintEvent& event) {
	

		wxPaintDC dc(this);
		dc.DrawBitmap(img, 10, 10, false);
		wxFont ft;
		ft.SetFaceName(L"思源宋体 Medium");
		ft.SetPointSize(12);
		dc.SetFont(ft);
		//ft.AddPrivateFont("SourceHanSansSC-Light.otf");
		//dc.SetFont(ft);
		
		dc.DrawText(L"你好，今天是星期六，天气不错", 100, 100);
		wxCaret* ca = new wxCaret(this, 3, ft.GetPixelSize().GetHeight());
		ca->Move(100, 100);
		ca->Show();

		wxFont ft1;
		ft1.AddPrivateFont("SourceHanSerifSC-Light.otf");
		dc.SetFont(ft1);
		dc.DrawText(wxString::FromUTF8("What are you doing什么"), 100, 200);
		wxFont ft2;
		ft1.SetNativeFontInfo("Terminal");
		dc.SetFont(ft2);
		dc.DrawText("how are yout",100,300);
		
		wxFont ft3;
		wxFontEnumerator fte;
		fte.EnumerateFacenames();
		wxArrayString as=fte.GetFacenames();

		/*wofstream fileout("fontlist.txt");
		fileout.imbue(std::locale("chs"));
		vector<wstring> wsvec;
		wsvec.emplace_back(L"字体1");
		wstring wstr(L"字体2");
		for (int i = 0; i < as.size(); i++) {
			fileout << as[i].wc_str() << endl;
		};
		fileout.close();*/

		//wxMessageBox(ft2.GetFaceName(), L"字体数量");
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
