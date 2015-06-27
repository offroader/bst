#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <map>
#include <cmath>

#include "Tree.cpp"
#include "OST.cpp"
#include "RBT.cpp"

using namespace std;

void test_ostDSW_ostSED_ostMod(int);
void test_build_rbt_ost(int);
void randomInsert(RBT*, OST*, int);
void test_my_tree(int);

int main (int argc, char** argv) {
	int n = 1000000;
	test_build_rbt_ost(n);
	test_my_tree(n);
	test_ostDSW_ostSED_ostMod(n);
}

void test_ostDSW_ostSED_ostMod (int N) {
	if (!N) {
		cout << "invalid tree size" << endl;
		return;
	}

	cout << "Test OST balancing using DSW and Sedgewick's algorithm (also our modification) for " << N << " elements" << endl;

	srand (time(NULL));

	cout << endl<< "N:" << N << endl;

	map<int, int> mymap;

	OST* ost1 = new OST();
	OST* ost2 = new OST();
	OST* ost3 = new OST();

	for (int i = 0; i < N; i++) {
		int k;
		if (rand() % 10 > 5) {
			k = rand() % 10000000000;
		} else {
			k = rand() % 1000;
		}

		if (mymap.find(k) == mymap.end()) {
			mymap.insert(pair<int,int>(k, 1));
			ost1->insert(k);
			ost2->insert(k);
			ost3->insert(k);
		} else {
			i--;
			continue;
		}
	}

	clock_t start, finish;

	cout << "Initial: " << endl;
	ost1->printHeight();
	ost2->printHeight();
	ost3->printHeight();

	cout << endl;

	start = clock();
	ost1->balanceDSW();
	finish = clock();
	cout << "DSW balancing time: " << ((double) (finish - start)) / 1000 << " ms" << endl;
	ost1->printHeight();

	start = clock();
	ost2->balance();
	finish = clock();
	cout << "Sed. balancing time: " << ((double) (finish - start)) / 1000 << " ms" << endl;
	ost2->printHeight();

	start = clock();
	ost3->balanceM();
	finish = clock();
	cout << "Modified Sed. balancing time: " << ((double) (finish - start)) / 1000 << " ms" << endl;
	ost3->printHeight();

	ost1->destroy();
	ost2->destroy();
	ost3->destroy();
}

void test_build_rbt_ost (int tree_size) {
	if (!tree_size) {
		cout << "invalid tree size" << endl;
		return;
	}

	cout << "Test RBT and OST building times for " << tree_size << " elements" << endl;

	map<int, int> mymap;
	int arr[tree_size];

	for (int i = 0; i < tree_size; i++) {
		int k;
		if (rand() % 10 > 5) {
			k = rand() % 100000000;
		} else {
			k = rand() % 1000;
		}

		if (mymap.find(k) == mymap.end()) {
			mymap.insert(pair<int,int>(k, 1));
			arr[i] = k;
		} else {
			i--;
			continue;
		}
	}

	//for (map<int, int>::iterator it = mymap.begin(); it != mymap.end(); ++it) {}

	// check
	cout << "map size: " << mymap.size() << endl;

	RBT* rbt = new RBT();
	OST* ost = new OST();
	clock_t start, finish;

	start = clock();
	for (int i = 0; i < tree_size; i++) {
		rbt->insert(arr[i]);
	}
	finish = clock();
	cout << "RBT building time: " << ((double) (finish - start)) / 1000 << " ms" << endl;


	start = clock();
	for (int i = 0; i < tree_size; i++) {
		ost->insert(arr[i]);
	}
	finish = clock();
	cout << "OST building time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	rbt->printSize();
	rbt->printRoot();
	rbt->printHeight();

	ost->printSize();
	ost->printRoot();
	ost->printHeight();

	rbt->destroy();
	ost->destroy();

//	free(arr);

	cout << "Test finished." << endl;
}

void randomInsert(RBT* rbt, OST* ost, int n) {
	map<int, int> mymap;

	for (int i = 0; i < n; i++) {
		int k;
		if (rand() % 10 > 5) {
			k = rand() % 100000000;
		} else {
			k = rand() % 1000;
		}

		if (mymap.find(k) == mymap.end()) {
			mymap.insert(pair<int,int>(k, 1));
			rbt->insert(k);
			ost->insert(k);
		} else {
			i--;
			continue;
		}
	}
}

void buildSimpleOST (OST* ost) {
	// unbalanced tree for example and drawing
	ost->insert(30);
	ost->insert(1);
	ost->insert(3);
	ost->insert(2);
	ost->insert(20);
	ost->insert(8);
	ost->insert(9);
	ost->insert(7);
	ost->insert(4);
	ost->insert(5);
	ost->insert(10);
}

void test_my_tree (int tree_size) {
	if (!tree_size) {
		cout << "invalid tree size" << endl;
		return;
	}

	cout << "Test our tree balancing time for " << tree_size << " elements" << endl;

	map<int, int> mymap;


	Tree* t1 = new Tree();
	Tree* t2 = new Tree();
	clock_t start, finish;


	for (int i = 0; i < tree_size; i++) {
		int k;
		if (rand() % 10 > 5) {
			k = rand() % 100000000;
		} else {
			k = rand() % 1000;
		}

		if (mymap.find(k) == mymap.end()) {
			mymap.insert(pair<int,int>(k, 1));
			t1->insert(k);
			t2->insert(k);
		} else {
			i--;
			continue;
		}
	}

	start = clock();
	t1->balanceDSW();
	finish = clock();
	cout << "DSW balancing time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	start = clock();
	t2->balance();
	finish = clock();
	cout << "Sedge balancing time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	t1->printHeight();
	t2->printHeight();

	t1->destroy();
	t2->destroy();

	cout << "Test finished." << endl;
}
