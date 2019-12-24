#include<eigen3/Eigen/core>
#include<random>
#include<iostream>
#include<utility>
#include<string>
#include<wx/wx.h>
#include<queue>
#include <limits>
#include <wx/log.h> 
using namespace std;

using namespace Eigen;

typedef Array<bool, Dynamic, Dynamic> MapData;
typedef pair<int, int> IntPoint;

class MapManager {
	MapData mapd;
	IntPoint start, end;
	int n;
public:
	MapManager(int n, double thr) :n(n) {
		mapd = random_map(n, thr);
	};
	inline bool query(int i, int j) {
		if (i<0 || i>n - 1 || j<0 || j>n - 1) {
			return true;
		} else {
			return mapd(i, j);
		};
	};
	inline int rows() {
		return mapd.rows();
	};
	inline int cols() {
		return mapd.cols();
	};
	inline IntPoint GetStart() {
		return start;
	};
	inline IntPoint GetEnd() {
		return end;
	};
	//Randomly generate a bool matrix as a map. Element true for obstacle.
	MapData random_map(int n, double thr) {
		MapData data;
		data.resize(n, n);

		MatrixXd rnd(n, n);
		rnd.setRandom();
		rnd = ((rnd.array() + 1) / 2).matrix();

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				data(i, j) = rnd(i, j) < thr ? true : false;
			};
		};
		return data;
	};

	//Find start and end point.

	pair<IntPoint, IntPoint> StartEnd() {
		int n = mapd.rows();
		bool flag1 = true, flag2 = true;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (flag1) {

					if (!mapd(i, j)) {
						start = make_pair(i, j);
						flag1 = false;
					};
				};
				if (flag2) {
					if (!mapd(n - 1 - i, n - 1 - j)) {
						end = make_pair(n - 1 - i, n - 1 - j);

						flag2 = false;
					};
				};
				if (!flag1 & !flag2) {
					break;
				};
			};
		};
		//cout << get<0>(s)<<" "<<get<1>(s) << endl;
		return make_pair(start, end);
	};
};
//A struct to store node.
struct Node {
	int row, col, f;
};
template<>
struct greater<Node*> {
	constexpr bool operator()(const Node* lhs, const Node* rhs) const
	{
		return lhs->f > rhs->f;
	}
};
//Find path.
class PathFinder {
private:
	IntPoint start, end;
	bool success = false;
	int n;
	//	auto f = [](const Node* &lhs, const Node* &rhs) {return lhs->f > rhs->f; };
	priority_queue < Node*, vector<Node*>, greater<Node*> > q;
	MatrixXi f, g;
	Node* current;
	MapData closed, open;
	vector<Node*> neighbor;
	//This map use x and y as key. We don't have to store pointers of all nodes.
	vector<vector<Node*>> pointer_map;
	MapManager* mp;
	bool noValidPath = false;
public:
	void AddSingleNeighbor(int i, int j) {
		if (!mp->query(i, j)) {
			//No obstacle.
			Node* p = pointer_map[i][j];
			if (p== nullptr) {
				//A nullptr, then create new pointer.
				neighbor.emplace_back(new Node{ i,j,f(i,j) });
			} else {
				neighbor.emplace_back(p);
			};
		};

	};
	void FindNeighbor(Node* node) {
		neighbor.clear();

		int x = node->row;
		int y = node->col;
		AddSingleNeighbor(x - 1, y);
		AddSingleNeighbor(x, y - 1);
		AddSingleNeighbor(x, y + 1);
		AddSingleNeighbor(x + 1, y);
	};

	inline int HFun(int x0, int y0) {
		return abs(x0 - end.first) + abs(y0 - end.second);
	};

	inline void SetClosed(Node* node) {
		closed(node->row, node->col) = true;
	};
	inline bool NoValidPath() {
		return noValidPath;
	}
	IntPoint NextStep() {
		if (q.size() == 0) {
			noValidPath = true;
			return end;
		};

		if (!success) {
			current = q.top();
			int cx = current->row, cy = current->col;
			if ((cx == end.first) && (cy == end.second)) {
				success = true;
				return end;
			};
			q.pop();
			open(cx, cy) = false;

			SetClosed(current);
			FindNeighbor(current);
			//Loop over neighbors.
			int x, y;
			for (int i = 0; i < neighbor.size();i++) {
				Node* ptr = neighbor[i];
				x = ptr->row;
				y = ptr->col;
				
				if (closed(x, y)) {
					continue;
				};

				int tscore = g(cx, cy) + 1;
				if (tscore < g(x, y)) {
					g(x, y) = tscore;
					f(x, y) = g(x, y) + HFun(x, y);
					if (!open(x, y)) {
						open(x, y) = true;
						q.push(ptr);
					};
				};
			};
			return IntPoint{ current->row,current->col };

		} else {
			return end;
		}
	};

	PathFinder(MapManager* m) :
		mp(m), start(m->GetStart()), end(m->GetEnd()), n(m->rows()) {
		closed.resize(n, n);

		open.resize(n, n);
		closed.setConstant(false);
		open.setConstant(false);

		f.resize(n, n);
		g.resize(n, n);
		f.setConstant(numeric_limits<int>::max());
		f(start.first, start.second) = HFun(start.first, start.second);
		g.setConstant(numeric_limits<int>::max());
		g(start.first, start.second) = 0;

		pointer_map.resize(n, vector<Node*>(n, nullptr));
		pointer_map[start.first][start.second] = new
			Node{ start.first, start.second,f(start.first, start.second) };

		q.push(pointer_map[start.first][start.second]);

	};

	bool finded() {
		return success;
	};
};

//A frame to plot grids.
class myFrame : public wxFrame {
private:
	MapManager* m;
	pair<IntPoint, IntPoint> p;
	PathFinder* pf;

	int nGrid;
	int regionStartX, regionStartY, regionHeight, regionWidth;

	void DrawCell(int i, int j, wxColour col) {
		wxPaintDC dc(this);
		dc.SetBrush(wxBrush(col)); // green filling
		dc.SetPen(wxPen(wxColour(255, 255, 255), 0));
		dc.DrawRectangle(regionStartX + j * 30, regionStartY + i * 30, 30, 30);


	};

public:
	void OnPaint(wxPaintEvent& evt) {
		//First draw a line frame.
		wxPaintDC dc(this);
		dc.SetPen(wxPen(wxColour(255, 255, 255), 2));
		dc.DrawRectangle(regionStartX, regionStartY, nGrid * 30, nGrid * 30);
	

		//Then draw map.
		for (int i = 0; i < nGrid; i++) {
			for (int j = 0; j < nGrid; j++) {
				if (m->query(i, j)) {
					DrawCell(i, j, wxColour(0, 0, 0));
				};
			};
		};
		DrawCell(m->GetStart().first, m->GetStart().second, wxColour(255, 0, 0));
		DrawCell(m->GetEnd().first, m->GetEnd().second, wxColour(0, 255, 0));
		//Run path finder.
		while (!pf->finded() && !pf->NoValidPath()) {
			//Not finded and there are path.
			IntPoint c = pf->NextStep();
			if (pf->finded()) {
				break;
			};
			DrawCell(c.first, c.second, wxColour(255, 0, 0));
			wxMilliSleep(25);
		};
	};
	myFrame(wxWindow* parent, wxWindowID id, const wxString& title, int n) :
		wxFrame(parent, id, title, wxPoint(500, 40), wxSize(500, 500)), nGrid(n),
		regionStartX((int)(1920 - n * 30) / 2), regionStartY((int)(1080 - n * 30) / 2),
		regionWidth(n * 30), regionHeight(n * 30) {
		m = new MapManager(n, 0.2);
		p = m->StartEnd();
		pf = new PathFinder(m);

	};

	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(myFrame, wxFrame)

// catch paint events
EVT_PAINT(myFrame::OnPaint)

END_EVENT_TABLE()
//wxApp
class myApp :public wxApp {

	bool OnInit() {
		myFrame* fr = new myFrame((wxWindow*)NULL, wxID_ANY, "A-star path finder", 30);
		//wxFrame* fr= new myFrame((wxWindow*)NULL, wxID_ANY, "A-star path finder", 30);
		fr->Maximize();
		fr->Show();
		return true;
	};
public:

};

IMPLEMENT_APP(myApp)
