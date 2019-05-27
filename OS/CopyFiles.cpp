// cpyf.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <exception>
#include <experimental/filesystem>
#include <filesystem>
#include <fstream>
#include <string>
#include <boost/algorithm/string/replace.hpp>

using namespace std::experimental::filesystem::v1;
using namespace std;

string sline ="//Sline:---------------------------------------------------";

void setup_cd(path file){
	ofstream ofile(file.string(), ios::out);
	string path = file.parent_path().string();
	boost::replace_all(path, "\\", "\\\\");

	if(file.filename().string() =="data.do"){
		ofile<<"cd \""<< path <<"\""<<endl;
		ofile<<endl;
		ofile<<"import excel using \"\",first clear"<<endl;
		ofile<<endl;
		ofile<<sline<<endl;
		ofile.close();
		return;
	};
	if(file.filename().string() =="main.do"){
		ofile<<"cd \""<< path<<"\""<<endl;
		ofile<<endl;
		ofile<<sline<<endl;
		ofile.close();
		return;
	};
	if(file.filename().string() =="main.r"){
		ofile<<"setwd(\""<< path <<"\")"<<endl;
		ofile<<endl;
		ofile<<sline<<endl;
		ofile.close();
		return;
	};
}
int main()
{
	path source = "f:\\work\\template";
	path target = current_path();
	
	try {
		copy(source, target, copy_options::recursive|copy_options::overwrite_existing);
		cout << "Copy finished!" << endl;

		setup_cd(target/ "programming/main.do");
		setup_cd(target / "programming/data.do");
		setup_cd(target / "programming/main.r");

		cout<<"Modify cd path finished!"<<endl;
	}
	catch (std::exception& e) { // Not using fs::filesystem_error since std::bad_alloc can throw too.
		// Handle exception or use error code overload of fs::copy.
		cout << "Error." << endl;
	}
}
