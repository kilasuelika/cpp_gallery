// ConsoleApplication9.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stack>
using namespace std;

vector<string> x{"总公司","子公司1","子公司2","部门1","部门1", "部门2","部门2"};
vector<int> y{-1,0,0,1,2,1,2};

//Given a node, use back track method to get it's full level names.
vector<string> back_track_name(int node) {
	vector<string> name;
	stack<string> temp_name;

	while (node != -1) {
		temp_name.push(x[node]);
		node = y[node];
	};

	while (temp_name.size() != 0) {
		string temp = temp_name.top();
		name.push_back(temp);
		temp_name.pop();
	};
	return name;
};

int main()
{
	//Construct a tree.
	map<int, vector<int>> data;
	for (int i = 0; i < y.size(); i++) {
		data[y[i]].push_back(i);
	};

	//A vector of strings to store content for each row.
	vector<vector<string>> order_data;

	queue<int> q;
	for (int ele : data[-1]) {
		q.push(ele);
	};
	//Level order traverse using a queue.
	while (q.size() != 0) {
		int temp = q.front();
		q.pop();
		for (int ele : data[temp]) {
			//If a leaf.
			if (data[ele].size() == 0) {
				order_data.push_back(back_track_name(ele));
			}
			else {
				q.push(ele);
			};
		};
	};

	//Print data.
	for (auto& elex : order_data) {
		for (auto& eley : elex) {
			cout << eley << " ";
		}
		cout << endl;
	}
}
