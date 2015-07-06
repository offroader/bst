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
void test_rbtDSW_rbtSED(int);
void test_rbtDSW(int);
void test_rbtSED(int);
void test_search_rbt_balancedrbt();

static int MAX_KEY_VALUE = 1000000000;
static int MAX_TREE_SIZE = 100000000;

int main (int argc, char** argv) {
	//for (int n = 1000; n <= MAX_TREE_SIZE; n *= 10) {}
	test_search_rbt_balancedrbt();
}

void test_ostDSW_ostSED_ostMod (int N) {
	if (!N || N > MAX_TREE_SIZE) {
		cout << "Invalid tree size" << endl;
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
		int k = rand() % MAX_KEY_VALUE;

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
	if (!tree_size || tree_size > MAX_TREE_SIZE) {
		cout << "Invalid tree size" << endl;
		return;
	}

	cout << "Test RBT and OST building times for " << tree_size << " elements" << endl;

	map<int, int> mymap;
	int* arr = new int[tree_size];

	for (int i = 0; i < tree_size; i++) {
		int k = rand() % MAX_KEY_VALUE;

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

	free(arr);

	cout << "Test finished." << endl;
}

void test_rbtDSW_rbtSED (int tree_size) {
	if (!tree_size || tree_size > MAX_TREE_SIZE/10) {
		cout << "Invalid tree size" << endl;
		return;
	}
	cout << "Test red-black tree balancing times for " << tree_size << " elements" << endl;

	int* arr = new int[MAX_TREE_SIZE];

	Tree* t1 = new Tree();
	Tree* t2 = new Tree();
	clock_t start, finish;

	cout << "building...." << endl;

	for (int i = 0; i < tree_size; i++) {
		int k = rand() % MAX_KEY_VALUE;
		if (arr[k] != 1) {
			arr[k] = 1;
			t1->insert(k);
			t2->insert(k);
			if (i % 1000000 == 0) cout << i << endl;
		} else {
			i--;
			continue;
		}
	}

	free(arr);

	cout << "balancing...." << endl;

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

void test_rbtDSW (int tree_size) {
	if (!tree_size || tree_size > MAX_TREE_SIZE/10) {
		cout << "Invalid tree size" << endl;
		return;
	}
	cout << "Test red-black tree balancing times for " << tree_size << " elements" << endl;

	Tree* tree = new Tree();
	clock_t start, finish;

	start = clock();
	cout << "building...." << endl;
	for (int i = 0; i < tree_size; i++) {
		tree->insert(rand() % MAX_KEY_VALUE);
	}
	finish = clock();
	cout << "building time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	tree->printSize();
	tree->printHeight();

	cout << "balancing...." << endl;
	start = clock();
	tree->balanceDSW();
	finish = clock();
	cout << "DSW balancing time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	tree->printSize();
	tree->printHeight();

	tree->destroy();

	cout << "Test finished." << endl;
}


void test_rbtSED (int tree_size) {
	if (!tree_size || tree_size > MAX_TREE_SIZE/10) {
		cout << "Invalid tree size" << endl;
		return;
	}
	cout << "Test red-black tree balancing times for " << tree_size << " elements" << endl;

	Tree* tree = new Tree();
	clock_t start, finish;

	start = clock();
	cout << "building...." << endl;
	for (int i = 0; i < tree_size; i++) {
		tree->insert(rand() % MAX_KEY_VALUE);
	}
	finish = clock();
	cout << "building time: " << ((double) (finish - start)) / 1000 << " ms" << endl;


	cout << "balancing...." << endl;
	start = clock();
	tree->balance();
	finish = clock();
	cout << "Sedgewick balancing time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	tree->printSize();
	tree->printHeight();

	tree->destroy();

	cout << "Test finished." << endl;
}


void test_search_rbt_balancedrbt () {
	int tree_size = 10000000;
	int search_size = 100000000;

	Tree* tree = new Tree();
//	int* arr = new int[search_size];
	clock_t start, finish;
	int found, notFound;

	start = clock();
	cout << "building tree...." << endl;
	for (int i = 0; i < tree_size; i++) {
		tree->insert(rand() % MAX_KEY_VALUE);
	}
	finish = clock();
	cout << "building time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

//	cout << "building search array..." << endl;
//	for (int i = 0; i < search_size; i++) {
//		arr[i] = rand() % MAX_KEY_VALUE;
//	}
//	cout << "search array is created. size: " << search_size << endl;

	cout << endl;
	tree->printSize();
	tree->printHeight();

	cout << endl;
	found = 0;
	notFound = 0;
	start = clock();
	cout << "searching...." << endl;
	for (int i = 0; i < search_size; i++) {
//		if (tree->find(arr[i]) == 1) found++;
//		if (tree->find(i) == 1) found++;
//		else notFound++;
		tree->find(i);
	}
	finish = clock();
	cout << "searching time: " << ((double) (finish - start)) / 1000 << " ms" << endl;
	cout << "found: "<< found << ". notFound: " << notFound << endl;

	cout << endl;
	cout << "balancing...." << endl;
	start = clock();
	tree->balanceDSW();
	finish = clock();
	cout << "balancing time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	cout << endl;
	tree->printSize();
	tree->printHeight();

	cout << endl;
	found = 0;
	notFound = 0;
	start = clock();
	cout << "searching...." << endl;
	for (int i = 0; i < search_size; i++) {
		//if (tree->find(arr[i]) == 1) found++;
		tree->find(i);
	}
	finish = clock();
	cout << "searching time: " << ((double) (finish - start)) / 1000 << " ms" << endl;
	cout << "found: "<< found << ". notFound: " << notFound << endl;

	tree->destroy();

	cout << endl;
	cout << "Test finished successfully." << endl;
}
