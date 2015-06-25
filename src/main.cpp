#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <map>
#include <cmath>

#include "Tree.cpp"
#include "OST.cpp"

using namespace std;

void test_ostDSW_ostSED_ostMod(int);
void test_rbt_ost(int);
void randomInsert(Tree*, OST*, int);

void test_ostDSW_ostSED_ostMod (int N) {
	srand (time(NULL));

	cout << endl<< "N:" << N << endl;

	map<int, int> mymap;

	OST* ost1 = new OST();
	OST* ost2 = new OST();
	OST* ost3 = new OST();

	for (int i = 0; i < N; i++) {
		int k;
		if (std::rand() % 10 > 5) {
			k = std::rand() % 1000000000;
		} else {
			k = std::rand() % 1000;
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

int main (int argc, char** argv) {
	test_ostDSW_ostSED_ostMod(1000);
	test_ostDSW_ostSED_ostMod(10000);
	test_ostDSW_ostSED_ostMod(100000);
//	test_ostDSW_and_ostSED(1000000);
//	test_ostDSW_and_ostSED(10000000);
}

void test_rbt_ost (int tree_size) {
	Tree* rbt = new Tree();
	OST* ost = new OST();

	cout << endl << "Tree size: " << tree_size << endl;

	randomInsert(rbt, ost, tree_size);

	rbt->printSize();
	ost->printSize();

	rbt->printHeight();
	ost->printHeight();

	rbt->printRoot();
	ost->printRoot();

	cout << " balancing ... " << endl;

	rbt->balance();
	ost->balance();

	rbt->printSize();
	ost->printSize();

	rbt->printHeight();
	ost->printHeight();

	rbt->printRoot();
	ost->printRoot();

	rbt->destroy();
	ost->destroy();
}

void randomInsert(Tree* rbt, OST* ost, int n) {
	map<int, int> mymap;

	for (int i = 0; i < n; i++) {
		int k;
		if (std::rand() % 10 > 5) {
			k = std::rand() % 100000000;
		} else {
			k = std::rand() % 1000;
		}

		cout << k << " ";
		if (mymap.find(k) == mymap.end()) {
			mymap.insert(pair<int,int>(k, 1));
			cout << rbt->insertNode(new Node(k));
			cout << " - ";
			cout << ost->insertNode(new OSTNode(k));
			cout << endl;
		} else {
			i--;
			continue;
		}
	}
}


void buildSimpleOST (OST* ost) {
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

