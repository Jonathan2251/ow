#pragma warning(disable:4786)
#include<fstream>
#include<iostream>
#include<string>
#include<conio.h>
#include "Graph.h"

template <class T>
void initial(EdgeGraph<T>& g, ifstream& is) { //依輸入檔名建立graph
	int vertexNo;
	is >> vertexNo;
	int edgesNo;
	is >> edgesNo;
	for (;!is.eof(); ) {
		T v1;
		is >> v1;
		T v2;
		is >> v2;
		int cost;
		is >> cost;
		g.insertEdge(v1,v2,cost);
	}
}

void showMenu() { //顯示主選單
	cout << "(1)Display the graph" << endl;
	cout << "(2)Find the biconnected components" << endl;
	cout << "(3)Find the shortest path between two vertices" << endl;
	cout << "(4)Find the minimum cost spanning tree" << endl;
	cout << "(5)Insert a new edge" << endl;
	cout << "(6)Delete an existing edge" << endl;
	cout << "(7)Update the cost of an existing edge" << endl;
	cout << "(8)Exit" << endl;
}

template <class T>
void displayGraph(EdgeGraph<T>& g) {	//依題目輸出結果格式印出graph
	cout << endl;
	for (int i = 0; i < g.head.size(); i++) {
		cout << " The edge incident on vertex " << g.head[i].name << ":";
		list<EdgeV2>::iterator p = g.head[i].link->begin();
		for (; p != g.head[i].link->end(); p++) {
			cout << g.head[i].name << "," << g.head[p->v2].name << " ";
		}
		cout << endl;
	}
}

template <class T>
void bicomponents(DirectGrpah<T>& g) {	//依題目輸出結果格式印出shortest path
	DirectGrpah<T>::BCIterator p;
	p = &g;
	int j;
	cout << endl;
	for (j = 1, p.first(); !p.isDone(); j++, p.next()) {
		cout << "The biconnected component " << j << ":";
		list<Edge<T> > l = p.current();
		list<Edge<T> >::iterator i;
		for (i = l.begin(); i != l.end(); i++) {
			cout << i->v1 << "," << i->v2 << " ";
		}
		cout << endl;
	}
}

template <class T>
void shortestPath(DirectGrpah<T>& g) {	//依題目輸出結果格式印出shortest path
	cout << endl;
	cout << "Please input the from and destination:" << endl;
	cout << "  Please input the from vertex:";
	T i;
	cin >> i;
	cout << "  Please input the destination vertex:";
	T j;
	cin >> j;
	DirectGrpah<T>::shortestPathIterator p;
	p = &g;
	if (!p.shortestPath(i,j)) {
		cout << "路徑不存在！" << endl;
		return;
	}
	if (i == j) {
		cout << "起始vertex與終止vertex相同" << endl;
		return;
	}
	cout << "The shortest path from " << i << " to " << j << endl;
	if (!p.isDone()) {
		int totalCost = 0;
		p.first();
		T old = p.current();
		if (!p.isDone()) {
			for (p.next(); !p.isDone(); p.next()) {
				cout << old << "," << p.current() << " ";
				totalCost += g.getEdgeCost(old, p.current());
				old = p.current();
			}
		}
		cout << endl;
		cout << "The cost of this path is:" << totalCost << endl;
	}
}

template <class T>
void MCST(DirectGrpah<T>& g) {	//依題目輸出結果格式印出minimum cost spanning tree
	DirectGrpah<T>::MCSTIterator p;
	p = &g;
	cout << endl;
	if (g.head.size() < 2) {
		cout << "圖形只有一vretex，故無minimum cost spanning tree" << endl;
		return;
	}
	if (p.isDone()) {
		cout << "圖形不為connect，故無minimum cost spanning tree" << endl;
		return;
	}
	cout << "The minimum cost spanning tree is:";
	int totalCost = 0;
	for (p.first(); !p.isDone(); p.next()) { //遊歷minimum cost spanning tree，一次一根edge
		Edge<T> edge = p.current(); //get edge
		cout << edge.v1 << "," << edge.v2 << " ";
		totalCost += edge.cost;
	}
	cout << endl;
	cout << "The cost of this spanning tree is:" << totalCost << endl;
}

template <class T>
void insertEdge(EdgeGraph<T>& g) {	//依題目輸入edge格式輸入一根edge，加進graph g中
	cout << endl;
	cout << "Please input the adjacent vertices of the edge:" << endl;
	cout << "    Please input the first vertex:";
	T v1;
	cin >> v1;
	cout << "    Please input the second vertex:";
	T v2;
	cin >> v2;
	cout << "    Please input the cost of the edge:";
	int cost;
	cin >> cost;
	if (!g.insertEdge(v1,v2,cost)) cout << "欲insert之edge已存在" << endl;
}

template <class T>
void deleteEdge(EdgeGraph<T>& g) {	//依題目輸入edge格式輸入一根edge，從graph g中刪除此edge
	cout << endl;
	cout << "Please input the adjacent vertices of the edge:" << endl;
	cout << "    Please input the first vertex:";
	T v1;
	cin >> v1;
	cout << "    Please input the second vertex:";
	T v2;
	cin >> v2;
	if (!g.deleteEdge(v1,v2)) cout << "欲delete之edge不存在" << endl;
}

template <class T>
void updateCost(EdgeGraph<T>& g) {	//依題目輸入格式更改edge cost
	cout << endl;
	cout << "Please input the adjacent vertices of the edge:" << endl;
	cout << "    Please input the first vertex:";
	T v1;
	cin >> v1;
	cout << "    Please input the second vertex:";
	T v2;
	cin >> v2;
	cout << "    Please input the cost of the edge:";
	int cost;
	cin >> cost;
	if (!g.updateCost(v1,v2,cost)) cout << "欲updateCost之edge不存在" << endl;
}

void main() {
	EdgeGraph<int> g;

	cout << "Please input the file of graph:"; //依輸入檔案的資料建立graph
	string str;
	cin >> str;
	ifstream is(&str[0]);
	if (!is) {
		cout << "找不到該輸入檔案" << endl;
		return;
	}
	initial(g, is); //依輸入檔名建立graph
	is.close();

	showMenu(); //顯示主選單
	cout << "Please input the function number(1-8):";
	int choice;
	cin >> choice;
	if (choice < 1 || choice > 8) cout << "輸入選擇錯誤（不是1到8）" << endl;
	while (choice != 8) {
		switch(choice) {
		case 1:
			displayGraph(g);	//依題目輸出結果格式印出graph
			break;
		case 2:
			bicomponents(g);	//依題目輸出結果格式印出shortest path
			break;
		case 3:
			shortestPath(g);	//依題目輸出結果格式印出minimum cost spanning tree
			break;
		case 4:
			MCST(g);	//依題目輸出結果格式印出minimum cost spanning tree
			break;
		case 5:
			insertEdge(g);
			break;
		case 6:
			deleteEdge(g);
			break;
		case 7:
			updateCost(g);
			break;
		case 8:
			break;
		}
		cout << "Please press any key to return..." << endl;
		getch();
		showMenu();
		cout << "Please input the function number(1-8):";
		cin >> choice;
		if (choice < 1 || choice > 8) cout << "輸入選擇錯誤（不是1到8）" << endl;
	}
}