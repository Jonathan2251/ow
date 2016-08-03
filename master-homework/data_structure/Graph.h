#include<list>
#include<vector>
#include<map>
#include<stack>
#include<set>

using namespace std;

namespace graph {
	const int null = 0;
	template <class T>	struct Edge {
		T v1;
		T v2;
		int cost;
		Edge() { };
		Edge(T aV1, T aV2, int aCost) : v1(aV1), v2(aV2), cost(aCost) { };
	};
	struct VEdge {
		int v1;
		int v2;
		int cost;
		VEdge() { };
		VEdge(int aV1, int aV2, int aCost) : v1(aV1), v2(aV2), cost(aCost) { };
		friend bool operator<(const VEdge& e1, const VEdge& e2) {
			if (e1.v1 < e2.v1) return true;
			else if (e1.v1 > e2.v1) return false;
			else if (e1.v2 < e2.v2) return true;
			else return false;
		};
		friend bool operator==(const VEdge& e1, const VEdge& e2) {
			return e1.v1 == e2.v1 && e1.v2 == e2.v2;
		};
	};
	struct EdgeV2 {
		int v2;
		int cost;
	};
	template <class T>	struct Node {
		T name;
		list<EdgeV2>* link;
	};
}

using namespace graph;

// 抽象化類別,提供一致性介面以去遊歷某一物件
// abstract class, interface for searching step by step.  
template <class T>
class Iterator {
public:
    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() = 0;
    virtual T current() const = 0;
protected:
    Iterator() { };
};

template <class T>
class DirectGrpah { //有向圖：edge有方向性，同一對頂點不可有兩條edge
public:
	vector<Node<T> > head;
	map<T,int> v; //vertex name 與 vertex index 對照的 table
	virtual int getVertex(T vName) { //傳回該vertex在vector中的index,若無此vertex傳回-1
		map<T,int>::iterator p = v.find(vName);
		if (p != v.end()) {
			return v[vName];
		}
		else {
			return -1;
		}
	};
	virtual vector<Node<T> >::iterator getVertexAddress(T vName) {
		map<T,int>::iterator p = v.find(vName);
		if (p != v.end()) return &head[v[vName]];
		else return head.end();
	};
	virtual bool findVertex(T vName) { //傳回vertex名稱 vName是否存在
		if (getVertex(vName) == -1) return false;
		return true;
	};
	virtual list<EdgeV2>::iterator getEdge(T v1Name, T v2Name) {  //若edge(v1Name,v2Name)存在則傳回該edge對應的list結構之指標，否則傳回該edge對應的list結構結尾之指標(head[i].link->end())，
		int i = getVertex(v1Name);						//注意：使用時須確定vertex v1Name存在，否則傳回值將不確定
		if (i != -1) {
			list<EdgeV2>::iterator p = head[i].link->begin();
			for ( ; p != head[i].link->end(); p++) {
				if (head[p->v2].name == v2Name) return p;
			}
		}
		return head[i].link->end();
	};
	virtual int getEdgeCost(T v1Name, T v2Name) { //edge存在則return該edge cost，否則return 32767(0x7fff)
		if (findEdge(v1Name,v2Name)) {
			list<EdgeV2>::iterator i = getEdge(v1Name,v2Name);
			return i->cost;
		}
		else return 0x7fff;
	};
	virtual int GetEdgeCost(int v1, int v2) { //edge存在則return該edge cost，否則return 32767(0x7fff)
		if (v1 <= head.size() && v2 <= head.size()) {
			list<EdgeV2>::iterator i = head[v1].link->begin();
			for (; i != head[v1].link->end(); i++)
				if (i->v2 == v2) return i->cost;
		}
		return 0x7fff;
	};
	virtual bool findEdge(T v1Name, T v2Name) { //vertex名稱 v1Name、v2Name皆存在且edge(v1Name,v2Name)存在，才為true
		int i = getVertex(v1Name);
		int j = getVertex(v2Name);
		if (i != -1 && j != -1) {
			list<EdgeV2>::iterator p = head[i].link->begin();
			for (; p->v2 != j && p != head[i].link->end(); p++);
			if (p != head[i].link->end()) return true;
		}
		return false;
	};
	virtual bool insertVertex(T vName) { //vertex名稱 vName不存在才插入
		if (findVertex(vName)) return false;
		else { 
			Node<T> t;
			t.name = vName;
			t.link = graph::null;
			head.push_back(t);
			v[vName] = head.size() - 1;
			return true;
		}
	};
	virtual bool insertEdge(T v1Name, T v2Name, int cost) { //vertex名稱 v1Name、v2Name皆存在且edge(v1Name,v2Name)不存在，才可插入
		if ((findVertex(v1Name))&&(findVertex(v2Name))) {
			int i = getVertex(v1Name);
			if (head[i].link == graph::null) {
				int j = getVertex(v2Name);
				EdgeV2 e;
				e.v2 = j;
				e.cost = cost;
				head[i].link = new list<EdgeV2>;
				head[i].link->push_back(e);
				return true;
			}
			else {
				if (!findEdge(v1Name, v2Name)) {
					int j = getVertex(v2Name);
					EdgeV2 e;
					e.v2 = j;
					e.cost = cost;
					head[i].link->push_back(e);
					return true;
				}
				else return false;
			}
		
		}
		else return false;
	};
	virtual bool deleteVertex(T vName) {
		vector<Node<T> >::iterator p = getVertexAddress(vName);
		if (p != head.end()) {
			delete p->link;
			head.erase(p);
			int j = getVertex(vName); //修改head中edge之index
			for (int i1 = 0; i1 < head.size(); i1++) {
				list<EdgeV2>::iterator i2 = head[i1].link->begin();
				for (; i2 != head[i1].link->end(); i2++) {
					if (i2->v2 >= j) i2->v2--;
				}
			}
			map<T,int>::iterator i = v.find(vName); //修改 table v
			v.erase(i);
			for (; p != head.end(); p++) {
				v[p->name]--;
			}
			return true;
		}
		else return false;
	};
	virtual bool deleteEdge(T v1Name, T v2Name) { //vertex名稱 v1Name存在且edge(v1Name,v2Name)存在，才刪除
		int i = getVertex(v1Name);
		if (i != -1) {
			list<EdgeV2>::iterator p = getEdge(v1Name, v2Name);
			if (p != head[i].link->end()) {
				head[i].link->erase(p);
				return true;
			}
		}
		return false;
	};
	virtual bool updateCost(T v1Name, T v2Name, int cost) { //vertex名稱 v1Name存在且edge(v1Name,v2Name)存在，才可更改cost
		int i = getVertex(v1Name);
		if (i != -1) {
			list<EdgeV2>::iterator p = getEdge(v1Name, v2Name);
			if (p != head[i].link->end()) {
				p->cost = cost;
				return true;
			}
		}
		return false;
	};
	class BCIterator : public Iterator<list<Edge<T> > > { //找出biconnected components
	private:
		stack<VEdge> s;
		list<list<Edge<T> > > bc; //biconnected component
		list<list<Edge<T> > >::iterator p;
		DirectGrpah<T>* g; 
		int *visited, *dfn, *low;
		int num;
		int min(int a, int b) {
			if (b < a) return b;
			else return a;
		}
		void bicon(int u, int v) {
			list<EdgeV2>::iterator ptr;
			int w;
			dfn[u] = low[u] = num++;
			for (ptr = g->head[u].link->begin(); ptr != g->head[u].link->end(); ptr++) {
				w = ptr->v2;
				if (v != w && dfn[w] < dfn[u]) {
					VEdge t(u,w,0);
					s.push(t);
					visited[u] = true;
					if (dfn[w] < 0) //{
						bicon(w,u);
						low[u] = min(low[u],low[w]);
						if (low[w] >= dfn[u]) {
							VEdge ve;
							list<Edge<T> > l;
							do {
								ve = s.top();
								Edge<T> e;
								e.v1 = g->head[ve.v1].name;
								e.v2 = g->head[ve.v2].name;
								e.cost = g->getEdgeCost(e.v1,e.v2);
								s.pop();
								l.push_back(e);
							} while (!((ve.v1 == u) && (ve.v2 == w)));
							bc.push_back(l);
						}
				//	}
					else if (w != v) low[u] = min(low[u],dfn[w]);
				}
			}
		};
	public:
		void operator=(DirectGrpah<T>* aG) {
			g = aG;
			int size = g->head.size();
			visited = new int[size];
			dfn = new int[size];
			low = new int[size];
			num = 0;
			for (int i = 0; i < g->head.size(); i++) {
				visited[i] = false;
				dfn[i] = low[i] = -1;
			}
			for (i = 0; i < size; i++) {
				if (visited[i] == false) {
					if (g->head[i].link->size() == 0) {
						Edge<T> e;
						e.v1 = g->head[i].name;
						e.v2 = g->head[i].name;
						e.cost = g->getEdgeCost(e.v1,e.v2);
						list<Edge<T> > l;
						l.push_back(e);
						bc.push_back(l);
					}
					else bicon(i,-1);
				}
			}
		};
		void first() {
			p = bc.begin();
		};
		void next() {
			if (p != bc.end()) p++;
		};
		bool isDone() { return p == bc.end(); };
		list<Edge<T> > current() const {
			return *p;
		};
	};
	class shortestPathIterator : public Iterator<T> {	//找最短路徑的iterator
	private:
		DirectGrpah<T>* g;
		list<int> path; //存shortest path所經過的vertex編號
		list<int>::iterator p;
		int cost;

		int choose(int distance[], int n, bool found[]) { //find the smallest distance not yet checked
			int i, min, minpos;
			min = 32767;
			minpos = -1;
			for (i = 0; i < n; i++) {
				if (distance[i] < min && !found[i]) {
					min = distance[i];
					minpos = i;
				}
			}
			return minpos;
		};
	public:
		void operator=(DirectGrpah<T>* aG) {
			g = aG;
		};
		bool shortestPath(T v1Name, T v2Name) { //找出v1Name到v2Name的最短路徑
			path.clear();
			int n = g->head.size();
			int *distance = new int[n];
			bool *found = new bool[n];
			int *from = new int[n];
			int v1 = g->getVertex(v1Name);
			int v2 = g->getVertex(v2Name);
			if (v1 == -1 || v2 == -1) return false;
			for (int i = 0; i < n; i++) {
				found[i] = false;
				distance[i] = 32767;
				from[i] = -1;
			}
			distance[v1] = 0;
			from[v1] = -1;
			for (i = 0; i < n-1; i++) {
				int u = choose(distance,n,found); //從目前走過的頂點之集合T中，找出可抵達之最短路徑(u,v)
				if (u == -1) return false;
				found[u] = true;
				for (int w = 0; w < n; w++) {
					if (!found[w]) {
						if (distance[u] + g->GetEdgeCost(u,w) < distance[w]) {
							distance[w] = distance[u] + g->GetEdgeCost(u,w);
							from[w] = u; //記錄來源節點
						}
					}
				}
			}
			cost = distance[v2]; //記錄距離distance(v1Name,v2Name)
			for (i = v2; i != -1; i = from[i]) path.push_front(i); //將所走過的vertex編號放進list(path)中
			delete distance;
			delete found;
			delete from;
			return true;
		};
		void first() {
			p = path.begin();
		};
		void next() { p++; };
		bool isDone() { return p == path.end(); };
		T current() const { //return vertex名稱
			return g->head[*p].name; 
		};
	};
	class MCSTIterator : public Iterator<Edge<T> > {	//minimum cost spanning tree iterator, graph裡edge的cost必須 < 0x7fff
	private:											//若圖形不為connect，則不產生minimum cost spanning tree
		DirectGrpah<T>* g;
		set<VEdge> TE; //存minimum cost spanning tree
		set<VEdge>::iterator p;

		VEdge leastCostEdge(set<int>& TV) { //(edge.v1,edge.v2)是最小cost的edge，其中edge.v1屬於TV集合、edge.v2不屬於TV集合
			VEdge edge;
			edge.v1 = -1;
			edge.v2 = -1;
			edge.cost = 0x7fff;
			set<int>::iterator i = TV.begin();
			for (; i != TV.end(); i++) {
				list<EdgeV2>::iterator p = g->head[*i].link->begin();
				for (; p != g->head[*i].link->end(); p++) {
					if (TV.find(p->v2) == TV.end() && (p->cost < edge.cost)) {
						edge.v1 = *i;
						edge.v2 = p->v2;
						edge.cost = p->cost;
					}
				}
			}
			return edge;
		};
	public:
		void operator=(DirectGrpah<T>* aG) { //依graph aG建立minimum cost spanning tree
			g = aG;
			TE.clear();
			set<int> TV;
			if (g->head.size() != 0) {
				TV.insert(0);
				int n = g->head.size();
				for (int i = 0; i < n - 1; i++) {
					VEdge edge = leastCostEdge(TV); //(edge.v1,edge.v2)是最小cost的edge，其中edge.v1屬於TV集合、edge.v2不屬於TV集合
					if (edge.v1 == -1) break; //找不到該edge
					TV.insert(edge.v2); //將edge.v2加入set TV中
					TE.insert(edge);
				}
				if (i < n - 1) TE.clear(); //圖形不為connect，故無法找到minimum cost spanning tree
			}
			p = TE.begin();
		};
		void first() {
			p = TE.begin();
		};
		void next() { p++; };
		bool isDone() { return p == TE.end(); };
		Edge<T> current() const {
			Edge<T> edge;
			edge.v1 = g->head[p->v1].name;
			edge.v2 = g->head[p->v2].name;
			edge.cost = p->cost;
			return edge; 
		};
	};
};

template <class T>
class UndirectGrpah : public DirectGrpah<T> { //無向圖：edge無方向性，同一對頂點不可有兩條edge
public:
	virtual bool insertEdge(T v1Name, T v2Name, int cost) { 
		if (DirectGrpah<T>::insertEdge(v1Name,v2Name,cost) && DirectGrpah<T>::insertEdge(v2Name,v1Name,cost)) return true;
		else return false;
	};
	virtual bool deleteEdge(T v1Name, T v2Name) { 
		if (DirectGrpah<T>::deleteEdge(v1Name,v2Name) && DirectGrpah<T>::deleteEdge(v2Name,v1Name)) return true;
		else return false;
	};
	virtual bool updateCost(T v1Name, T v2Name, int cost) {  
		if (DirectGrpah<T>::updateCost(v1Name,v2Name,cost) && DirectGrpah<T>::updateCost(v2Name,v1Name,cost)) return true;
		else return false;
	};
};

template <class T>
class EdgeGraph : public UndirectGrpah<T> { //以edge為導向的graph
public:
	bool insertEdge(T v1Name, T v2Name, int cost) { //輸入edge(v1,v2,cost)，若v1或v2不存在，則加進vertex v1或v2
		UndirectGrpah<T>::insertVertex(v1Name);
		UndirectGrpah<T>::insertVertex(v2Name);
		return UndirectGrpah<T>::insertEdge(v1Name,v2Name,cost);
	};
	bool deleteEdge(T v1Name, T v2Name) { //刪除edge(v1,v2)，若v1或v2沒有edge incident on，則刪除vertex v1或v2
		if (UndirectGrpah<T>::deleteEdge(v1Name,v2Name)) {
			int i = UndirectGrpah<T>::getVertex(v1Name);
			if (head[i].link->size() == 0) {
				UndirectGrpah<T>::deleteVertex(v1Name);
			}
			i = UndirectGrpah<T>::getVertex(v2Name);
			if (head[i].link->size() == 0) {
				UndirectGrpah<T>::deleteVertex(v2Name);
			}
			return true;
		}
		else return false;
	};
};
