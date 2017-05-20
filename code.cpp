#include <iostream>
#include <cstring>
#include <set>
#include <vector>

using namespace std;
typedef long long int LL;
typedef pair<int, int> PII;
typedef vector<int> VI;
typedef vector< PII > VPII;
const int DIM = 50;
int N = 50;
int id = 0;
struct kdtree {
	kdtree *left, *right;
	int di[DIM];
};


/** Sorting comparison funtion */
bool cmp(kdtree* a, kdtree* b) {
	return a->di[id] < b->di[id];
}


/** Computes squared distance between two points */
int distance(kdtree* a, kdtree* b) {
	int val = 0;
	for (int i = 0 ; i < DIM; i ++) {
		int t = a->di[i] - b->di[i];
		val += t * t;
	}
	return val;
}
kdtree* static_build(kdtree *rootaddr, vector<kdtree*> &vec, int depth, int s, int e) {
	if (e < s)
		return NULL;
	if (s == e) {
		rootaddr = (kdtree*)(malloc(sizeof(kdtree)));
		for (int i = 0 ; i < DIM; i ++) {
			rootaddr->di[i] = vec[s]->di[i];
		}
		rootaddr->left = NULL;
		rootaddr->right = NULL;
	}
	else {
		id = depth % DIM;
		sort(vec.begin() + s, vec.begin() + e + 1, cmp);
		rootaddr = (kdtree*)(malloc(sizeof(kdtree)));
		int t = (s + e)/2;
		for (int i = 0 ; i < DIM; i ++) {
			rootaddr->di[i] = vec[t]->di[i];
		}
		rootaddr->left = static_build(rootaddr->left, vec, depth + 1, s, (s + e) / 2 - 1);
		rootaddr->right = static_build(rootaddr->right, vec, depth + 1, (s + e) / 2 + 1, e);
	}
	return rootaddr;
}
kdtree* dynamic_build(kdtree *rootaddr, kdtree* p, int depth) {

	if (rootaddr == NULL) {
		rootaddr = (kdtree*)(malloc(sizeof(kdtree)));
		for (int i = 0 ; i < DIM; i ++) {
			rootaddr->di[i] = p->di[i];
		}
		rootaddr->left = NULL;
		rootaddr->right = NULL;
	}
	else {
		id = depth % DIM;
		if (p->di[id] < rootaddr->di[id])
			dynamic_build(rootaddr->left, p, depth + 1);
		else
			dynamic_build(rootaddr->right, p, depth + 1);
	}
	return rootaddr;
}
set<pair<int, kdtree*> >s;
void nearest_neighbour(kdtree *rootaddr, kdtree* &best_node, int &best_dist, kdtree* p, int depth) {
	if (rootaddr == NULL)
		return;
	int dis = distance(p, best_node);
	if (dis < best_dist) {
		best_dist = distance(p, best_node);
		best_node = rootaddr;
	}
	s.insert(make_pair(dis, rootaddr));
	id = depth % DIM;
	if (p->di[id] < rootaddr->di[id]) {
		nearest_neighbour(rootaddr->left, best_node, best_dist, p, depth + 1);
		if (abs(rootaddr->di[id] - p->di[id]) < best_dist || s.size() < N)
			nearest_neighbour(rootaddr->right, best_node, best_dist, p, depth + 1);
	}
}
void print(kdtree *nodeaddr, int depth, char c) {
	if (nodeaddr == NULL)
		return;
	//cout << nodeaddr->x << " " << nodeaddr->y << " " << depth << " " << c << "\n";
	print(nodeaddr->left, depth + 1, 'l');
	print(nodeaddr->right, depth + 1, 'r');
}

void delete_tree(kdtree *nodeaddr) {
	if (nodeaddr == NULL)
		return;
	delete_tree(nodeaddr->left);
	delete_tree(nodeaddr->right);
	/** free the memory location associated with the node */
	free(nodeaddr);
}
int main() {
	freopen("input.txt", "r", stdin);
	int blk = 500;
	int Q;
	kdtree* rootaddr;
	cin >> Q;
	vector<kdtree*> vec;
	for (int i = 0 ; i < Q; i ++) {
		cout << "Enter the point" << endl;
		kdtree* xy;
		int x;
		for (int j = 0; j < DIM; j++) {
			cin >> x;
			xy->di[j] = x;
		}
		cout << "Enter the number of nearest neighbours" << endl;
		cin >> N;
		s.clear();
		kdtree *loc;
		 x = (int)(1e9) + 10;
		set<pair<int, kdtree*> > ::iterator it;
		nearest_neighbour(rootaddr, loc, x, xy, 0);
		vector<kdtree*>vd;
		for (it = s.begin(); it != s.end(); it ++) {
			pair<int, kdtree*> y = *it;
			vd.push_back(y.second);
		}
		vec.push_back(xy);
		if ((i + 1) % blk) {
			delete_tree(rootaddr);
			rootaddr = NULL;
			rootaddr = static_build(rootaddr, vec, 0, 0, (int)vec.size() - 1);
		}
		else {
			rootaddr = dynamic_build(rootaddr, xy, 0);
		}
	}
}