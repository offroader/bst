#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <map>

#include "Tree.cpp"
#include "OST.cpp"

using namespace std;

void test(int);
void test1(int);
void osttest(int);
void rbtest(int);
void randomInsert(Tree*, int);
void randomInsert(OST*, int);
void randomInsert(Tree*, OST*, int);

int main(int argc, char** argv) {
	srand (time(NULL));

//	rbtest(1000);
//	osttest(1000);
//
//	rbtest(10000);
//	osttest(10000);
//
//	rbtest(100000);
//	osttest(100000);
//
//	rbtest(1000000);
//	osttest(1000000);
//
//	rbtest(10000000);
//	osttest(10000000);

	test(1000);

	return 0;
}


void test (int tree_size) {
	Tree* tree = new Tree();
	cout << endl << "Tree size: " << tree_size << endl;

	randomInsert(tree, tree_size);

	tree->printSize();
	cout << endl;

	tree->printHeight();

	tree->printRoot();

	tree->convertToOST();

	tree->printHeight();


	tree->destroy();
}

void test1(int tree_size) {
	Tree* rbt = new Tree();
	OST* ost = new OST();

	cout << endl << "Tree size: " << tree_size << endl;

	randomInsert(rbt, ost, tree_size);

	cout << "Size of rbt: ";
	rbt->printSize();
	cout << endl;
	cout << "Size of ost: ";
	ost->printSize();

	cout << endl;

	cout << "Height of rbt: ";
	rbt->printHeight();
	cout << endl;
	cout << "Height of ost: ";
	ost->printHeight();


	cout << endl;

	rbt->printRoot();
	cout << endl;
	ost->printRoot();


	cout << endl;

	rbt->destroy();
	ost->destroy();
}

void osttest (int tree_size) {
	OST* tree = new OST();
	clock_t start, finish;

	cout<< endl<< "OST tree size: " << tree_size << endl;
    tree->printSize();

	start = clock();
	randomInsert(tree, tree_size);
	finish = clock();
	cout << "Build time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	//tree->printInOrder();

	//tree->printSize();
	tree->printHeight();
	tree->printRoot();

	start = clock();
	tree->balance();
	finish = clock();

	//tree->printSize();
	tree->printHeight();
	tree->printRoot();

	cout << "Time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	tree->destroy();

	cout << "Tree is destroyed" << endl << endl;

}

void rbtest (int tree_size) {
	Tree* tree = new Tree();
	clock_t start, finish;

	cout<< endl<< "RB tree size: " << tree_size << endl;
    tree->printSize();

	start = clock();
	randomInsert(tree, tree_size);
	finish = clock();
	cout << "Build time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	//tree->printInOrder();

	//tree->printSize();
	tree->printHeight();
	tree->printRoot();

	start = clock();
	tree->balance();
	finish = clock();

	//tree->printSize();
	tree->printHeight();
	tree->printRoot();

	cout << "Time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	tree->destroy();

	cout << "Tree is destroyed" << endl << endl;

}

void randomInsert(Tree* tree, int n) {
	map<int, int> mymap;

	for (int i = 0; i < n; i++) {
		int k;
		if (std::rand() % 10 > 5) {
			k = std::rand() % 100000000;
		} else {
			k = std::rand() % 1000;
		}

		if (mymap.find(k) == mymap.end()) {
			mymap.insert(pair<int,int>(k, 1));
			tree->insertNode(new Node(k));
		} else {
			i--;
			continue;
		}
	}
}

void randomInsert(OST* tree, int n) {
	for (int i = 0; i < n; i++) {
		int k;
		if (std::rand() % 10 > 5) {
			k = std::rand() % 100000000;
		} else {
			k = std::rand() % 1000;
		}
		OSTNode* node = new OSTNode(k);
		int inserted = tree->insertNode(node);
		if (inserted == 0) {
			delete node;
			i--;
			continue;
		}
	}
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

//	map<int,int>::iterator it;
//	for (it=mymap.begin(); it != mymap.end(); ++it) {}
}

