// Filesystem1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;
using namespace std::filesystem;

int main()
{
	path p = current_path();
	path input_p = p.parent_path() / "data/prod.csv";
	path output_p = p.parent_path() / "data/prod_transpose.csv";

	ifstream ifile(input_p.string(), ios::in);
	ofstream ofile(output_p.string(), ios::out);

	//count cols.
	string line;
	getline(ifile, line);
	ofile << "bankname, year, prod" << endl;

	//read and write.
	int col = 0;
	while (getline(ifile, line)) {
		stringstream ss(line);
		string cell;
		string rowid;
		while (getline(ss, cell, ',')) {
			++col;
			switch (col) {
				case 1:
					rowid = cell;
					//cout << rowid << endl;
					break;
				case 11:
					ofile << rowid << "," << 2006 + col << "," << cell << endl;
					//cout << col << endl;
					//cout << rowid << endl;
					col = 0;
					break;

				default:
					ofile << rowid << "," << 2006 + col << "," << cell << endl;
					//cout << col << endl;
					//cout << rowid << endl;
					break;
			};
		}
	};

	ifile.close();
	ofile.close();
	cin >> line;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
